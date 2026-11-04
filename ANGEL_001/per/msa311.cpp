#include "msa311.h"

bool MSA311::Init(I2CHandle *_i2c) {
    i2c = _i2c;

    // Ensuring that it has the correct Part ID
    buf[0] = MSA311_REG_PARTID;
    i2c->TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    if (buf[0] != 0x13) return false;

    // Setting Data Rate (500hz)
    buf[0] = MSA311_REG_ODR;
	buf[1] = 0b00001001;
	i2c->TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    // Setting Range (2g)
    buf[0] = MSA311_REG_RESRANGE;
	buf[1] = 0b00000000;
	i2c->TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    // Setting Power Mode (Normal, 250Hz BW)
    buf[0] = MSA311_REG_POWERMODE;
	buf[1] = 0b00010010;
	i2c->TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, TIMEOUT);

    return true;
}

void MSA311::Read(uint16_t REG, uint16_t size) {
    buf[0] = REG;
    i2c->TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, TIMEOUT);
    i2c->ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, size, TIMEOUT);
}

int16_t MSA311::byteToInt() {
    int16_t t = 0 | (buf[0] >> 4);
    t |= buf[1] << 4;
    if (buf[1] >> 7) t |= 0b1111 << 12;
    return t;
}

void MSA311::Update() {
    Read(MSA311_REG_OUT_X_L, 2);
    X = byteToInt();

    Read(MSA311_REG_OUT_Y_L, 2);
    Y = byteToInt();

    Read(MSA311_REG_OUT_Z_L, 2);
    Z = byteToInt();
}