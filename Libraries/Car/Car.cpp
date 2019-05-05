/* 
	util.cpp - Libary for implementing the car basic functionailities.
	Created by Lance, Mar 11, 2019
*/

#include "Arduino.h"
#include <Servo.h>
#include "Car.h"

Car::Car(String ID){
	_ID=ID;
	_WIFItopic="2BThu01Car/"+ID;
	pinMode(_leftQTIPin,INPUT);
	pinMode(_rightQTIPin,INPUT);
	pinMode(_middleQTIPin,INPUT);
	pinMode(_leftServoPin,OUTPUT);
	pinMode(_rightServoPin,OUTPUT);
	left.attach(_leftServoPin);
	right.attach(_rightServoPin);
	moveForward(0);	
}

void Car::moveForward(int speed){
	left.writeMicroseconds(_leftServoBase+speed);
	right.writeMicroseconds(_rightServoBase-speed);
}

void Car::turn(int speed){
	left.writeMicroseconds(_leftServoBase+speed);
	right.writeMicroseconds(_rightServoBase+speed);
}

void Car::turnLeft90(){
	//spin 90 degrees to left
	turn(-50);
	delay(700);
	while(getQTIReading(_middleQTIPin) < thre3){
		delay(1);
	}
	moveForward(0);
}

void Car::turnRight90(){
	// spin 90 degrees to right
	turn(50);
	delay(700);
	while(getQTIReading(_middleQTIPin) < thre3){
		delay(1);
	}
	moveForward(0);
}

int Car::getQTIReading(int pin){
	digitalWrite(pin, HIGH);
	return analogRead(pin);
}

void Car::getAllQTIReadings(int &l_qti,int &m_qti, int &r_qti){
	l_qti=getQTIReading(_leftQTIPin);
	m_qti=getQTIReading(_middleQTIPin);
	r_qti=getQTIReading(_rightQTIPin);
}


bool Car::followLine(int l_qti, int m_qti, int r_qti){
	double pos=0;
	if(m_qti < thre1 && r_qti < thre1 && l_qti > thre2){
		// the line is closest to the left QTI sensor
		// map the value of l_qti from 0 ~ 1023 to -1 ~ -0.5
		pos = l_qti/1023.0 /2.0 - 1;

		left.writeMicroseconds(_leftServoBase + 50 + (int)(pos*40));
		right.writeMicroseconds(_rightServoBase - 50 + (int)(pos*40));
		return true;
	}
	if(m_qti < thre1 && l_qti < thre1 && r_qti > thre2){
		// the line is closest to the right QTI sensor
		// map the value from 0 ~ 1023 to 0.5 ~ 1
		pos = 1 - r_qti/1023.0 /2.0;

		left.writeMicroseconds(_leftServoBase + 50 + (int)(pos*40));
		right.writeMicroseconds(_rightServoBase - 50 + (int)(pos*40));
		return true;
	}
	if(l_qti < thre1 && m_qti < thre1 && r_qti < thre1){
		// All QTI sensors cannot find the line
		return false;
	}

	// Otherwise, the line is between the left and right QTI sensors
	pos = (double)(r_qti - l_qti)/(l_qti + m_qti + r_qti);

	left.writeMicroseconds(_leftServoBase + 50 + (int)(pos*40));
	right.writeMicroseconds(_rightServoBase - 50 + (int)(pos*40));
	return true;
}

void Car::moveWithTurn(int spd, int turnspd){
	left.writeMicroseconds(_leftServoBase + spd + (int)(turnspd));
	right.writeMicroseconds(_rightServoBase - spd + (int)(turnspd));
}


int Car::getUltraSonicReading(){
	long duration, distance_in_cm;
	// The sensor is triggered by a HIGH pulse of 10 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(_ultraSonicPin, OUTPUT);
	digitalWrite(_ultraSonicPin, LOW); 
	delayMicroseconds(2); 
	digitalWrite(_ultraSonicPin, HIGH);
	delayMicroseconds(10); 
	digitalWrite(_ultraSonicPin, LOW);
	pinMode(_ultraSonicPin, INPUT);
	digitalWrite(_ultraSonicPin, HIGH);
	duration = pulseIn(_ultraSonicPin, HIGH);
	distance_in_cm = (duration/2) / 29.1;

	return (int)distance_in_cm;
} 

void Car::goOneBlock(){
	int count=0,total_count=0;
	int r_qti,l_qti,m_qti;

	bool status=false;
	while(true){
		
		getAllQTIReadings(l_qti,m_qti,r_qti);

		// if((!status) && l_qti > thre1 && r_qti > thre1){
		// 	followLine(l_qti,m_qti,r_qti);
		// 	continue;
		// }else if (!status){			
		// 	status=true;
		// 	Serial.println("status is true now");
		// 	delay(1000);
		// }

		if(l_qti > thre3 && m_qti > thre3 && r_qti > thre3){
			count++;
		}else{
			total_count = count;
			count = 0;
		}

		if(total_count > 10){
			Serial.println(total_count);
			delay(300);
			moveForward(0);
			return;
		}else{
			// follow the line otherwise
			followLine(l_qti,m_qti,r_qti);
		}
	}
}