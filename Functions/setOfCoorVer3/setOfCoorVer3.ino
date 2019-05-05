/*Assume the map is 6*6 */

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#include "Car.h"

struct Coor
{
  int x;
  int y;
  bool visited;
};

Car* car;
int x, y;
int drc;
int led = 8;
int index;
Coor coor[5];
String coorStr[5] = {"1,5", "1,2", "2,1", "3,4", "3,2"}; //input the coordinates here, {"x1,y1","x2,y2", "x3,y3", ...}
//int arrSize = sizeof(coor) / sizeof(coor[0]);
int arrSize = 5;
String carName = "car2"; // input the name of the car here, "car1" or "car2"

void setup() {
  Serial.begin(9600);
//  while(!Serial.available()){
//    
//  }
//  carName = Serial.readStringUntil('\n');
//
//  for (int i = 0; i < 5; i++){
//    while(!Serial.available()){
//      
//    }
//    String input = Serial.readStringUntil('\n');
//    coor[i].x = int(input[0] - '0');
//    coor[i].y = int(input[2] - '0');
//    coor[i].visited = false;
//  }

  for (int i = 0; i < 5; i++){
    coor[i].x = int(coorStr[i][0] - '0');
    coor[i].y = int(coorStr[i][2] - '0');
    coor[i].visited = false;
  }
  
  Serial.print("car: ");
  Serial.println(carName);
  Serial.println("coordinates:");
  for (int i = 0; i < 5; i++){
    Serial.print("(");
    Serial.print(coor[i].x);
    Serial.print(",");
    Serial.print(coor[i].y);
    Serial.print(")");
    if (coor[i].visited)
      Serial.println(" visited");
    else
      Serial.println(" not visited");
  }
  
  car=new Car(carName);
  car->moveForward(0);
  pinMode(led, OUTPUT);

  if (carName == "car1"){
    index = 0;
    x = 0;
    y = 0;
    drc = RIGHT;
  }
  else{
    index = arrSize-1;
    x = 5;
    y = 5;
    drc = LEFT;
  }

  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  Serial.println(drc);
  delay(3000);
}

void loop() {
  while(coor[index].visited){
    if (carName == "car1")
      index++;

    else
      index--;
  }
  
  if (carName == "car1" && coor[index].x > 2){
      while(true)
        car->moveForward(0);
  }
  else if (carName == "car2" && coor[index].x < 3)
  {
    while(true)
      car->moveForward(0);
  }

  if(x==coor[index].x && y==coor[index].y && !coor[index].visited){
    car->moveForward(0);
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    delay(1000);
    coor[index].visited = true;
    
    if (carName == "car1")
      index++;

    else
      index--;
  }

  else if(x != coor[index].x){
    if(x < coor[index].x){
      selectDirection(RIGHT);
      car->goOneBlock();
      x++;
    }
    else{
      selectDirection(LEFT);
      car->goOneBlock();
      x--;
    }
  }

  else{
    if(y < coor[index].y){
      selectDirection(UP);
      car->goOneBlock();
      y++;
    }
    else{
      selectDirection(DOWN);
      car->goOneBlock();
      y--;
    }
  }

  if (carName == "car1"){
    for (int i = 0; coor[i].x < 3; i++){
      if (x == coor[i].x && y == coor[i].y && !coor[i].visited){
        coor[i].visited = true;
        car->moveForward(0);
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        delay(1000);
      }
    }

//    if (coor[index].x > 2)
//    {
//      while(true)
//        car->moveForward(0);
//    }
  }

  else{
    for (int i = arrSize - 1; coor[i].x > 2; i--){
      if (x == coor[i].x && y == coor[i].y && !coor[i].visited){
        coor[i].visited = true;
        car->moveForward(0);
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        delay(1000);
      }
    }

//    if (coor[index].x < 3)
//    {
//      while(true)
//        car->moveForward(0);
//    }
  }

//  if(coor[index].visited){
//    if (carName == "car1")
//      index++;
//
//    else
//      index--;
//  }

  delay(10);
}

void selectDirection(int targetDirection){
  int direcDifference=(targetDirection-drc+4)%4;
  if(direcDifference==0){
    return;
  }else if (direcDifference==1){
    car->turnRight90();
  }else if (direcDifference==2){
    car->turnRight90();
    car->turnRight90();
  }else if (direcDifference==3){
    car->turnLeft90();
  }
  drc=targetDirection;
  return;
}

