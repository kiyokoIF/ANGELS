#include "daisy_seed.h"
#include "daisysp.h"
#include "periph/msa311.h"
#include "periph/cap1188.h"
#include "periph/sgtl5000.h"

#define ADC_CHANNELS 	6	// number of ADC Channels needed 

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '#' : '.'), \
  ((byte) & 0x40 ? '#' : '.'), \
  ((byte) & 0x20 ? '#' : '.'), \
  ((byte) & 0x10 ? '#' : '.'), \
  ((byte) & 0x08 ? '#' : '.'), \
  ((byte) & 0x04 ? '#' : '.'), \
  ((byte) & 0x02 ? '#' : '.'), \
  ((byte) & 0x01 ? '#' : '.') 

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

MSA311 *accel;

CAP1188 *cap;

SGTL5000 *codec;

I2CHandle i2c;

SaiHandle sai_2_handle_;

Oscillator osc;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = osc.Process();
		out[1][i] = out[0][i];
	}
}

int main(void)
{
	hw.Init();

	// Adding secondary SAI i2s output
	SaiHandle::Config sai2Config;
	sai2Config.periph          = SaiHandle::Config::Peripheral::SAI_2;
    sai2Config.sr              = SaiHandle::Config::SampleRate::SAI_48KHZ;
    sai2Config.bit_depth       = SaiHandle::Config::BitDepth::SAI_24BIT;
    sai2Config.a_sync          = SaiHandle::Config::Sync::SLAVE;
    sai2Config.b_sync          = SaiHandle::Config::Sync::MASTER;
    sai2Config.a_dir           = SaiHandle::Config::Direction::TRANSMIT;
    sai2Config.b_dir           = SaiHandle::Config::Direction::RECEIVE;
    sai2Config.pin_config.fs   = seed::D27;
    sai2Config.pin_config.mclk = seed::D24;
    sai2Config.pin_config.sck  = seed::D28;
    sai2Config.pin_config.sb   = seed::D25;
    sai2Config.pin_config.sa   = seed::D26;

	sai_2_handle_.Init(sai2Config);

	// Setting Audio to work with i2s out
    AudioHandle::Config audio_config;
    audio_config.blocksize  = 48;
    audio_config.samplerate = SaiHandle::Config::SampleRate::SAI_48KHZ;
    audio_config.postgain   = 1.f;

	// hw.audio_handle.DeInit(); will deinit sai1 as well which is bad. I don't think reinitializing is bad
    hw.audio_handle.Init(audio_config, hw.AudioSaiHandle(), sai_2_handle_);

	// reupdating those things
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	osc.Init(hw.AudioSampleRate());

	// Setting up ADCs
	AdcChannelConfig adc_config[ADC_CHANNELS];
	
	adc_config[0].InitSingle(seed::A0);
	adc_config[1].InitSingle(seed::A1);
	adc_config[2].InitSingle(seed::A2);
	adc_config[3].InitSingle(seed::A3);
	adc_config[4].InitSingle(seed::A4);
	adc_config[5].InitSingle(seed::A5);

	// Initialize the ADC peripheral with that configuration
	hw.adc.Init(adc_config, ADC_CHANNELS);

	// Start the ADC
	hw.adc.Start();	

	// Serial Output
	hw.StartLog();
	System::Delay(500);
	hw.PrintLine("Serial Out Working. SAI1: %d SAI2: %d", hw.AudioSaiHandle().IsInitialized(), sai_2_handle_.IsInitialized());

	// Setting up I2C comms for peripherals
	I2CHandle::Config i2c_conf;

	i2c_conf.periph 		= I2CHandle::Config::Peripheral::I2C_1;
    i2c_conf.speed 			= I2CHandle::Config::Speed::I2C_400KHZ;
    i2c_conf.mode 			= I2CHandle::Config::Mode::I2C_MASTER;
    i2c_conf.pin_config.scl = seed::D11;
    i2c_conf.pin_config.sda = seed::D12;

    i2c.Init(i2c_conf);

	System::Delay(500);

	accel = new MSA311();

	if (!accel->Init(&i2c)) {
		hw.PrintLine("MSA311 not found");
		while(1){}
	} else {
		hw.PrintLine("MSA311 found");
	}

	System::Delay(500);

	cap = new CAP1188();

	if (!cap->Init(&i2c)) {
		hw.PrintLine("CAP1188 not found");
		while(1){}
	} else {
		hw.PrintLine("CAP1188 found");
	}

	System::Delay(500);

	codec = new SGTL5000();

	hw.PrintLine("%d", codec->Init(&i2c));
	while(1){}

	if (!codec->Init(&i2c)) {
		hw.PrintLine("%d", codec->Init(&i2c));
		while(1){}
	} else {
		hw.PrintLine("SGTL5000 found");
	}

	// Start Audio
	hw.StartAudio(AudioCallback);
	
	while(1) {

		for (int i = 0; i < ADC_CHANNELS; i++) {
			int value = hw.adc.Get(i);

			float percent = (float) value / 65536.0f;

			hw.Print("ADC%d: %.3f  ", i, percent);
		}

		accel->Update();
		cap->Update();

		hw.PrintLine("X: %5d, Y: %5d, Z:%5d, CAP:"BYTE_TO_BINARY_PATTERN, accel->getX(), accel->getY(), accel->getZ(), BYTE_TO_BINARY(cap->getSens()));

		System::Delay(100);
	}
}
