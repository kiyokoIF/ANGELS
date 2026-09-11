#include "cap1188.h"

bool CAP1188::Init(I2CHandle *_i2c) {
    i2c = _i2c;

    Read(CAP1188_PRODID, 1);
    if (buf[0] != 0x50) return false;

    Read(CAP1188_MANUID, 1);
    if (buf[0] != 0x5D) return false;

    buf[0] = CAP1188_MTBLK;
    buf[1] = 0x00;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1188_LEDLINK;
    buf[1] = 0x00;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1188_MAIN;
    buf[1] = 0x00 || GAIN << 6;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1188_INT_EN;
    buf[1] = 0xFF;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1188_STANDBYCFG;
    buf[1] = 0xFF;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);

    return true;
}

void CAP1188::Read(uint16_t REG, uint16_t size) {
    buf[0] = REG;
    i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(CAP1188_I2CADDR, buf, size, TIMEOUT);
}

void CAP1188::Update() {
    Read(CAP1188_SENINPUTSTATUS, 1);
    sens = buf[0];
    // resetting interupt bit
    if (sens) {
        buf[0] = CAP1188_MAIN;
        buf[1] = 0x00 || GAIN << 6;
        i2c->TransmitBlocking(CAP1188_I2CADDR, buf, 2, TIMEOUT);
    }
}