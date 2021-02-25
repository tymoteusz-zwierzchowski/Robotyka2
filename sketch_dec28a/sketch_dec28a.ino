#include <AccelStepper.h>
#include <TimeLib.h>

#define MotorInterfaceType 1

AccelStepper silnikZ = AccelStepper(MotorInterfaceType, 2, 3);
AccelStepper silnikX = AccelStepper(MotorInterfaceType, 6, 7);
AccelStepper silnikY = AccelStepper(MotorInterfaceType, 10, 11);

String incomingString; // for incoming serial data

unsigned long timer = 0;
unsigned long timer5 = 0;
unsigned long timer1 = 0;
int currentMillis = 0;
int oldSecond = 0;

int xValue = 0;
int yValue = 0;
int zValue = 0;
int xValues = 0;
int yValues = 0;
int zValues = 0;

void setup() {
  
  silnikZ.setMaxSpeed(6400);
  silnikX.setMaxSpeed(6400);
  silnikY.setMaxSpeed(6400);

  silnikZ.setAcceleration(2880000);
  silnikX.setAcceleration(2880000);
  silnikY.setAcceleration(2880000);

  silnikZ.setCurrentPosition(0);
  silnikX.setCurrentPosition(0);
  silnikY.setCurrentPosition(0);
  
  Serial.begin(115200); // opens serial port, sets data rate to 115200 bps
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming string:
    incomingString = Serial.readString();
      String mode = incomingString.substring(0, 1);
      if (mode == "t"){
        String dd = incomingString.substring(1, 3);
        String MM = incomingString.substring(4, 6);
        String yy = incomingString.substring(7, 11);
        String hh = incomingString.substring(12, 14);
        String mm = incomingString.substring(15, 17);
        String ss = incomingString.substring(18, 20);
        currentMillis = 0;
        oldSecond = ss.toInt();
        setTime(hh.toInt(), mm.toInt(), ss.toInt(), dd.toInt(), MM.toInt(), yy.toInt());
      }
      else {
      
      String xString = incomingString.substring(0, 3);
      if (xString == "rt+") xValue = 361;
      else if (xString == "rt-") xValue = -1;
      else xValue = xString.toInt();
      
      String yString = incomingString.substring(4, 7);
      if (yString == "rt+") yValue = 361;
      else if (yString == "rt-") yValue = -1;
      else yValue = yString.toInt();
      
      String zString = incomingString.substring(8, 11);
      if (zString == "rt+") zValue = 361;
      else if (zString == "rt-") zValue = -1;
      else zValue = zString.toInt();
      
      String xStrings = incomingString.substring(12, 15);
      xValues = xStrings.toInt();
      String yStrings = incomingString.substring(16, 19);
      yValues = yStrings.toInt();
      String zStrings = incomingString.substring(20, 23);
      zValues = zStrings.toInt();

      timer = millis();
      int add = timer - timer1;
      timer1 = timer;
      currentMillis=currentMillis+add;
      if (second() != oldSecond){
        currentMillis=currentMillis-1000;
        oldSecond=second();
      }

      //rotate(xValue, yValue, zValue, xValues, yValues, zValues);
      }
  }

  int x = xValue;
  int y = yValue;
  int z = zValue;
  int xs = xValues;
  int ys = yValues;
  int zs = zValues;

  if (x>360) x = 3250;
  else if (x<0) x = -50;
  else x = map(x, 0, 360, 0, 3200);
  if (y>360) y = 3250;
  else if (y<0) y = -50;
  else y = map(y, 0, 360, 0, 3200);
  if (z>360) z = 3250;
  else if (z<0) z = -50;
  else z = map(z, 0, 360, 0, 3200);

    if (silnikZ.currentPosition() != z && silnikZ.currentPosition() > z){
      silnikZ.setSpeed(-zs*160/3);
      silnikZ.runSpeed();
    }
    if (silnikZ.currentPosition() != z && silnikZ.currentPosition() < z){
      silnikZ.setSpeed(zs*160/30);
      silnikZ.runSpeed();
    }
    if (silnikX.currentPosition() != x && silnikX.currentPosition() > x){
      silnikX.setSpeed(-xs*160/3);
      silnikX.runSpeed();
    }
    if (silnikX.currentPosition() != x && silnikX.currentPosition() < x){
      silnikX.setSpeed(xs*160/3);
      silnikX.runSpeed();
    }
    if (silnikY.currentPosition() != y && silnikY.currentPosition() > y){
      silnikY.setSpeed(-ys*160/3);
      silnikY.runSpeed();
    }
    if (silnikY.currentPosition() != y && silnikY.currentPosition() < y){
      silnikY.setSpeed(ys*160/3);
      silnikY.runSpeed();
    }

    if (silnikX.currentPosition()>3200) silnikX.setCurrentPosition(1);
    else if (silnikX.currentPosition()<0) silnikX.setCurrentPosition(3199);
    if (silnikY.currentPosition()>3200) silnikY.setCurrentPosition(1);
    else if (silnikY.currentPosition()<0) silnikY.setCurrentPosition(3199);
    if (silnikZ.currentPosition()>3200) silnikZ.setCurrentPosition(1);
    else if (silnikZ.currentPosition()<0) silnikZ.setCurrentPosition(3199);
  
  timer = millis();
  if (timer - timer1 >= 1UL){
      int add = timer - timer1;
      timer1 = timer;
      
    if (timer - timer5 >= 2UL) {
        timer5 = timer;
        float xx = float(silnikX.currentPosition())*9/80;
        String sendX = String(xx);
        if(xx<100) sendX = "0" + sendX;
        if(xx<10) sendX = "0" + sendX;
        float yy = float(silnikY.currentPosition())*9/80;
        String sendY = String(yy);
        if(yy<100) sendY = "0" + sendY;
        if(yy<10) sendY = "0" + sendY;
        float zz = float(silnikZ.currentPosition())*9/80;
        String sendZ = String(zz);
        if(zz<100) sendZ = "0" + sendZ;
        if(zz<10) sendZ = "0" + sendZ;
        String dayS = String(day());
        if(day()<10) dayS = "0" + dayS;
        String monthS = String(month());
        if(month()<10) monthS = "0" + monthS;
        String yearS = String(year());
        String hourS = String(hour());
        if(hour()<10) hourS = "0" + hourS;
        String minuteS = String(minute());
        if(minute()<10) minuteS = "0" + minuteS;
        String secondS = String(second());
        if(second()<10) secondS = "0" + secondS;
        if (second() == oldSecond){
          currentMillis=currentMillis+add;
          if (currentMillis>999){
            currentMillis=999;
          }
        }
        else {
          oldSecond=second();
          currentMillis=0;
        }
        String milliS = String(currentMillis);
        if(currentMillis<100) milliS = "0" + milliS;
        if(currentMillis<10) milliS = "0" + milliS;
        Serial.print(sendX + ";" + sendY + ";" + sendZ + ";" + dayS + ";" + monthS + ";" + yearS + ";" + hourS + ";" + minuteS + ";" + secondS + ";" + milliS + "\n");
      }
}
}
//
//void rotate(int x, int y, int z, int xs, int ys, int zs) {
//  if (x>360) x = 201;
//  else if (x<0) x = -1;
//  else x = map(x, 0, 360, 0, 200);
//  if (y>360) y = 201;
//  else if (y<0) y = -1;
//  else y = map(y, 0, 360, 0, 200);
//  if (z>360) z = 250;
//  else if (z<0) z = -50;
//  else z = map(z, 0, 360, 0, 200);
//
//  while (silnikZ.currentPosition() != z || silnikX.currentPosition() != x || silnikY.currentPosition() != y)  {
//
//    if (silnikX.currentPosition()>199) silnikX.setCurrentPosition(0);
//    else if (silnikX.currentPosition()<1) silnikX.setCurrentPosition(200);
//    if (silnikY.currentPosition()>199) silnikY.setCurrentPosition(0);
//    else if (silnikY.currentPosition()<1) silnikY.setCurrentPosition(200);
//    if (silnikZ.currentPosition()>199) silnikZ.setCurrentPosition(0);
//    else if (silnikZ.currentPosition()<1) silnikZ.setCurrentPosition(200);
//    
//    if (silnikZ.currentPosition() != z && silnikZ.currentPosition() > z){
//      silnikZ.setSpeed(-zs*10/3);
//      silnikZ.runSpeed();
//    }
//    if (silnikZ.currentPosition() != z && silnikZ.currentPosition() < z){
//      silnikZ.setSpeed(zs*10/3);
//      silnikZ.runSpeed();
//    }
//    if (silnikX.currentPosition() != x && silnikX.currentPosition() > x){
//      silnikX.setSpeed(-xs*10/3);
//      silnikX.runSpeed();
//    }
//    if (silnikX.currentPosition() != x && silnikX.currentPosition() < x){
//      silnikX.setSpeed(xs*10/3);
//      silnikX.runSpeed();
//    }
//    if (silnikY.currentPosition() != y && silnikY.currentPosition() > y){
//      silnikY.setSpeed(-ys*10/3);
//      silnikY.runSpeed();
//    }
//    if (silnikY.currentPosition() != y && silnikY.currentPosition() < y){
//      silnikY.setSpeed(ys*10/3);
//      silnikY.runSpeed();
//    }
//
//    timer = millis();
//  if (timer - timer1 >= 1UL){
//      int add = timer - timer1;
//      timer1 = timer;
//      
//    if (timer - timer5 >= 2UL) {
//        timer5 = timer;
//        int xx = map(silnikX.currentPosition(), 0, 200, 0, 360);
//        String sendX = String(xx);
//        if(xx<100) sendX = "0" + sendX;
//        if(xx<10) sendX = "0" + sendX;
//        int yy = map(silnikY.currentPosition(), 0, 200, 0, 360);
//        String sendY = String(yy);
//        if(yy<100) sendY = "0" + sendY;
//        if(yy<10) sendY = "0" + sendY;
//        int zz = map(silnikZ.currentPosition(), 0, 200, 0, 360);
//        String sendZ = String(zz);
//        if(zz<100) sendZ = "0" + sendZ;
//        if(zz<10) sendZ = "0" + sendZ;
//        String dayS = String(day());
//        if(day()<10) dayS = "0" + dayS;
//        String monthS = String(month());
//        if(month()<10) monthS = "0" + monthS;
//        String yearS = String(year());
//        String hourS = String(hour());
//        if(hour()<10) hourS = "0" + hourS;
//        String minuteS = String(minute());
//        if(minute()<10) minuteS = "0" + minuteS;
//        String secondS = String(second());
//        if(second()<10) secondS = "0" + secondS;
//        if (second() == oldSecond){
//          currentMillis=currentMillis+add;
//          if (currentMillis>999){
//            currentMillis=999;
//          }
//        }
//        else {
//          oldSecond=second();
//          currentMillis=0;
//        }
//        String milliS = String(currentMillis);
//        if(currentMillis<100) milliS = "0" + milliS;
//        if(currentMillis<10) milliS = "0" + milliS;
//        Serial.print(sendX + ";" + sendY + ";" + sendZ + ";" + dayS + ";" + monthS + ";" + yearS + ";" + hourS + ";" + minuteS + ";" + secondS + ";" + milliS + "\n");
//      }
//}
//}
//}
