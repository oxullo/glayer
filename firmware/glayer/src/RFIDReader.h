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


#ifndef SRC_RFIDREADER_H_
#define SRC_RFIDREADER_H_

typedef struct RFIDUid {
    uint8_t uid[8];
    uint8_t length;
} RFIDUid;


class RFIDReader {
public:
    typedef enum Baudrate {
        BAUDRATE_106K_ISO14443A = 0,
        BAUDRATE_212K_FELICA,
        BAUDRATE_424K_FELICA,
        BAUDRATE_106K_ISO14443B,
        BAUDRATE_106K_INNOVISION_JEWEL
    } Baudrate;

    RFIDReader(int irq_pin);

    bool begin();
    uint32_t get_fw_version();
    bool read_passive_uid(Baudrate baudrate, RFIDUid *uid, uint16_t timeout = 1000);
    bool powerdown();

private:
    int irq_pin_;

    void read(uint8_t* buff, uint8_t n);
    void write_cmd(uint8_t* cmd, uint8_t cmdlen);
    bool is_ready();
    bool read_ack();
    bool wait_ready(uint16_t timeout);
    bool send_cmd(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout = 1000);
};

#endif /* SRC_RFIDREADER_H_ */
