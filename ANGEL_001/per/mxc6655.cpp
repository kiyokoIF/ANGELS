#include "mxc6655.h"


bool MXC6655::Init(I2CHandle *_i2c) {
    i2c = _i2c;

    // Ensuring that it has the correct Part ID
    buf[0] = MXC6655_REG_DEV_ID;
    i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(MXC6655_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    if (buf[0] & MXC6655_MASK_DEV_ID) return false;

    // Setting Range (2g) and power down false
    buf[0] = MXC6655_REG_CONTROL;
	buf[1] = 0b00000000;
	i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    // Disabling Orientation / Shake interrupts
    buf[0] = MXC6655_REG_OINT_SET;
	buf[1] = 0b00000000;
	i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    // Disabling TC, enabling data interrupt
    buf[0] = MXC6655_REG_DINT_SET;
	buf[1] = 0b00000001;
	i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    return true;
}

void MXC6655::Read(uint16_t REG, uint16_t size) {
    buf[0] = REG;
    i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(MXC6655_I2CADDR_DEFAULT, buf, size, TIMEOUT);
}

int16_t MXC6655::byteToInt() {
    int16_t t = 0 | (buf[1] >> 4);
    t |= buf[0] << 4;
    if (buf[0] >> 7) t |= 0b1111 << 12; // setting correct negative int val
    return t;
}

void MXC6655::Update() {
    // Check if data ready?
    Read(MXC6655_REG_DATA_INT, 1);
    if (!(buf[0] & MXC6655_MASK_DATA_INT)) return;

    Read(MXC6655_REG_OUT_X_H, 2);
    X = byteToInt();

    Read(MXC6655_REG_OUT_Y_H, 2);
    Y = byteToInt();

    Read(MXC6655_REG_OUT_Z_H, 2);
    Z = byteToInt();

    // Resetting data interrupt
    buf[0] = MXC6655_REG_DATA_INT;
	buf[1] = 0b00000000;
	i2c->TransmitBlocking(MXC6655_I2CADDR_DEFAULT, buf, 2, TIMEOUT);
}