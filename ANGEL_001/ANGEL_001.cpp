#include "daisy_seed.h"
#include "daisysp.h"
#include "periph/msa311.h"

#define ADC_CHANNELS 	6	// number of ADC Channels needed 

using namespace daisy;
using namespace daisysp;

DaisySeed hw;

uint8_t buf[2];

MSA311 *accel;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i];
		out[1][i] = in[1][i];
	}
}

int main(void)
{
	hw.Init();
	// hw.SetAudioBlockSize(4); // number of samples handled per callback
	// hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	// hw.StartAudio(AudioCallback);

	AdcChannelConfig adc_config[ADC_CHANNELS];
	
	// Set up the ADC config with a connection to pin A0
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

	hw.StartLog();
	System::Delay(100);
	hw.PrintLine("I'm Working");

	accel = new MSA311();

	if (!accel->Init()) {
		hw.PrintLine("Fucked");
		while(1){}
	}

	// I2CHandle::Config i2c_conf;

	// i2c_conf.periph 		= I2CHandle::Config::Peripheral::I2C_1;
    // i2c_conf.speed 			= I2CHandle::Config::Speed::I2C_100KHZ;
    // i2c_conf.mode 			= I2CHandle::Config::Mode::I2C_MASTER;
    // i2c_conf.pin_config.scl = {DSY_GPIOB, 8};
    // i2c_conf.pin_config.sda = {DSY_GPIOB, 9};

	// I2CHandle i2c;
	

	
	while(1) {

		// if (i2c.ReadDataAtAddress(MSA311_I2CADDR_DEFAULT, 0x01, 1, &buf, 1, 1000) == I2CHandle::Result::OK) {
		// 	hw.Print("X axis: %d   ", buf);
		// } else {
		// 	hw.Print("I2C failed   ");
		// }

		// for (int i = 0; i < ADC_CHANNELS; i++) {
		// 	int value = hw.adc.Get(i);

		// 	float percent = (float) value / 65536.0f;

		// 	hw.Print("ADC %d Value: %f   ", i, percent);
		// }
		// buf[0] = MSA311_REG_OUT_X_L;
		// hw.Print("%d  ", buf[0]);

		// if (i2c.TransmitBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000) != I2CHandle::Result::OK) {
		// 	hw.PrintLine("brokie");
		// 	while(1){}
		// }

		// i2c.ReceiveBlocking(MSA311_I2CADDR_DEFAULT, buf, 1, 1000);
		// hw.Print("%d\n", buf[0]);
		accel->Update();

		hw.PrintLine("X: %d, Y: %d, Z:%d", accel->getX(), accel->getY(), accel->getZ());

		System::Delay(100);
	}
}
