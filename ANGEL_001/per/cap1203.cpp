#include "cap1203.h"

bool CAP1203::Init(I2CHandle *_i2c) {
    i2c = _i2c;

    Read(CAP1203_PRODUCT_ID, 1);
    if (buf[0] != 0x6D) return false;

    Read(CAP1203_MAN_ID, 1);
    if (buf[0] != 0x5D) return false;

    buf[0] = CAP1203_SENSINPUTEN;
    buf[1] = 0x07;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1203_INT_ENABLE;
    buf[1] = 0x07;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1203_REPEAT_RATE;
    buf[1] = 0x00;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 2, TIMEOUT);

    buf[0] = CAP1203_MULTITOUCH;
    buf[1] = 0x0C;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 2, TIMEOUT);

    return true;
}

void CAP1203::Read(uint16_t REG, uint16_t size) {
    buf[0] = REG;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(CAP1203_I2CADDR, buf, size, TIMEOUT);
}

void CAP1203::Update() {
    // check if interrupt
    Read(CAP1203_MAIN_CTRL_REG, 1);
    if (!buf[0]) return;

    Read(CAP1203_SENSOR_INPUTS, 1);
    sens = buf[0];
    
    // resetting interupt bit
    buf[0] = CAP1203_MAIN_CTRL_REG;
    buf[1] = 0x00;
    i2c->TransmitBlocking(CAP1203_I2CADDR, buf, 2, TIMEOUT);
}