/*Assume the map is 6*6 */

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#include "Car.h"

Car* car;
int x=0,y=0;
int drc=RIGHT;

int targetX=4;
int targetY=5;


void setup() {
  Serial.begin(9600);
  car=new Car("Car");
  car->moveForward(0);
  delay(3000);
//  car->goOneBlock();

}

void loop() {
  if(x==targetX && y==targetY){
    car->moveForward(0);
    while(true);    
  }
  
  int obstacleDistance=car->getUltraSonicReading();
  int nx,ny,newdrc;
  int dist_l=9999,dist_c=9999,dist_r=9999;
  if(obstacleDistance>15){
    calculateNextPosition(drc,nx,ny);
    if(checkBoundary(nx,ny)){
      dist_c=abs(targetX-nx)+abs(targetY-ny);
    }
  }
  //turn left
  newdrc=(drc-1+4)%4;
  calculateNextPosition(newdrc,nx,ny);
  if(checkBoundary(nx,ny)){
      dist_l=abs(targetX-nx)+abs(targetY-ny);
  }
  //turn right
  newdrc=(drc+1+4)%4;
  calculateNextPosition(newdrc,nx,ny);
  if(checkBoundary(nx,ny)){
      dist_r=abs(targetX-nx)+abs(targetY-ny);
  }

  int dist_min=min(dist_l,min(dist_c,dist_r));
  if(dist_c==dist_min){
    drc=drc;
  }else if(dist_l==dist_min){
    car->turnLeft90();
    drc=(drc-1+4)%4;
  }else if(dist_r==dist_min){
    car->turnRight90();
    drc=(drc+1+4)%4;
  }
//
//  Serial.println(drc);
//
//  
  int nextx,nexty;
  calculateNextPosition(drc,nextx,nexty);
  car->goOneBlock();
  x=nextx;
  y=nexty;
  Serial.print(x);
  Serial.print(',');
  Serial.println(y);

  delay(10);
  
}


void selectDirection(int targetDirection){
  Serial.println(targetDirection);
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

void calculateNextPosition(int drc, int &nextx, int &nexty){
  if(drc==UP){
    nextx=x;
    nexty=y+1;
  }else if(drc==DOWN){
    nextx=x;
    nexty=y-1;
  }else if(drc==LEFT){
    nextx=x-1;
    nexty=y;
  }else if(drc==RIGHT){
    nextx=x+1;
    nexty=y;
  }
  return;
}

bool checkBoundary(int x, int y){
  if(x>=0 && x<=5 && y>=0 && y<=5){
    return true;
  }else{
    return false;
  }
}
