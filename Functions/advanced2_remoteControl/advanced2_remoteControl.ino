#include "Car.h"
#include "ColorPAL.h"
#include "OLED.h"
#include "ESP_WiFi.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
const int thre3 = 650;  //Set the threshold of black color


////////////////PARAMETERS/////////////////

int CAR_USED=1;   //Car1 or Car2

///////////////////////////////////////////

int x,y,drc,tx,ty;
char* my_topic,sub_topic;
int mode=0; //0 for manually control up/down/left/right, 1 for task 1, 2 for task 2


Car* car;
ColorPAL* color;
OLED* oled;
WiFi_Car* wificar;

int turnCount=0;
int sensorCount=0;
bool continousTurn=false;
bool otherCarFinish=false;

void setup() {
  if(CAR_USED==1){
    x=0,y=0;
    drc=RIGHT;
    tx=2,ty=5;
    my_topic="IDP_2B_Lance_Car_1/final";
//    sub_topic="IDP_2B_Lance_Car_1/#";
  }else if(CAR_USED==2){
    x=5,y=5;
    drc=LEFT;
    tx=3,ty=0;
    my_topic="IDP_2B_Lance_Car_2/final";
//    sub_topic="IDP_2B_Lance_Car_1/#";
  }
  Serial.begin(9600);
  wificar=new WiFi_Car(my_topic);
  oled=new OLED();
  car=new Car("Car");
  car->moveForward(0);
  // Serial.println("start color!");
  color=new ColorPAL();
  delay(100);
  initCheck();
}

/*
  Check if the red target is at home point
*/
void initCheck(){
  for(int i=0;i<100;i++){
    int findRed=color->senseRed();
    if(findRed==1){
      sensorCount+=1;
    }else if (findRed==-1){
      sensorCount=0;
    }
  }

  if(sensorCount>3){
    wificar->send("find");
    endSearch(true);         
  }
  sensorCount=0;
}

void loop(){
  bool res=false;
  MqttMsg msg=wificar->receive(res);
  Serial.println(msg.topic);
  Serial.println(res);
  char* serial_c_str = new char[msg.payload.length() + 1];
  strcpy(serial_c_str, msg.payload.c_str());
  char* sub_str = strtok(serial_c_str, " ");
  if(res){
    Serial.println(msg.payload);
    
    if(strcmp(sub_str,"MODE")==0){
      //change mode
      sub_str=strtok(NULL, " ");
      if(strcmp(sub_str,"0")==0){
        //move accodring to command
        mode=0;
        sub_str=strtok(NULL," ");
        Serial.println(sub_str);
        int targetdrc=drc;
        if(strcmp(sub_str,"UP")==0){
          targetdrc=UP;
        }else if(strcmp(sub_str,"DOWN")==0){
          targetdrc=DOWN;
        }else if(strcmp(sub_str,"LEFT")==0){
          targetdrc=LEFT;
        }else if(strcmp(sub_str,"RIGHT")==0){
          targetdrc=RIGHT;
        }
        selectDirection(targetdrc);
      }else if(strcmp(sub_str,"1")==0){
        //task1
        mode=1;
        sub_str=strtok(NULL," ");
        tx=sub_str[0]-'0';
        sub_str=strtok(NULL," ");
        ty=sub_str[0]-'0';
      }else if(strcmp(sub_str,"2")==0){
        //task2
        mode=2;
      }
    }
  }
  delete [] serial_c_str;
  Serial.print("mode is :");
  Serial.println(mode);
  if(mode==0){
    Serial.println("random move");
    int nx,ny;
    calculateNextPosition(drc,nx,ny);
    if(checkBoundary(nx,ny)){
      car->goOneBlock();
      x=nx;
      y=ny;
    }else{
      car->moveForward(0);      
    }
    delay(100);
  }else if(mode==1){
    Serial.println("task1");
    loop_Task1();
  }else if(mode==2){
    Serial.println("task2");
    loop_Task2();
  }
}

void loop_Task2() {
  //check the result of other car
  checkOtherCarMessage();
  
  //display the current position
  char buff[100];
  sprintf(buff, "X:%d Y:%d",x,y);
  oled->displayText(buff, oled->WHITE);

  //move to next intersection
  int nx,ny,newdrc;
  bool success=false;
  calculateNextPosition(drc,nx,ny);
  if(checkBoundary(nx,ny)){
      success=goOneBlockAndSearchRed();
      x=nx;
      y=ny;
      if(success){
        endSearch(true);
      }      
      // delay(10);
  }

  //If at the end of the current line, make turns. Cars go in 'S' shape.
  //Car1 takes y-axis with x=0,1,2
  //Car2 takes y-axis with x=3,4,5
  if((!checkBoundary(nx,ny) && turnCount<4) || continousTurn){
    if(turnCount<2){
      //turn right
      newdrc=(drc+1+4)%4;
      car->turnRight90();
    }else{
      //turn left
      newdrc=(drc-1+4)%4;
      car->turnLeft90();
    }
    continousTurn=!continousTurn;
    drc=newdrc;
    turnCount+=1;
  }else if (turnCount>=4 && (!checkBoundary(nx,ny))){
    endSearch(false);
  }
}

void checkOtherCarMessage(){
  bool res=false;
  MqttMsg msg=wificar->receive(res);

  char* serial_c_str = new char[msg.payload.length() + 1];
  strcpy(serial_c_str, msg.payload.c_str());
  char* sub_str = strtok(serial_c_str, " ");
  Serial.println(msg.payload);
  if(res){
    Serial.println(sub_str);
    if(strcmp(sub_str,"Find")==0){
      Serial.println("Find by car 2");
       sub_str = strtok(NULL, " ");
       x=sub_str[0]-'0';
       sub_str = strtok(NULL, " ");
       y=sub_str[0]-'0';
       endSearch(true);
    }else if(strcmp(sub_str,"CannotFind")==0){
      Serial.println("stop here");
      otherCarFinish=true;
    }
  }
  delete [] serial_c_str;
}

void endSearch(bool result){
  car->moveForward(0);
  char buff[100];
  if(result){
    //tell the other car point find    
    sprintf(buff, "Find %d %d",x,y);
    wificar->send(buff);
    
    //display on oled and stop
    sprintf(buff, "Find at:\nX:%d Y:%d",x,y);
    oled->displayText(buff, oled->RED);
  }else{
    //notify the other car
    sprintf(buff, "CannotFind 0 0",x,y);
    wificar->send(buff);

    //wait the result of the other car
    while(!otherCarFinish){
      checkOtherCarMessage();
      Serial.println(otherCarFinish);
      delay(1000);
    }

    //display
    oled->displayText("Cannot find", oled->BLUE);
  }  
  while(true);
}

/*
  Move to the next intersection, based on the current direction of the car.
  Sense color during moving, if red, then return true, else return false.
*/
bool goOneBlockAndSearchRed(){
  int count=0,total_count=0;
  int r_qti,l_qti,m_qti;

  bool status=false;
  int redCount=0;
  while(true){    
    car->getAllQTIReadings(l_qti,m_qti,r_qti);

    //sense the color, set a counter of 3 to handle noise
    int findRed=color->senseRed();
    if(findRed==1){
      redCount+=1;
    }else if (findRed==-1){
      redCount=0;
    }
    if(redCount>3){
      //Red color is found, return true
      return true;
    }

    //sense intersection, return when one block has been passed
    if(l_qti > thre3 && m_qti > thre3 && r_qti > thre3){
      count++;
    }else{
      total_count = count;
      count = 0;
    }

    if(total_count > 10){
      delay(10);
      car->moveForward(0);
      return false;
    }else{
      // follow the line otherwise
      car->followLine(l_qti,m_qti,r_qti);
    }
  }
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

void loop_Task1() {
  if(x==tx && y==ty){
    while(true){
      car->moveForward(0);
    }    
  }
  
  int obstacleDistance=car->getUltraSonicReading();
  int nx,ny,newdrc;
  int dist_l=9999,dist_c=9999,dist_r=9999;
  if(obstacleDistance>15){
    calculateNextPosition(drc,nx,ny);
    if(checkBoundary(nx,ny)){
      dist_c=abs(tx-nx)+abs(ty-ny);
    }
  }
  //turn left
  newdrc=(drc-1+4)%4;
  calculateNextPosition(newdrc,nx,ny);
  if(checkBoundary(nx,ny)){
      dist_l=abs(tx-nx)+abs(ty-ny);
  }
  //turn right
  newdrc=(drc+1+4)%4;
  calculateNextPosition(newdrc,nx,ny);
  if(checkBoundary(nx,ny)){
      dist_r=abs(tx-nx)+abs(ty-ny);
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
  Serial.println("turn to:");
  Serial.println(targetDirection);
  int direcDifference=(targetDirection-drc+4)%4;
  if(direcDifference==0){
    return;
  }else if (direcDifference==1){
    Serial.println("right turn :");
    car->turnRight90();    
  }else if (direcDifference==2){
    Serial.println("right turn1 :");
    car->turnRight90();
    Serial.println("right turn2 :");
    car->turnRight90();
  }else if (direcDifference==3){
    Serial.println("left turn :");
    car->turnLeft90();
  }
  drc=targetDirection;
  return;
}
