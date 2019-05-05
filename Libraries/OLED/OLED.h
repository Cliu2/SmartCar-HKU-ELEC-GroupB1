#include "Arduino.h"
#include <Adafruit_SSD1331.h>

#ifndef IDP_OLED_h
#define IDP_OLED_h

class OLED{
	//colors

	public:
		const int BLACK=0x0000;
		const int BLUE=0x0006;
		const int RED=0xF800;
		const int GREEN=0x07E0;
		const int CYAN=0x07FF;
		const int MAGENTA=0xF81F;
		const int YELLOW=0xFFE0;
		const int WHITE=0xFFFF;
		const int BACKGROUND=0x0000;
		int sclk=52;
		int mosi=51;
		int rst=49;
		int dc=48;
		int cs=53;
		OLED();
		void displayText(String text, int color, int posX=5, int posY=5, int size=2);



	private:
		Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);		

		void setup();
		void tftPrintTest();

};


#endif