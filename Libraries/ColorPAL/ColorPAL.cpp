#include "ColorPAL.h"
#include <SoftwareSerial.h>


ColorPAL::ColorPAL(){
	setup();
}

void ColorPAL::setup(){
	reset();				  // Send reset to ColorPal
	Serial2.begin(sioBaud);
	Serial2.print("= (00 $ m) !"); // Loop print values, see ColorPAL documentation
	// serout->end();			  // Discontinue serial port for transmitting
	// serin->begin(sioBaud);	        // Set up serial port for receiving
	// pinMode(sio, INPUT);
}

void ColorPAL::reset(){
	//Use the TX of Serial2, pin 16 to initiate the colorPAL
	delay(200);
	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
	pinMode(16, INPUT);
	while (digitalRead(16) != HIGH);
	pinMode(16, OUTPUT);
	digitalWrite(16, LOW);
	delay(80);
	pinMode(16, INPUT);
	delay(waitDelay);
}

bool ColorPAL::readData(int &r, int &g, int &b){
	char buffer[32]="";
	if (Serial2.available() > 0) {
		// Wait for a $ character, then read three 3 digit hex numbers
		buffer[0] = Serial2.read();
		if (buffer[0] == '$') {
			for(int i = 0; i < 9; i++) {
				while (Serial2.available() == 0);		 // Wait for next input character
				buffer[i] = Serial2.read();				
				if (buffer[i] == '$')	             // Return early if $ character encountered
					return false;
			}
			parseData(buffer,r,g,b);
			return true;
			// delay(10);
		}
	}
	return false;
}

void ColorPAL::parseData(char * data, int &r, int &g, int& b){
	int red,green,blue;
	sscanf (data, "%3x%3x%3x", &red, &green, &blue);
	char buffer[32];
	sprintf(buffer, "R%4.4d G%4.4d B%4.4d", red, green, blue);
	// Serial.println(buffer);
	r=red;
	g=green;
	b=blue;

}

int ColorPAL::senseRed(){
	// setup();
	int red,green,blue;
	if(readData(red,green,blue)){
		if((red-green)>60 && (red-blue)>60 && red>50){
			// Serial.println("red");
			return 1;
		}else{
			return -1;
		}
	}
	return 0;
	
}