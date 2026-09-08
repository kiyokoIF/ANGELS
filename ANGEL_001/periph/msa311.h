#pragma once
#ifndef MSA311_H
#define MSA311_H
#include <daisy_seed.h>
#include <daisysp.h>
/*=========================================================================
I2C ADDRESS/BITS
-----------------------------------------------------------------------*/
#define MSA311_I2CADDR_DEFAULT (0x62) ///< Fixed I2C address
/*=========================================================================*/

#define MSA311_REG_PARTID 0x01    ///< Register that contains the part ID
#define MSA311_REG_OUT_X_L 0x02   ///< Register address for X axis lower byte
#define MSA311_REG_OUT_X_H 0x03   ///< Register address for X axis higher byte
#define MSA311_REG_OUT_Y_L 0x04   ///< Register address for Y axis lower byte
#define MSA311_REG_OUT_Y_H 0x05   ///< Register address for Y axis higher byte
#define MSA311_REG_OUT_Z_L 0x06   ///< Register address for Z axis lower byte
#define MSA311_REG_OUT_Z_H 0x07   ///< Register address for Z axis higher byte
#define MSA311_REG_MOTIONINT 0x09 ///< Register address for motion interrupt
#define MSA311_REG_DATAINT 0x0A   ///< Register address for data interrupt
#define MSA311_REG_CLICKSTATUS                                                 \
  0x0B ///< Register address for click/doubleclick status
#define MSA311_REG_RESRANGE 0x0F  ///< Register address for resolution range
#define MSA311_REG_ODR 0x10       ///< Register address for data rate setting
#define MSA311_REG_POWERMODE 0x11 ///< Register address for power mode setting
#define MSA311_REG_INTSET0 0x16   ///< Register address for interrupt setting #0
#define MSA311_REG_INTSET1 0x17   ///< Register address for interrupt setting #1
#define MSA311_REG_INTMAP0 0x19   ///< Register address for interrupt map #0
#define MSA311_REG_INTMAP1 0x1A   ///< Register address for interrupt map #1
#define MSA311_REG_TAPDUR 0x2A    ///< Register address for tap duration
#define MSA311_REG_TAPTH 0x2B     ///< Register address for tap threshold

using namespace daisy;

class MSA311 {
private:
    I2CHandle i2c;
    int16_t X;
    int16_t Y;
    int16_t Z;
    uint8_t buf[2];

public:
    MSA311() {};

    /**
     * @brief Initialize the device to default settings
     * @return boolean if initialization was successful or not
     */
    bool Init();

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
