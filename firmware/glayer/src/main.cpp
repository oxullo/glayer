#include <Arduino.h>

#include <Adafruit_VS1053.h>

#include "ArcadeButton.h"
#include "LightBarrier.h"
#include "RFIDReader.h"

const int LED_UP_PIN = 16;
const int PB_UP_PIN = 17;
const int LED_DN_PIN = 18;
const int PB_DN_PIN = 19;

const int LB_LED_PIN = 14;
const int LB_COLLECTOR_PIN = 15;

const int RFID_READER_IRQ_PIN = 0;

const int VS1053_RESET = -1; // = 11;
const int VS1053_MP3CS = 6;
const int VS1053_XDCS = 10;
const int VS1053_DREQ = 9;
const int SDCARDCS = 5;

typedef enum SystemState {
    SYSSTATE_INIT,
    SYSSTATE_WAIT_CARD,
    SYSSTATE_CARD_DETECTED,
    SYSSTATE_CARD_IDENTIFIED
} SystemState;


ArcadeButton button_up(PB_UP_PIN, LED_UP_PIN), button_down(PB_DN_PIN,
        LED_DN_PIN);

LightBarrier light_barrier(LB_LED_PIN, LB_COLLECTOR_PIN);

RFIDReader rfid_reader(RFID_READER_IRQ_PIN);

Adafruit_VS1053_FilePlayer player = Adafruit_VS1053_FilePlayer(
        VS1053_RESET, VS1053_MP3CS, VS1053_XDCS, VS1053_DREQ, SDCARDCS);

SystemState system_state = SYSSTATE_INIT;

RFIDUid rfid_uid;


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

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        ;

    button_up.begin();
    button_down.begin();

    if (!rfid_reader.begin()) {
        // TODO: handle
        while(1);
    }

    rfid_reader.powerdown();

    if (!player.begin()) {
        // TODO: handle
        while(1);
    }
    player.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
    player.setVolume(10, 10);

    if (!SD.begin(SDCARDCS)) {
        // TODO: handle
        while(1);
    }
}

void loop()
{
    switch (system_state) {
        case SYSSTATE_INIT:
            button_down.update();
            button_up.update();
            change_state(SYSSTATE_WAIT_CARD);
            break;

        case SYSSTATE_WAIT_CARD:
            if (light_barrier.check_card()) {
                change_state(SYSSTATE_CARD_DETECTED);
            } else {
                delay(1000);
            }
            break;

        case SYSSTATE_CARD_DETECTED:
        {
            bool result = rfid_reader.read_passive_uid(RFIDReader::BAUDRATE_106K_ISO14443A,
                    &rfid_uid, 100);

            if (result) {
                change_state(SYSSTATE_CARD_IDENTIFIED);
            } else if (!light_barrier.check_card()) {
                change_state(SYSSTATE_WAIT_CARD);
            } else {
                delay(500);
            }

            break;
        }

        case SYSSTATE_CARD_IDENTIFIED:
        {
            static uint32_t ts_last_card_check = 0;

            button_down.update();
            button_up.update();

            if (millis() - 1000 > ts_last_card_check) {
                if (!light_barrier.check_card()) {
                    change_state(SYSSTATE_WAIT_CARD);
                }
                ts_last_card_check = millis();
            }
            break;
        }

    }
}
