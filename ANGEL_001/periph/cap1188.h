#pragma once
#ifndef CAP1188_H
#define CAP1188_H

#include <daisy_seed.h>

#define CAP1188_I2CADDR 0x29 ///< The default I2C address

// Some registers we use
#define CAP1188_SENINPUTSTATUS                                                 \
  0x3 ///< The Sensor Input Status Register stores status bits that indicate a
      ///< touch has been detected. A value of ‘0’ in any bit indicates that no
      ///< touch has been detected. A value of ‘1’ in any bit indicates that a
      ///< touch has been detected.
#define CAP1188_MTBLK                                                          \
  0x2A ///< Multiple Touch Configuration register controls the settings for the
       ///< multiple touch detection circuitry. These settings determine the
       ///< number of simultaneous buttons that may be pressed before additional
       ///< buttons are blocked and the MULT status bit is set. [0/1]
#define CAP1188_LEDLINK                                                        \
  0x72 ///< Sensor Input LED Linking. Controls linking of sensor inputs to LED
       ///< channels
#define CAP1188_PRODID                                                         \
  0xFD ///< Product ID. Stores a fixed value that identifies each product.
#define CAP1188_MANUID                                                         \
  0xFE ///< Manufacturer ID. Stores a fixed value that identifies SMSC
#define CAP1188_STANDBYCFG                                                     \
  0x41 ///< Standby Configuration. Controls averaging and cycle time while in
       ///< standby.
#define CAP1188_REV                                                            \
  0xFF ///< Revision register. Stores an 8-bit value that represents the part
       ///< revision.
#define CAP1188_MAIN                                                           \
  0x00 ///< Main Control register. Controls the primary power state of the
       ///< device.
#define CAP1188_MAIN_INT                                                       \
  0x01 ///< Main Control Int register. Indicates that there is an interrupt.
#define CAP1188_LEDPOL                                                         \
  0x73 ///< LED Polarity. Controls the output polarity of LEDs.
#define CAP1188_INT_EN                                                         \
  0x27 ///< Enables/disables Interupts for Sensors
#define GAIN                                                                   \
  0b00 ///< Sets Gain (00 default)


#define TIMEOUT 1000

using namespace daisy;

class CAP1188 {
private:
    I2CHandle *i2c;
    uint8_t sens;

    void Read(uint16_t REG, uint16_t size);

    uint8_t buf[2];

public:
    CAP1188() {}

    bool Init(I2CHandle *_i2c);

    void Update();

    uint8_t getSens() { return sens; };

};

#endif