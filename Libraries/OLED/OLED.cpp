#include "Arduino.h"
// #include <Adafruit_SSD1331.h>
#include "OLED.h"

OLED::OLED(){
	setup();
}

void OLED::setup(){
	// Serial.begin(9600);
	display.begin();
	display.fillScreen(BLACK);
	display.setRotation(2); 
	
	tftPrintTest();
	delay(1000);
	display.fillScreen(BACKGROUND);
	display.setCursor(0,0);
}

void OLED::tftPrintTest(){
	display.fillScreen(BLACK);
	display.setCursor(15, 5);
	display.setTextColor(WHITE);	
	display.setTextSize(2);
	display.println("HKUEEE");
	display.setCursor(0, 25);
	display.setTextColor(CYAN);
	display.setTextSize(2);
	display.println("IDP 2019");
	display.setCursor(15, 50);
	display.setTextColor(YELLOW);
	display.setTextSize(2);
	display.println("Group 1 Thu");
	// delay(1000);	
}

void OLED::displayText(String text, int color, int posX, int posY, int size){
	display.fillScreen(BLACK);
	// delay(50);
	// display.fillScreen(WHITE);
	// delay(250);
	// display.fillScreen(BLACK);
	display.setTextColor(color);	
	display.setTextSize(size);
	display.setCursor(posX, posY);
	display.println(text);
	// delay(1500);
}
