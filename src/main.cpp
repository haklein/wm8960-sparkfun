#include "Arduino.h"

#include "I2S_Sidetone.hpp"
I2S_Sidetone sidetone;

#include <SparkFun_WM8960_Arduino_Library.h>
WM8960 codec;

void setup(){
	Serial.begin(115200);
	Wire.begin(CONFIG_WM8960_SDA, CONFIG_WM8960_SCL);
	if (codec.begin() == false) //Begin communication over I2C
	{
		Serial.println("The device did not respond. Please check wiring.");
		while (1); // Freeze
	}
	Serial.println("Device is connected properly.");
	// General setup needed
	codec.enableVREF();
	codec.enableVMID();

	// Connect from DAC outputs to output mixer
	codec.enableLD2LO();
	codec.enableRD2RO();

	// Enable output mixers
	codec.enableLOMIX();
	codec.enableROMIX();

	// CLOCK STUFF, These settings will get you 44.1KHz sample rate, and class-d
	// freq at 705.6kHz
	codec.enablePLL(); // Needed for class-d amp clock
	codec.setPLLPRESCALE(WM8960_PLLPRESCALE_DIV_2);
	codec.setSMD(WM8960_PLL_MODE_FRACTIONAL);
	codec.setCLKSEL(WM8960_CLKSEL_PLL);
	codec.setSYSCLKDIV(WM8960_SYSCLK_DIV_BY_2);
	codec.setBCLKDIV(4);
	codec.setDCLKDIV(WM8960_DCLKDIV_16);
	codec.setPLLN(7);
	codec.setPLLK(0x86, 0xC2, 0x26); // PLLK=86C226h
	codec.setWL(WM8960_WL_16BIT);

	codec.enablePeripheralMode(); 

	codec.enableDacLeft();
	codec.enableDacRight();

	codec.disableDacMute();   // Default is "soft mute" on, so we must disable mute to make channels active
				  //codec.enableLoopBack(); // Loopback sends ADC data directly into DAC
	codec.disableLoopBack();

	codec.enableSpeakers();

	codec.setDacLeftDigitalVolumeDB(20.0);
	codec.setDacRightDigitalVolumeDB(20.0);

	Serial.println("Volume set to +0dB");
	codec.setSpeakerVolumeDB(0.00);

	sidetone.begin(44100,16,2,128); //  defaults to 44100, 16, 2, 32
	sidetone.setFrequency(600.0);
	sidetone.setVolume(1.0);
}

void loop() {
	Serial.println("Beep on");
	sidetone.on();
	delay(1000);
	Serial.println("Beep off");
	sidetone.off();
	delay(1000);


}

