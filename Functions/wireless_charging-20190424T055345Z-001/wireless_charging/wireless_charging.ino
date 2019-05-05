#include <Adafruit_SSD1331.h>
#include <Servo.h>

Servo left, right; 

#define sclk 52 //--- connect this to the display module CLK pin (Serial Clock)

#define mosi 51 //--- connect this to the display module DIN pin (Serial Data)

#define rst  49 //--- connect this to the display module RES pin (Reset)

#define dc   48 //--- connect this to the display module D/C  pin (Data or Command)

#define cs   53 //--- connect this to the display module CS  pin (Chip Select)

#define analogInput     A15

#define _ultraSonicPin  A3

// Color definitions

#define  BLACK           0x0000

#define BLUE            0x0006

#define RED             0xF800

#define GREEN           0x07E0

#define CYAN            0x07FF

#define MAGENTA         0xF81F

#define YELLOW          0xFFE0  

#define WHITE           0xFFFF

#define BACKGROUND      0x0000



Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

int value =0;
float vout = 0.0;
float vin = 0.0;
double pos = 0; 

int l=1533;
int r=1525;

int getUltraSonicReading(){
  long duration, distance_in_cm;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(_ultraSonicPin, OUTPUT);
  linefollow(0);
  digitalWrite(_ultraSonicPin, LOW); 
  linefollow(0);
  delayMicroseconds(2); 
  linefollow(0);
  digitalWrite(_ultraSonicPin, HIGH);
  linefollow(0);
  delayMicroseconds(10); 
  linefollow(0);
  digitalWrite(_ultraSonicPin, LOW);
  linefollow(0);
  pinMode(_ultraSonicPin, INPUT);
  linefollow(0);
  digitalWrite(_ultraSonicPin, HIGH);
  linefollow(0);
  duration = pulseIn(_ultraSonicPin, HIGH);
  linefollow(0);
  distance_in_cm = (duration/2) / 29.1;
  linefollow(0);
  delay (100);
  return (int) distance_in_cm;
} 


void forward(){
  left.writeMicroseconds(l+30);
  right.writeMicroseconds(r-30);
}

void stay(){
  left.writeMicroseconds(l);
  right.writeMicroseconds(r);
}

void linefollow(double pos){
  // difference in left/right wheel speed is proportional to
  // the deviation of the line position
  double lpos=l+31;
  int rpos=r-30; 
  left.writeMicroseconds(lpos);
  right.writeMicroseconds(rpos);
}

void setup()
{
  delay(2000);
  Serial.begin(9600);
  display.begin();
  display.fillScreen(BLACK);
  display.fillScreen(BACKGROUND);
  display.setCursor(0,0);
  pinMode(9, OUTPUT); // left servo
  pinMode(10, OUTPUT); // right servo
  left.attach(9);
  right.attach(10);
  stay();
}


void loop() {

  // read the value at analog input
  value = analogRead(analogInput);
  vin = (value * 5.0) / 1024.0; // see text
  Serial.println (getUltraSonicReading());
  

  if( vin > 3){
  delay (100);
  value = analogRead(analogInput);
  vin = (value * 5.0) / 1024.0;
  stay();
  display.fillScreen(BLACK);
  display.setCursor(0, 1);
  display.setTextColor(WHITE);  
  display.setTextSize(2);  
  display.println("INPUT V= ");
  display.setCursor(0, 25);
  display.print(vin);
  while (true)
  {  }
  }    
  else if (getUltraSonicReading() < 3+)
  {
  stay();
  delay (1000);
  do
  {
  left.writeMicroseconds(l+20);
  right.writeMicroseconds(r+20);
  value = analogRead(analogInput);
  vin = (value * 5.0) / 1024.0;
  }while (vin < 3);
  stay(); 
  display.fillScreen(BLACK);
  display.setCursor(0, 1);
  display.setTextColor(WHITE);  
  display.setTextSize(2);  
  display.println("INPUT V= ");
  display.setCursor(0, 25);
  display.print(vin);
  while (true)
  {
  }
  
 }
 else
 linefollow(0);

 

}



