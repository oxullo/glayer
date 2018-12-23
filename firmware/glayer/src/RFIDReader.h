/*
 * RFIDReader.h
 *
 *  Created on: 23 Dec 2018
 *      Author: xi
 */

#ifndef SRC_RFIDREADER_H_
#define SRC_RFIDREADER_H_

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
    bool read_passive_uid(Baudrate baudrate, uint8_t * uid, uint8_t * uidLength,
            uint16_t timeout = 1000);
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
