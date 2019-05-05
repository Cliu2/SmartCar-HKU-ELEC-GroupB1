/*
	util.h - Library for basic control for the ELEC 3848 group project car.
	This libary also contains functions for sensors that will be used in this project.
	Created by Lance, Zhang, Liu, Mar 11, 2019.
*/

#ifndef IDP_Car_h
#define IDP_Car_h

#include "Arduino.h"
#include <Servo.h>

class Car{
	const int thre1 = 90; // qti threshold 1: white
	const int thre2 = 300; // qti threshold 2
	const int thre3 = 650; // qti threshold 3: black

	public:
		Car(String ID);
		void moveForward(int speed); //speed indicates how fast the car should move, can be negative, which means move backward
		void turn(int speed);//speed indicates how fast to spin the car, positive for turn right(clockwise), negative for turn left(anti-clockwise)
		void turnLeft90();
		void turnRight90();
		int getUltraSonicReading();
		int getQTIReading(int pin);
		bool followLine(int l_qti, int m_qti, int r_qti);
		void getAllQTIReadings(int &l_qti,int &m_qti, int &r_qti);
		void goOneBlock();
		void moveWithTurn(int spd, int turnspd);


		//MORE TO BE DONE

	private:
		String _ID;
		String _WIFItopic;
		int _leftServoBase=1527;
		int _rightServoBase=1530;
		Servo left,right;


		//define the pins
		int _leftServoPin=9;
		int _rightServoPin=10;
		int _leftQTIPin=A2;
		int _rightQTIPin=A0;
		int _middleQTIPin=A1;
		int _ultraSonicPin=11;


		//MORE PINS TO BE DONE
};


#endif