#include <Arduino.h>

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

ArcadeButton button_up(PB_UP_PIN, LED_UP_PIN), button_down(PB_DN_PIN,
        LED_DN_PIN);
LightBarrier light_barrier(LB_LED_PIN, LB_COLLECTOR_PIN);
RFIDReader rfid_reader(RFID_READER_IRQ_PIN);

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        ;

    rfid_reader.begin();

    uint32_t fw_ver = rfid_reader.get_fw_version();
    Serial.print("PN532 fw version: ");
    Serial.println(fw_ver, HEX);

    rfid_reader.powerdown();
}

void loop()
{
    static bool last = false;
    bool current = light_barrier.check_card();

    if (current != last) {
        uint8_t buffer[8];
        uint8_t uid_length;

        bool result = rfid_reader.read_passive_uid(RFIDReader::BAUDRATE_106K_ISO14443A,
                buffer, &uid_length, 1000);

        Serial.print("Result=");
        Serial.print(result);
        Serial.print(" len=");
        Serial.println(uid_length);

        delay(500);
    }

    last = current;
}
