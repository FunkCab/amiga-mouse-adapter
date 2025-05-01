/*
 * Nunchuck functions  -- Talk to a Wii Nunchuck
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Nunchuck reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */
#include <C:\projects\amiga-mouse-adapter\NCHUCK_CODE\NewWiiChk.h>
#include <Wire.h>
#include <string.h>
#include <utility\twi.h>
#undef int
#include <stdio.h>


void NewWiiChk::init(){
//Serial.println("Start setup c");
	Wiicnt = 0;

	init_wireless(0);
}

void NewWiiChk::init_wireless(unsigned short timeout){
	static bool WiiInit = false;
	if (WiiInit == true) return; // send the initilization handshake
	WiiInit = true;
	byte rc = 1;
	int Wiicnt=0;
	int Light = LOW;
	pinMode(13, OUTPUT);
	unsigned long time = millis();
	Wire.begin(); // initialize i2c
	do{
		Wire.beginTransmission ((byte)0x52);	// transmit to device 0x52
		Wire.write (0xF0);	 // sends memory address
		Wire.write (0x55);	 // sends data.
		if(Wire.endTransmission() == 0){ // stop transmitting
			Wire.beginTransmission ((byte)0x52);	// transmit to device 0x52
			Wire.write (0xFB);	 // sends memory address
			Wire.write (0x00);	 // sends sent a zero.
			if(Wire.endTransmission () == 0){	// stop transmitting
			
				rc = 0;
			}
		}
		Wiicnt++;
		digitalWrite(13,((Wiicnt < 1000) ? HIGH : LOW)); 
		if (Wiicnt >= 2500) Wiicnt = 0;
	}while (rc != 0 && (!timeout || ((millis() - time) < timeout)));
	digitalWrite(13,LOW);
}

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it


void NewWiiChk::clearTwiInputBuffer(void)
{
// clear the receive buffer from any partial data
while( Wire.available ())
	Wire.read ();
}
// Send a request for data to the nunchuck
// was "send_zero()"
void NewWiiChk::send_request(){
	for(byte i = 0; i < 3; i++){
			Wire.beginTransmission(0x52);// transmit to device 0x52
			Wire.write((uint8_t)0x00);// sends one byte
			Wire.endTransmission();// stop transmitting
	}
}
// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char NewWiiChk::nunchuk_decode_byte (char x)
{
    //x = (x ^ 0x17) + 0x17;
    return x;
}

// Receive data back from the nunchuck, 
// returns 1 on successful read. returns 0 on failure
int NewWiiChk::get_data()
{
    int Wiicnt=0;
	//static uint8_t buf[6];
    Wire.requestFrom (0x52, 6);// request data from nunchuck
    while (Wire.available ()) {
        // receive byte as an integer
        buf[Wiicnt] = nunchuk_decode_byte( Wire.read() );
        Wiicnt++;
    }
    NewWiiChk::send_request();  // send request for next data payload
    // If we recieved the 6 bytes, then go print them

    if (Wiicnt >= 5) {
	    Wii.x = buf[0];
		Wii.y = buf[1];
		Wii.xa = buf[2]; // * 2 * 2; 
		Wii.ya = buf[3]; // * 2 * 2;
		Wii.za = buf[4]; // * 2 * 2;

		Wii.z = ((buf[5] >> 0) & 1) ? 0 : 1;
		Wii.c = ((buf[5] >> 1) & 1) ? 0 : 1;

        return 1;   // success
    }
    return 0; //failure
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void NewWiiChk::print_data()
{ 
    static int i=0;

    Serial.print(i,DEC);
    Serial.print("\t");

    Serial.print("joy:");
    Serial.print(joyx(), DEC);
    Serial.print(",");
    Serial.print(joyy(), DEC);
    Serial.print("  \t");

    Serial.print("acc:");
    Serial.print(accelx(), DEC);
    Serial.print(",");
    Serial.print(accely(), DEC);
    Serial.print(",");
    Serial.print(accelz(), DEC);
    Serial.print("\t\t");

    Serial.print("but:");
    Serial.print(zbutton(), DEC);
    Serial.print(",");
    Serial.print(cbutton(), DEC);
	Serial.print("\t");

    Serial.print("\r\n");  // newline
    i++;
}

// returns zbutton state: 1=pressed, 0=notpressed
int NewWiiChk::zbutton()
{
    return Wii.z;  // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
int NewWiiChk::cbutton()
{
    return Wii.c;  // voodoo
}

// returns value of x-axis joystick
int NewWiiChk::joyx()
{
    return Wii.x; 
}

// returns value of y-axis joystick
int NewWiiChk::joyy()
{
    return Wii.y;
}

// returns value of x-axis accelerometer
int NewWiiChk::accelx()
{
    return Wii.xa;   // FIXME: this leaves out 2-bits of the data
}

// returns value of y-axis accelerometer
int NewWiiChk::accely()
{
    return Wii.ya;   // FIXME: this leaves out 2-bits of the data
}

// returns value of z-axis accelerometer
int NewWiiChk::accelz()
{
    return Wii.za;   // FIXME: this leaves out 2-bits of the data
}
