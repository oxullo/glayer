/*
Glayer MP3 player

Copyright (C) 2018  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>

#include <Adafruit_VS1053.h>
#include <Adafruit_TPA2016.h>
#include <SD.h>

#include "UserInterface.h"
#include "LightBarrier.h"
#include "RFIDReader.h"
#include "Sequencer.h"

// #define DEBUG

namespace {
const int LB_LED_PIN = 14;
const int LB_COLLECTOR_PIN = 15;

const int AMP_SHUTDOWN_PIN = 12;
const int AMP_LIMIT_LEVEL = 10;

const int RFID_READER_IRQ_PIN = 0;

const int VS1053_RESET = -1; // = 11;
const int VS1053_MP3CS = 6;
const int VS1053_XDCS = 10;
const int VS1053_DREQ = 9;

const int SDCARDCS = 5;

const uint8_t AUDIO_VOLUME_INITIAL = 50;
const uint8_t AUDIO_VOLUME_MIN = 15;
const uint8_t AUDIO_VOLUME_MAX = 75;

const uint32_t IDLE_NOTIFICATION_PERIOD = 30000;

const uint32_t WAIT_CARD_POLL_DELAY = 200;
}


typedef enum SystemState {
    SYSSTATE_INIT,
    SYSSTATE_WAIT_CARD,
    SYSSTATE_CARD_DETECTED,
    SYSSTATE_CARD_IDENTIFIED
} SystemState;


UserInterface ui;
Sequencer sequencer;

LightBarrier light_barrier(LB_LED_PIN, LB_COLLECTOR_PIN);

RFIDReader rfid_reader(RFID_READER_IRQ_PIN);
RFIDUid rfid_uid;

uint8_t audio_volume = AUDIO_VOLUME_INITIAL;
Adafruit_TPA2016 audioamp = Adafruit_TPA2016();
Adafruit_VS1053_FilePlayer player = Adafruit_VS1053_FilePlayer(VS1053_RESET,
        VS1053_MP3CS, VS1053_XDCS, VS1053_DREQ, SDCARDCS);

SystemState system_state = SYSSTATE_INIT;

uint32_t ts_idle_timer = 0;


void change_state(SystemState new_state)
{
    if (new_state != system_state) {
        Serial.print(F("Changing state: "));
        Serial.print(system_state);
        Serial.print(F("->"));
        Serial.println(new_state);

        system_state = new_state;
    }
}

void audio_set_enabled(bool enabled)
{
    digitalWrite(AMP_SHUTDOWN_PIN, enabled ? HIGH : LOW);
    if (enabled) {
        delay(5);
        audioamp.setAGCCompression(TPA2016_AGC_2);
        audioamp.setAGCMaxGain(2);
        audioamp.setReleaseControl(0);
    }
}

void audio_change_volume(int8_t offset)
{
    if (offset < 0) {
        if (audio_volume - AUDIO_VOLUME_MIN > -offset) {
            audio_volume += offset;
        } else {
            audio_volume = AUDIO_VOLUME_MIN;
        }
    } else {
        if (AUDIO_VOLUME_MAX - audio_volume > offset) {
            audio_volume += offset;
        } else {
            audio_volume = AUDIO_VOLUME_MAX;
        }
    }

    Serial.print(F("Setting volume level to: "));
    Serial.println(audio_volume);

    player.setVolume(audio_volume, audio_volume);
}

void play_jingle()
{
    Serial.print("Playing jingle..");
    audio_set_enabled(true);
    player.stopPlaying();
    player.playFullFile("start.mp3");
    Serial.println("..done.");
    audio_set_enabled(false);
}

void setup()
{
    Serial.begin(115200);

    ui.begin();

    pinMode(AMP_SHUTDOWN_PIN, OUTPUT);

#ifdef DEBUG
    while (!Serial);
#endif

    audioamp.begin();

    if (!rfid_reader.begin()) {
        ui.set_fatal_error(1);
    }
    rfid_reader.powerdown();

    if (!player.begin()) {
        ui.set_fatal_error(2);
    }
    player.useInterrupt(VS1053_FILEPLAYER_PIN_INT);

    if (!SD.begin(SDCARDCS)) {
        ui.set_fatal_error(3);
    }

    play_jingle();

    sequencer.begin(&player);
}

void loop()
{
    switch (system_state) {
        case SYSSTATE_INIT:
            ui.reset();
            change_state(SYSSTATE_WAIT_CARD);
            break;

        case SYSSTATE_WAIT_CARD:
            if (light_barrier.check_card()) {
                change_state(SYSSTATE_CARD_DETECTED);
            } else {
                if (millis() - ts_idle_timer > IDLE_NOTIFICATION_PERIOD) {
                    ui.blink();
                    ts_idle_timer = millis();
                } else {
                    delay(WAIT_CARD_POLL_DELAY);
                }
            }
            break;

        case SYSSTATE_CARD_DETECTED: {
            // TODO: magic number
            bool result = rfid_reader.read_passive_uid(
                    RFIDReader::BAUDRATE_106K_ISO14443A, &rfid_uid, 100);

            rfid_reader.powerdown();

            if (result) {
                char buffer[18];

                if (rfid_uid.length == 4) {
                    sprintf(buffer, "%02x%02x%02x%02x", rfid_uid.uid[0],
                            rfid_uid.uid[1], rfid_uid.uid[2], rfid_uid.uid[3]);
                } else if (rfid_uid.length == 8) {
                    sprintf(buffer, "%02x%02x%02x%02x%02x%02x%02x%02x",
                            rfid_uid.uid[0], rfid_uid.uid[1], rfid_uid.uid[2],
                            rfid_uid.uid[3], rfid_uid.uid[4], rfid_uid.uid[5],
                            rfid_uid.uid[6], rfid_uid.uid[7]);
                } else {
                    // TODO: handle error
                    delay(500);
                    return;
                }

                audio_set_enabled(true);

                if (sequencer.start(buffer)) {
                    ui.blink();
                }
                change_state(SYSSTATE_CARD_IDENTIFIED);
            } else if (!light_barrier.check_card()) {
                audio_set_enabled(false);
                ui.reset();
                sequencer.reset();

                ts_idle_timer = millis();
                change_state(SYSSTATE_WAIT_CARD);
            } else {
                // TODO: magic number
                delay(500);
            }

            break;
        }

        case SYSSTATE_CARD_IDENTIFIED: {
            static uint32_t ts_last_card_check = 0;
            static uint32_t ts_last_volume_change = 0;

            UserInterface::Action action = ui.update();

            switch (action) {
                case UserInterface::ACTION_INCREASE_VOLUME:
                case UserInterface::ACTION_DECREASE_VOLUME:
                    // TODO: magic number
                    if (millis() - 100 > ts_last_volume_change) {
                        if (action == UserInterface::ACTION_INCREASE_VOLUME) {
                            audio_change_volume(-1);
                        } else {
                            audio_change_volume(1);
                        }
                        ts_last_volume_change = millis();
                    }
                    break;

                case UserInterface::ACTION_NEXT_TRACK:
                    sequencer.next();
                    break;

                case UserInterface::ACTION_PREVIOUS_TRACK:
                    sequencer.previous();
                    break;

                case UserInterface::ACTION_NONE:
                    sequencer.check();
                    break;
            }

            // TODO: magic number
            if (millis() - 1000 > ts_last_card_check) {
                if (!light_barrier.check_card()) {
                    audio_set_enabled(false);
                    ui.reset();
                    sequencer.reset();

                    change_state(SYSSTATE_WAIT_CARD);
                }
                ts_last_card_check = millis();
            }
            break;
        }

    }
}
