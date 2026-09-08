#include "msa311.h"

bool MSA311::Init() {
    I2CHandle::Config i2c_conf;

	i2c_conf.periph 		= I2CHandle::Config::Peripheral::I2C_1;
    i2c_conf.speed 			= I2CHandle::Config::Speed::I2C_100KHZ;
    i2c_conf.mode 			= I2CHandle::Config::Mode::I2C_MASTER;
    i2c_conf.pin_config.scl = seed::D11;
    i2c_conf.pin_config.sda = seed::D12;

    i2c.Init(i2c_conf);

    // Ensuring that it has the correct Part ID
    buf[0] = MSA311_REG_PARTID;
    i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
    i2c.ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
    if (buf[0] != 0x13) return false;

    // Setting Data Rate (500hz)
    buf[0] = MSA311_REG_ODR;
	buf[1] = 0b00001001;
	i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);

    // Setting Range (2g)
    buf[0] = MSA311_REG_RESRANGE;
	buf[1] = 0b00000000;
	i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);

    // Setting Power Mode (Normal, 250Hz BW)
    buf[0] = MSA311_REG_POWERMODE;
	buf[1] = 0b00010010;
	i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);

    return true;
}

void MSA311::Update() {
    buf[0] = MSA311_REG_OUT_X_L;
    i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
    i2c.ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);
    int16_t x = 0 | (buf[0] >> 4);
    x |= buf[1] << 4;
    if ((buf[1] >> 7) == 1) x |= 0b1111 << 12;
    X = x;

    buf[0] = MSA311_REG_OUT_Y_L;
    i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
    i2c.ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);
    int16_t y = 0 | (buf[0] >> 4);
    y |= buf[1] << 4;
    if ((buf[1] >> 7) == 1) y |= 0b1111 << 12;
    Y = y;

    buf[0] = MSA311_REG_OUT_Z_L;
    i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
    i2c.ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 2, 1000);
    int16_t z = 0 | (buf[0] >> 4);
    z |= buf[1] << 4;
    if ((buf[1] >> 7) == 1) z |= 0b1111 << 12;
    Z = z;
}