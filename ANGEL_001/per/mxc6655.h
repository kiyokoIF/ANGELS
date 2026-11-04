#pragma once
#ifndef MXC6655_H
#define MXC6655_H
#include <daisy_seed.h>
/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define MXC6655_I2CADDR_DEFAULT 0x15  ///< Fixed I2C address

/*=========================================================================
I2C REGISTERS
-----------------------------------------------------------------------*/
#define MXC6655_REG_ORIENT_INT 0x00     ///< Register address for orientation interrupt
#define MXC6655_REG_DATA_INT 0x01       ///< Register address for data interrupt
#define MXC6655_REG_STATUS 0x02         ///< Register address for orientation status
#define MXC6655_REG_OUT_X_H 0x03        ///< Register address for X axis higher byte
#define MXC6655_REG_OUT_X_L 0x04        ///< Register address for X axis lower byte
#define MXC6655_REG_OUT_Y_H 0x05        ///< Register address for Y axis higher byte
#define MXC6655_REG_OUT_Y_L 0x06        ///< Register address for Y axis lower byte
#define MXC6655_REG_OUT_Z_H 0x07        ///< Register address for Z axis higher byte
#define MXC6655_REG_OUT_Z_L 0x08        ///< Register address for Z axis lower byte
#define MXC6655_REG_TEMP_OUT 0x09       ///< Register address for temperature output
#define MXC6655_REG_OINT_SET 0x0A       ///< Register address for orientation interrupt enable/disable
#define MXC6655_REG_DINT_SET 0x0B       ///< Register address for data interrupt enable/disable
#define MXC6655_REG_SHAKE_DET 0x0C      ///< Register address for shake threshold
#define MXC6655_REG_CONTROL 0x0D        ///< Register address for force scale and power down
#define MXC6655_REG_DEV_ID 0x0F         ///< Register address for device ID

/*=========================================================================
I2C BIT MASKS
-----------------------------------------------------------------------*/
#define MXC6655_MASK_DEV_ID 0xFA        ///< Binary & with DEVID should be 0
#define MXC6655_MASK_DATA_INT 0x01      ///< Binary & means data ready

/*=========================================================================*/

#define TIMEOUT 1000

using namespace daisy;

class MXC6655 {
public:
    I2CHandle *i2c;
    int16_t X;
    int16_t Y;
    int16_t Z;
    uint8_t buf[2];

    /**
     * @brief Reads selected register into buffer
     * @param REG register to be read from
     * @param size number of bytes to read
     */
    void Read(uint16_t REG, uint16_t size);

    /**
     * @brief Reads selected register into buffer
     * @param REG register to be read from
     * @param size number of bytes to read
     */
    void ReadDMA(uint16_t REG, uint16_t size);

    /**
     * @brief convert bytes in buffer to signed integer
     * @return signed integer
     */
    int16_t byteToInt();

public:
    MXC6655() {};

    /**
     * @brief Initialize the device to default settings
     * @return boolean if initialization was successful or not
     */
    bool Init(I2CHandle *_i2c);

    /**
     * @brief Called to update X Y, and Z coordinates
     * 
     */
    void Update();

    int16_t getX() { return X; };
    int16_t getY() { return Y; };
    int16_t getZ() { return Z; };
};

#endif
