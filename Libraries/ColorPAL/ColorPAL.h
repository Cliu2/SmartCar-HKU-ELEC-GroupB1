#include <SoftwareSerial.h>
#include "Arduino.h"

#ifndef IDP_ColorPAL_h
#define IDP_ColorPAL_h

class ColorPAL{
	const int unused = 255; 		// Non-existant pin # for SoftwareSerial
	const int sioBaud = 4800;
	const int waitDelay = 200;

	public:
		ColorPAL();
		bool readData(int &red, int &green, int &blue);
		int senseRed();

	private:
		void setup();
		void reset();
		void parseData(char * data, int &red, int &green, int& blue);
};

#endif