/*
 * RFIDReader.cpp
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
 */

// Extracted from Adafruit PN532 library
// https://github.com/adafruit/Adafruit-PN532
// Interfacing documentation: https://www.nxp.com/docs/en/user-guide/141520.pdf

#include <Arduino.h>
#include <Wire.h>

#include "RFIDReader.h"
#include "pn532_regs.h"

namespace {
const uint16_t PN532_PACKBUFFSIZ = 64;

uint8_t pn532ack[] = { 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00 };
uint8_t pn532_packetbuffer[PN532_PACKBUFFSIZ];
}

RFIDReader::RFIDReader(int irq_pin) :
        irq_pin_(irq_pin)
{
    pinMode(irq_pin, INPUT);
}

bool RFIDReader::begin()
{
    Wire.begin();

    pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode;
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!

    if (!send_cmd(pn532_packetbuffer, 4)) {
        return false;
    }

    // read data packet
    read(pn532_packetbuffer, 8);

    return (pn532_packetbuffer[6] == 0x15);
}

uint32_t RFIDReader::get_fw_version()
{
    uint32_t response;

    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (!send_cmd(pn532_packetbuffer, 1)) {
        return 0;
    }

    // read data packet
    read(pn532_packetbuffer, 12);

    int offset = 7; // Skip a response byte when using I2C to ignore extra data.
    response = pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset++];

    return response;
}

bool RFIDReader::read_passive_uid(Baudrate baudrate, RFIDUid *uid, uint16_t timeout)
{
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1; // max 1 cards at once (we can set this to 2 later)
    pn532_packetbuffer[2] = (uint8_t)baudrate;

    if (!send_cmd(pn532_packetbuffer, 3, timeout)) {
#ifdef PN532DEBUG
        Serial.println(F("No card(s) read"));
#endif
        return 0x0;  // no cards read
    }

    // wait for a card to enter the field (only possible with I2C)
#ifdef PN532DEBUG
    Serial.println(F("Waiting for IRQ (indicates card presence)"));
#endif
    if (!wait_ready(timeout)) {
#ifdef PN532DEBUG
        Serial.println(F("IRQ Timeout"));
#endif
        return 0x0;
    }

    // read data packet
    read(pn532_packetbuffer, 20);
    // check some basic stuff

    /* ISO14443A card response should be in the following format:

     byte            Description
     -------------   ------------------------------------------
     b0..6           Frame header and preamble
     b7              Tags Found
     b8              Tag Number (only one used in this example)
     b9..10          SENS_RES
     b11             SEL_RES
     b12             NFCID Length
     b13..NFCIDLen   NFCID                                      */

#ifdef MIFAREDEBUG
    Serial.print(F("Found "));
    Serial.print(pn532_packetbuffer[7], DEC);
    Serial.println(F(" tags"));
#endif
    if (pn532_packetbuffer[7] != 1) {
        return 0;
    }

    uint16_t sens_res = pn532_packetbuffer[9];
    sens_res <<= 8;
    sens_res |= pn532_packetbuffer[10];
#ifdef MIFAREDEBUG
    Serial.print(F("ATQA: 0x"));
    Serial.println(sens_res, HEX);
    Serial.print(F("SAK: 0x"));
    Serial.println(pn532_packetbuffer[11], HEX);
#endif

    /* Card appears to be Mifare Classic */
    uid->length = pn532_packetbuffer[12];
#ifdef MIFAREDEBUG
    Serial.print(F("UID:"));
#endif
    for (uint8_t i = 0; i < pn532_packetbuffer[12]; i++) {
        uid->uid[i] = pn532_packetbuffer[13 + i];
#ifdef MIFAREDEBUG
        Serial.print(F(" 0x"));
        Serial.print(uid->uid[i], HEX);
#endif
    }
#ifdef MIFAREDEBUG
    Serial.println();
#endif

    return 1;
}

bool RFIDReader::powerdown()
{
    pn532_packetbuffer[0] = PN532_COMMAND_POWERDOWN;
    pn532_packetbuffer[1] = 1 << 7; // WakeUpEnable I2C

    if (!send_cmd(pn532_packetbuffer, 2)) {
        return false;
    }

    // read data packet
    read(pn532_packetbuffer, 8);

    return ((pn532_packetbuffer[6] == 0x17) && ((pn532_packetbuffer[7] & 0x3f) == 0));

}

void RFIDReader::read(uint8_t* buff, uint8_t n)
{
    // TODO: Necessary?
    delay(2);

#ifdef PN532DEBUG
    Serial.print(F("Reading: "));
#endif
    // Start read (n+1 to take into account leading 0x01 with I2C)
    Wire.requestFrom((uint8_t) PN532_I2C_ADDRESS, (uint8_t) (n + 2));
    // Discard the leading 0x01
    Wire.read();
    for (uint8_t i = 0; i < n; i++) {
        delay(1);
        buff[i] = Wire.read();
#ifdef PN532DEBUG
        Serial.print(F(" 0x"));
        Serial.print(buff[i], HEX);
#endif
    }
    // Discard trailing 0x00 0x00
    // Wire.read();

#ifdef PN532DEBUG
    Serial.println();
#endif
}

void RFIDReader::write_cmd(uint8_t* cmd, uint8_t cmdlen)
{
    // I2C command write.
    uint8_t checksum;

    cmdlen++;

#ifdef PN532DEBUG
    Serial.print(F("\nSending: "));
#endif

    delay(2);     // or whatever the delay is for waking up the board

    // I2C START
    Wire.beginTransmission(PN532_I2C_ADDRESS);
    checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
    Wire.write(PN532_PREAMBLE);
    Wire.write(PN532_PREAMBLE);
    Wire.write(PN532_STARTCODE2);

    Wire.write(cmdlen);
    Wire.write(~cmdlen + 1);

    Wire.write(PN532_HOSTTOPN532);
    checksum += PN532_HOSTTOPN532;

#ifdef PN532DEBUG
    Serial.print(F(" 0x")); Serial.print((byte)PN532_PREAMBLE, HEX);
    Serial.print(F(" 0x")); Serial.print((byte)PN532_PREAMBLE, HEX);
    Serial.print(F(" 0x")); Serial.print((byte)PN532_STARTCODE2, HEX);
    Serial.print(F(" 0x")); Serial.print((byte)cmdlen, HEX);
    Serial.print(F(" 0x")); Serial.print((byte)(~cmdlen + 1), HEX);
    Serial.print(F(" 0x")); Serial.print((byte)PN532_HOSTTOPN532, HEX);
#endif

    for (uint8_t i = 0; i < cmdlen - 1; i++) {
        Wire.write(cmd[i]);
        checksum += cmd[i];
#ifdef PN532DEBUG
        Serial.print(F(" 0x")); Serial.print((byte)cmd[i], HEX);
#endif
    }

    Wire.write((byte) ~checksum);
    Wire.write((byte) PN532_POSTAMBLE);

    // I2C STOP
    Wire.endTransmission();

#ifdef PN532DEBUG
    Serial.print(F(" 0x")); Serial.print((byte)~checksum, HEX);
    Serial.print(F(" 0x")); Serial.print((byte)PN532_POSTAMBLE, HEX);
    Serial.println();
#endif
}

bool RFIDReader::is_ready()
{
    // I2C check if status is ready by IRQ line being pulled low.
    uint8_t x = digitalRead(irq_pin_);
    return x == 0;
}

bool RFIDReader::read_ack()
{
    uint8_t ackbuff[6];

    read(ackbuff, 6);

    return (0 == strncmp((char *) ackbuff, (char *) pn532ack, 6));
}

bool RFIDReader::wait_ready(uint16_t timeout)
{
    uint16_t timer = 0;
    while (!is_ready()) {
        if (timeout != 0) {
            timer += 10;
            if (timer > timeout) {
#ifdef PN532DEBUG
                Serial.println("TIMEOUT!");
#endif
                return false;
            }
        }
        delay(10);
    }
    return true;
}

bool RFIDReader::send_cmd(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout)
{
    // write the command
    write_cmd(cmd, cmdlen);

    // Wait for chip to say its ready!
    if (!wait_ready(timeout)) {
        return false;
    }

#ifdef PN532DEBUG
    Serial.println(F("IRQ received"));
#endif

    // read acknowledgement
    if (!read_ack()) {
#ifdef PN532DEBUG
        Serial.println(F("No ACK frame received!"));
#endif
        return false;
    }

    return true; // ack'd command
}
