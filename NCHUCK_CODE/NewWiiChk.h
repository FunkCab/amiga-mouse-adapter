#ifndef NewWiiChk_h
#define NewWiiChk_h

#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <utility\twi.h>
#undef int
#include <stdio.h>


class NewWiiChk {
	public:
		typedef struct WiiVals
		{
		  int x, y, xa, ya, za, z, c;
		} WiiVals;

		WiiVals Wii; // gyro angular velocity readings

		void init();
		int get_data();
		void print_data();
		int zbutton();
		int cbutton();
		int joyx();
		int joyy();
		int accelx();
		int accely();
		int accelz();

	private:
		uint8_t buf[6];   // array to store nunchuck data,
		int Wiicnt;
		void init_wireless(unsigned short timeout);
		void clearTwiInputBuffer(void);
		void send_request();
		char nunchuk_decode_byte (char x);
};

#endif