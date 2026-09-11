#pragma once
#ifndef SGTL5000_H
#define SGTL5000_H

#include <daisy_seed.h>

#define         SGTL5000_I2C_ADDR                           0x2A

#define			SGTL5000_CHIP_ID							0x0000
#define			SGTL5000_CHIP_DIG_POWER						0x0002
#define			SGTL5000_CHIP_CLK_CTRL						0x0004
#define			SGTL5000_CHIP_I2S_CTRL						0x0006
#define			SGTL5000_CHIP_SSS_CTRL						0x000A
#define			SGTL5000_CHIP_ADCDAC_CTRL					0x000E
#define			SGTL5000_CHIP_DAC_VOL						0x0010
#define			SGTL5000_CHIP_PAD_STRENGTH					0x0014
#define			SGTL5000_CHIP_ANA_ADC_CTRL					0x0020
#define			SGTL5000_CHIP_ANA_HP_CTRL					0x0022
#define			SGTL5000_CHIP_ANA_CTRL						0x0024
#define			SGTL5000_CHIP_LINREG_CTRL					0x0026
#define			SGTL5000_CHIP_REF_CTRL						0x0028
#define			SGTL5000_CHIP_MIC_CTRL						0x002A
#define			SGTL5000_CHIP_LINE_OUT_CTRL					0x002C
#define			SGTL5000_CHIP_LINE_OUT_VOL					0x002E
#define			SGTL5000_CHIP_ANA_POWER						0x0030
#define			SGTL5000_CHIP_PLL_CTRL						0x0032
#define			SGTL5000_CHIP_CLK_TOP_CTRL					0x0034
#define			SGTL5000_SHIP_ANA_STATUS					0x0036
#define			SGTL5000_CHIP_ANA_TEST1						0x0038
#define			SGTL5000_CHIP_ANA_TEST2						0x003A
#define			SGTL5000_CHIP_SHORT_CTRL					0x003C
#define			SGTL5000_DAP_CONTROL						0x0100
#define			SGTL5000_DAP_PEQ							0x0102
#define			SGTL5000_DAP_BASS_ENHANCE					0x0104
#define			SGTL5000_DAP_BASS_ENHANCE_CTRL				0x0106
#define			SGTL5000_DAP_AUDIO_EQ						0x0108
#define			SGTL5000_DAP_SGTL_SURROUND					0x010A
#define			SGTL5000_DAP_FILTER_COEF_ACCESS				0x010C
#define			SGTL5000_DAP_COEF_WR_B0_MSB					0x010E
#define			SGTL5000_DAP_COEF_WR_B0_LSB					0x0110
#define			SGTL5000_DAP_AUDIO_EQ_BASS_BAND0			0x0116
#define			SGTL5000_DAP_AUDIO_EQ_BAND1					0x0118
#define			SGTL5000_DAP_AUDIO_EQ_BAND2					0x011A
#define			SGTL5000_DAP_AUDIO_EQ_BAND3					0x011C
#define			SGTL5000_DAP_AUDIO_EQ_TREBLE_BAND4			0x011E
#define			SGTL5000_DAP_MAIN_CHAN						0x0120
#define			SGTL5000_DAP_MIX_CHAN						0x0122
#define			SGTL5000_DAP_AVC_CTRL						0x0124
#define			SGTL5000_DAP_AVC_THRESHOLD					0x0126
#define			SGTL5000_DAP_AVC_ATTACK						0x0128
#define			SGTL5000_DAP_AVC_DECAY						0x012A

#define			SGTL5000_DAP_COEF_WR_B1_MSB					0x012C
#define			SGTL5000_DAP_COEF_WR_B1_LSB					0x012E
#define			SGTL5000_DAP_COEF_WR_B2_MSB					0x0130
#define			SGTL5000_DAP_COEF_WR_B2_LSB					0x0132
#define			SGTL5000_DAP_COEF_WR_A1_MSB					0x0134
#define			SGTL5000_DAP_COEF_WR_A1_LSB					0x0136
#define			SGTL5000_DAP_COEF_WR_A2_MSB					0x0138
#define			SGTL5000_DAP_COEF_WR_A2_LSB					0x013A

#define TIMEOUT 1000

class SGTL5000 {
private:
    I2CHandle *i2c;
    uint8_t buf[4];

    void Read(uint16_t REG) {
        buf[0] = REG >> 8;
        buf[1] = REG & 0xFF;
        i2c->TransmitBlocking(SGTL5000_I2C_ADDR, buf, 2, TIMEOUT);
        i2c->ReceiveBlocking(SGTL5000_I2C_ADDR, buf, 2, TIMEOUT);
    }

    void Write(uint16_t REG, uint16_t DAT) {
        buf[0] = REG >> 8;
        buf[1] = REG & 0xFF;
        buf[2] = DAT >> 8;
        buf[3] = DAT & 0xFF;
        i2c->TransmitBlocking(SGTL5000_I2C_ADDR, buf, 4, TIMEOUT);
    }

public:
    SGTL5000(){};

    uint8_t Init(I2CHandle *_i2c) {
        i2c = _i2c;

        Read(SGTL5000_CHIP_ID);
        if (buf[0] != 0xA0) return buf[0];

        // Enable DAC and i2s in
        Write(SGTL5000_CHIP_DIG_POWER, 0x0021);     // 00000000 00 1 0 00 0 1
        System::Delay(4);

        // Sets Sys_Clk to 48kHz and MCLK to 256 * Sys_Clk
        Write(SGTL5000_CHIP_CLK_CTRL, 0x0008);      // 00000000 00 00 10 00
        System::Delay(4);

        // Sets SCLKFREQ, Sets MASTER, Sets Clock Edge Rising, sets DLEN 24, sets i2s mode left justified, sets LRalign, sets Left 0 right 1
        Write(SGTL5000_CHIP_I2S_CTRL, 0x00D2);      // 0000000 0 1 1 01 00 1 0
        System::Delay(4);

        // Set DAC source to i2s in
        Write(SGTL5000_CHIP_SSS_CTRL, 0x0010);
        System::Delay(4);

        // Sets soft ramp + DAC unmute
        Write(SGTL5000_CHIP_ADCDAC_CTRL, 0x0200);
        System::Delay(4);

        // Sets DAC Volume
        Write(SGTL5000_CHIP_DAC_VOL, 0x3C3C);
        System::Delay(4);

        // Sets Power of i2s i2c
        Write(SGTL5000_CHIP_PAD_STRENGTH, 0x02AF);
        System::Delay(4);

        // Sets Headphone Vol
        Write(SGTL5000_CHIP_ANA_HP_CTRL, 0x1818);
        System::Delay(4);

        // Unmutes headphone and sets output from DAC
        Write(SGTL5000_CHIP_ANA_CTRL, 0x0101);
        System::Delay(4);

        // Powers up DAC, Headphones, lin regulator
        Write(SGTL5000_CHIP_ANA_POWER, 0x7038);     // 0b0111 0000 0011 1000
        System::Delay(4);

        return true;
    }
};

#endif