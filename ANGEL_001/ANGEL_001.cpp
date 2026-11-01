#include "daisy_seed.h"
#include "daisysp.h"
// #include "periph/msa311.h"
// #include "periph/cap1188.h"

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

Oscillator osc;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		float sig = osc.Process();
		out[0][i] = sig;
		out[1][i] = sig;
	}
}

int main(void)
{
	hw.Init();

	GPIO led;
	GPIO codec_deemp;
	GPIO amp_sd;

	led.Init(Pin(PORTC, 7), GPIO::Mode::OUTPUT);
	codec_deemp.Init(Pin(PORTC, 13), GPIO::Mode::OUTPUT);
	amp_sd.Init(Pin(PORTC, 15), GPIO::Mode::OUTPUT);

	hw.SetAudioBlockSize(4);
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

	osc.Init(hw.AudioSampleRate());
	osc.SetFreq(50);
	osc.SetAmp(1);
	osc.SetWaveform(osc.WAVE_SIN); 

	codec_deemp.Write(false);

	amp_sd.Write(false);

	hw.StartAudio(AudioCallback);

	while (1) {
		led.Write(true);
		System::Delay(500);
		led.Write(false);
		System::Delay(500);
	}

	// // Setting up ADCs
	// AdcChannelConfig adc_config[ADC_CHANNELS];
	
	// adc_config[0].InitSingle(seed::A0);
	// adc_config[1].InitSingle(seed::A1);
	// adc_config[2].InitSingle(seed::A2);
	// adc_config[3].InitSingle(seed::A3);
	// adc_config[4].InitSingle(seed::A4);
	// adc_config[5].InitSingle(seed::A5);

	// // Initialize the ADC peripheral with that configuration
	// hw.adc.Init(adc_config, ADC_CHANNELS);

	// // Start the ADC
	// hw.adc.Start();	

	// // Serial Output
	// hw.StartLog();
	// System::Delay(500);
	// hw.PrintLine("Serial Out Working. SAI1: %d SAI2: %d", hw.AudioSaiHandle().IsInitialized(), sai_2_handle_.IsInitialized());

	// // Setting up I2C comms for peripherals
	// I2CHandle::Config i2c_conf;

	// i2c_conf.periph 		= I2CHandle::Config::Peripheral::I2C_1;
    // i2c_conf.speed 			= I2CHandle::Config::Speed::I2C_400KHZ;
    // i2c_conf.mode 			= I2CHandle::Config::Mode::I2C_MASTER;
    // i2c_conf.pin_config.scl = seed::D11;
    // i2c_conf.pin_config.sda = seed::D12;

    // i2c.Init(i2c_conf);

	// System::Delay(500);

	// accel = new MSA311();

	// if (!accel->Init(&i2c)) {
	// 	hw.PrintLine("MSA311 not found");
	// 	while(1){}
	// } else {
	// 	hw.PrintLine("MSA311 found");
	// }

	// System::Delay(500);

	// cap = new CAP1188(); 

	// if (!cap->Init(&i2c)) {
	// 	hw.PrintLine("CAP1188 not found");
	// 	while(1){}
	// } else {
	// 	hw.PrintLine("CAP1188 found");
	// }

	// System::Delay(500);

	// // Start Audio
	
	
	// while(1) {

	// 	for (int i = 0; i < ADC_CHANNELS; i++) {
	// 		int value = hw.adc.Get(i);

	// 		float percent = (float) value / 65536.0f;

	// 		hw.Print("ADC%d: %.3f  ", i, percent);
	// 	}

	// 	accel->Update();
	// 	cap->Update();

	// 	hw.PrintLine("X: %5d, Y: %5d, Z:%5d, CAP:" BYTE_TO_BINARY_PATTERN, accel->getX(), accel->getY(), accel->getZ(), BYTE_TO_BINARY(cap->getSens()));

	// 	System::Delay(100);
	// }
}
