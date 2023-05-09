#include "simpletools.h"
#include "servo.h"

//pins
int rightWheel = 14;
int leftWheel = 12;
int attackServo = 16;
int echo = 1;
int trigger = 2;
int leftLED = 15;
int rightLED = 0;
int leftSensor = 7;
int rightSensor = 4;
int led = 5;
int enemy = 9;

int leftLimit = 2300; //limit to determine whether bright or dark
int rightLimit = 1300; //limit to determine whether bright or dark
int leftVal = 800; //left sensor reading
int rightVal = 800; //right sensor reading
long duration = 1; //duration of sound wave travel
int distance = 1; //distance measurement
int numIntersection = -2; //number of intersections detected
int numLeft = 0; //number of left turns
int numRight = 0; //number of right turns
int detectingObjects = 1; //detect blocking object only when going down main road
int leftSpeed = 50;
int rightSpeed = -12;
int flag = 0;

void measure_IR();
void follow_line();
void keep_following();
void measure_distance();
void check_friends();
void destroy_enemies();
void turn_right();
void turn_left();
void turn_around();

int main()
{
  //servo_speed(leftWheel,80); //fast
  //servo_speed(rightWheel,-40); //fast
  
  //servo_speed(leftWheel,35); //slow
  //servo_speed(rightWheel,-10); //slow
  
  //servo_speed(leftWheel,22); //stop
  //servo_speed(rightWheel,13); //stop       
  
  /*
  while(1){
    follow_line();
    if (rightVal > rightLimit && leftVal > leftLimit) {
        //pause(100);
        //numIntersection = numIntersection + 1;               
        servo_speed(leftWheel,70);
        servo_speed(rightWheel,-30);
        high(led);
        pause(400);
        low(led);
      }        
  } 
  */ 
  
  //preset attack arm
  servo_angle(attackServo,1700);
  pause(2000);    
  
  //move down main road while detecting front objects to find blocked intersection
  while (detectingObjects == 1){
    //if rpi sending enemy signal then knock off detected enemy   
    if (input(enemy) == 1){
      destroy_enemies();
    }
    else {
      measure_distance();
      follow_line();
      //if intersection detected
      if (rightVal > rightLimit && leftVal > leftLimit) {
        //pause(100);
        numIntersection = numIntersection + 1;  
        //move off intersection while blinking intersection indicator led             
        servo_speed(leftWheel,70);
        servo_speed(rightWheel,-30);
        high(led);
        pause(400);
        low(led);
        
        //curved line "intersection"
        if (numIntersection == -1){
          if (numRight > numLeft) {
            servo_speed(leftWheel,22); //stop
          }  
          if (numRight < numLeft) {
            servo_speed(rightWheel,13); //stop
          } 
          pause(300);        
        }
      }                        
    }                
        
    if (distance < 6){
      //blocking object detected
      detectingObjects = 0;
      pause(300);
      turn_around();
      pause(300);
    }                       
  } 
  
  //blocking object at i2
  //run navigation routine
  if (numIntersection == 1){
    while (numIntersection <= 22){
      if (input(enemy) == 1){
        destroy_enemies();
      }
      else   {
        follow_line();
        if (rightVal > rightLimit && leftVal > leftLimit) {
          //pause(100);
          numIntersection = numIntersection + 1;
          servo_speed(leftWheel,70); //fast
          servo_speed(rightWheel,-30); //fast
          high(led);
          pause(400);
          low(led);
          
          //i1
          if (numIntersection == 2){
            keep_following();
            turn_right();
          }
          //b1
          if (numIntersection == 3){
            keep_following();
            turn_right();
          }  
          //b4
          if (numIntersection == 6) {
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }   
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }          
            turn_right(); 
          }
          //i4
          if (numIntersection == 7) {
            keep_following();
            turn_right(); 
          }
          //i3 
          if (numIntersection == 8) {
            measure_distance();
            while (distance > 4){
              follow_line();
              measure_distance();
            }        
            pause(100);               
            turn_around(); 
            pause(100);
          }
          //i5
          if (numIntersection == 11) {           
            turn_around(); 
          } 
          //i4
          if (numIntersection == 12) {           
            keep_following();
            turn_left(); 
          }
          //a4
          if (numIntersection == 13) {           
            keep_following();
            turn_right(); 
          }
          //a1
          if (numIntersection == 16) {           
            keep_following();
            turn_right(); 
          }
          //b1
          if (numIntersection == 18) {           
            keep_following();
            turn_right(); 
          }
          //b5
          if (numIntersection == 22) { 
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }  
            keep_following();        
            while(1){
              servo_speed(leftWheel,22); //stop
              servo_speed(rightWheel,13); //stop 
            }            
          }
        }          
      }               
    }
  }
  
  //blocking object at i3
  //run navigation routine
  if (numIntersection == 2){
    while (numIntersection <= 22){
      if (input(enemy) == 1){
        destroy_enemies();
      }
      else   {
        follow_line();
        if (rightVal > rightLimit && leftVal > leftLimit) {
          //pause(100);
          numIntersection = numIntersection + 1;
          servo_speed(leftWheel,70); //fast
          servo_speed(rightWheel,-30); //fast
          high(led);
          pause(400);
          low(led);
          
          //i1
          if (numIntersection == 4){
            keep_following();
            turn_right();
          }
          //b1
          if (numIntersection == 5){
            keep_following();
            turn_right();
          }  
          //b4
          if (numIntersection == 8) {
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }   
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }          
            turn_right(); 
          }
          //i4
          if (numIntersection == 9) {
            keep_following();
            turn_right(); 
            measure_distance();
            while (distance > 4){
              follow_line();
              measure_distance();
            }  
            pause(100);                     
            turn_around();
            pause(100);
          }
          //i5
          if (numIntersection == 11) {  
            pause(100);                     
            turn_around(); 
            pause(100);
          }
          //i4
          if (numIntersection == 12) {           
            keep_following();
            turn_left(); 
          }
          //a4
          if (numIntersection == 13) {           
            keep_following();
            turn_right(); 
          }
          //a1
          if (numIntersection == 16) {           
            keep_following();
            turn_right(); 
          }
          //b1
          if (numIntersection == 18) {           
            keep_following();
            turn_right(); 
          }
          //b5
          if (numIntersection == 22) { 
            keep_following(); 
            if (input(enemy) == 1){
              destroy_enemies();
            }
            keep_following();         
            while(1){
              servo_speed(leftWheel,22); //stop
              servo_speed(rightWheel,13); //stop 
            }            
          }
        }          
      }               
    }
  }
  
  //blocking object at i5
  //run navigation routine
  if (numIntersection == 4){
    while (numIntersection <= 23){
      if (input(enemy) == 1){
        destroy_enemies();
      }
      else   {
        follow_line();
        if (rightVal > rightLimit && leftVal > leftLimit) {
          //pause(100);
          numIntersection = numIntersection + 1;
          servo_speed(leftWheel,70); //fast
          servo_speed(rightWheel,-30); //fast
          high(led);
          pause(400);
          low(led);
          
          //i1
          if (numIntersection == 8){
            keep_following();
            turn_right();
          }
          //b1
          if (numIntersection == 9){
            keep_following();
            turn_right();
          }  
          //b4
          if (numIntersection == 12) {
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }   
            keep_following();
            if (input(enemy) == 1){
              destroy_enemies();
            }          
            turn_right(); 
          }
          //a4
          if (numIntersection == 14) {           
            keep_following();
            turn_right(); 
          }
          //a1
          if (numIntersection == 17) {           
            keep_following();
            turn_right(); 
          }
          //b1
          if (numIntersection == 19) {           
            keep_following();
            turn_right(); 
          }
          //b5
          if (numIntersection == 23) { 
            keep_following(); 
            if (input(enemy) == 1){
              destroy_enemies();
            }
            keep_following();         
            while(1){
              servo_speed(leftWheel,22); //stop
              servo_speed(rightWheel,13); //stop 
            }            
          }
        }          
      }               
    }
  }
}

void measure_IR(){
  //turn on IR LEDs
  high(leftLED); high(rightLED);
  
  //charge capacitors
  high(rightSensor); high(leftSensor);
  pause(100);
  //read in reflectance value based on rc_time dissipation
  leftVal = rc_time(leftSensor,1);
  rightVal = rc_time(rightSensor,1);
  
  print("right sensor: %d \n",rightVal);
  print("left sensor: %d \n",leftVal);
  
  //turn off IR LEDs
  low(leftLED); low(rightLED);
  pause(100);
}   

void follow_line(){
  measure_IR();
  
  //turn wheels based on IR readings  
  if (leftVal <= leftLimit) {
    servo_speed(leftWheel,leftSpeed);
  }    
  else{
    servo_speed(leftWheel,22);
    numRight = numRight + 1;
  }     
  
  if (rightVal <= rightLimit) {
    servo_speed(rightWheel,rightSpeed);
  }    
  else{
    servo_speed(rightWheel,13);
    numLeft = numLeft + 1;
  }     
}

void keep_following(){
  for (int i=1;i<=3;i++){
    follow_line();
  }    
} 

void measure_distance(){
  //clear trigger then send ultrasonic chirp
  low(trigger);
  pause(.2);
  high(trigger);
  pause(.1);
  low(trigger);

  //read in echo and calculate measured distance
  duration = pulse_in(echo, 1);
  distance = duration * 0.034 / 2;
  print("distance: %d \n",distance);
} 

void destroy_enemies(){
  //knock off enemy
  servo_speed(leftWheel,22); //stop
  servo_speed(rightWheel,13); //stop
  pause(2000);
  //swing attackServo arm
  servo_angle(attackServo,10);
  pause(1000);
  servo_angle(attackServo,1700);
  pause(1000);
}  

void turn_right(){
  servo_speed(leftWheel,70);
  servo_speed(rightWheel,70);
  pause(850);
  servo_speed(leftWheel,22); //stop
  servo_speed(rightWheel,13); //stop
}

void turn_left(){
  servo_speed(leftWheel,-40);
  servo_speed(rightWheel,-40);
  pause(800);
  servo_speed(leftWheel,22); //stop
  servo_speed(rightWheel,13); //stop
}

void turn_around(){
  servo_speed(leftWheel,-40);
  servo_speed(rightWheel,-40);
  pause(1600);
  servo_speed(leftWheel,22); //stop
  servo_speed(rightWheel,13); //stop
}

