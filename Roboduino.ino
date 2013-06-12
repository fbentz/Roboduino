#include <Ultrasonic.h>
#include <Wire.h>
#include <snootor.h>

#define AVANCER 1
#define RECULER 2
#define TOURNER 3
#define ATTENDRE 4


const int REDLED = 7;
const int GREENLED = 8;
const int TRIG = 4;
const int ECHO = 2;
const int DELAY = 50;
const int SPEED = 128;
long cm;
Ultrasonic eyes = Ultrasonic(TRIG,ECHO);
int state = AVANCER;
int duree;
SnootorDC Motor_1;
SnootorDC Motor_2;

void setup() {
  Serial.begin(9600);
  pinMode(REDLED,OUTPUT);
  pinMode(GREENLED,OUTPUT);
  Wire.begin();
  Motor_1.init(1);
  Motor_2.init(2);
  digitalWrite(GREENLED,HIGH);
  digitalWrite(REDLED, LOW);
  Motor_1.setSpeed(SPEED);
  Motor_2.setSpeed(SPEED);
  duree = 0;
  state = AVANCER;
}

void LED_Obstacle(int is_true){
    if(is_true){
     digitalWrite(GREENLED, LOW);
     digitalWrite(REDLED, HIGH); 
    }
    else{
     digitalWrite(GREENLED, HIGH);
     digitalWrite(REDLED, LOW);     
    }
}

void robot_tourner(){
     Motor_1.run(FORWARD);
     Motor_2.run(BACKWARD);
}


void robot_avancer(){
     Motor_1.run(FORWARD);
     Motor_2.run(FORWARD);
}

void robot_reculer(){
     Motor_1.run(BACKWARD);
     Motor_2.run(BACKWARD);
}

void robot_release_motors(){
    Motor_1.run(RELEASE);
    Motor_2.run(RELEASE);

}

void loop() {
    SC.dump();
    cm = eyes.convert(eyes.timing(), 1);
    Serial.println(cm);    
 
    if(cm > 0 && cm < 15) {
        LED_Obstacle(1);
    }
    else {
        LED_Obstacle(0);
      
    }

    switch(state){
    case AVANCER:
      if(cm > 0 && cm < 15) {
         state = ATTENDRE;
         duree = 10; 
      }
      else {
        robot_avancer();
      }
      break;
    
    case ATTENDRE:

      if(duree == 0){
        state = TOURNER;
        duree = 8; 
      }
      else{
        duree--;
      }
      break;
      
    case TOURNER:
      if(duree == 0){
        state = AVANCER;
        if (cm > 0 && cm < 15){
         state = RECULER;
         duree = 5;           
        }
      }
      else {
          duree--;
         robot_tourner();
      }
      break;
     
    case RECULER:
      if(duree == 0){
        state = TOURNER;
        duree = 5;          
      }
     else {
       duree--;
       robot_reculer();
     }
     break;   
      
    }
    delay(50);
    robot_release_motors();    
}

