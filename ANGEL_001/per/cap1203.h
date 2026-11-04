#pragma once
#ifndef CAP1203_H
#define CAP1203_H

#include <daisy_seed.h>

#define CAP1203_I2CADDR 0x28 ///< The default I2C address

/*****************************Registers**************************/
#define CAP1203_MAIN_CTRL_REG			0x00
#define CAP1203_GEN_STATUS				0x02
#define CAP1203_SENSOR_INPUTS			0x03
#define CAP1203_SENSINPUTEN				0x21
#define CAP1203_INT_ENABLE				0x27
#define CAP1203_REPEAT_RATE				0x28
#define CAP1203_MULTITOUCH				0x2A

#define CAP1203_BASECOUNT				0x2E
#define CAP1203_RECALCONFIG				0x2F

#define CAP1203_STANDBYCHAN				0x40
#define CAP1203_STANDBYCONF				0x41
#define CAP1203_STANDBY_SENS			0x42
#define CAP1203_STANDBY_THRE			0x43
#define CAP1203_CONFIG2					0x44
#define CAP1203_S1BASECOUNT				0x50
#define CAP1203_S2BASECOUNT				0x51
#define CAP1203_S3BASECOUNT				0x52
#define CAP1203_PWR_BUTTON				0x60
#define CAP1203_PWR_CONFIG				0x61
#define CAP1203_S1INPCAL				0xB1	
#define CAP1203_S2INPCAL				0xB2
#define CAP1203_S3INPCAL				0xB3
#define CAP1203_S1CALLSB				0xB9

#define CAP1203_PRODUCT_ID				0xFD
#define CAP1203_MAN_ID					0xFE
#define CAP1203_REV						0xFF	

#define TIMEOUT 1000

using namespace daisy;

class CAP1203 {
private:
    I2CHandle *i2c;
    uint8_t sens;

    /**
     * @brief Read a register
     * 
     * @param REG 
     * @param size 
     */
    void Read(uint16_t REG, uint16_t size);

    uint8_t buf[2];

public:
    CAP1203() {}

    /**
     * @brief Initialize the CAP1203 chip
     * 
     * @param _i2c I2C Handle object for this chip.
     * @return true: initialized successfully
     * @return false: could not find the chip
     */
    bool Init(I2CHandle *_i2c);

    /**
     * @brief 
     * 
     */
    void Update();

    /**
     * @brief Get sensor status
     * 1 bit per sensor. only lower 3 bits used. 
     * @return uint8_t 
     */
    uint8_t getSens() { return sens; };

};

#endif