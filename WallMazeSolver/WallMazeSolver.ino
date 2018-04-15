
#include <NewPing.h>

#define MAX_DISTANCE 200
//Motor sheld Pins
//M1
int enA = 5;
int in1 = 3;
int in2 = 4;
//M2
int enB = 6;
int in3 = 7;
int in4 = 8;

int setp = 0;

int encoderValue = 0;
void count(void);
/////////////////////////////////////////////////////// Ultra Sonics///////////////////
int trigLF = 11;    //Trig - green Jumper
int echoLF = 10;    //Echo - yellow Jumper

int trigRF = 18;    //Trig - green Jumper
int echoRF = 19;    //Echo - yellow Jumper


int trigF = 16;    //Trig - green Jumper
int echoF = 17;    //Echo - yellow Jumper

int trigRB = 14;    //Trig - green Jumper
int echoRB = 15;    //Echo - yellow Jumper
/////////////////////////////////////////////////////// Ultra Sonics END////////////////
long duration, cmF, cmLB, cmLF, cmRB, cmRF;
int beepPin = 13;

NewPing left(trigLF, echoLF, MAX_DISTANCE);
NewPing front(trigF, echoF, MAX_DISTANCE);
NewPing right(trigRF, echoRF, MAX_DISTANCE);

NewPing rightB(trigRB, echoRB, MAX_DISTANCE);
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);

  Serial.begin(9600);
  pinMode(21, INPUT);
  delay(300);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(beepPin, OUTPUT);
  attachInterrupt(2, count, FALLING); //A
  pinMode(trigLF, OUTPUT);
  pinMode(echoLF, INPUT);

  pinMode(trigRF, OUTPUT);
  pinMode(echoRF, INPUT);


  pinMode(trigF, OUTPUT);
  pinMode(echoF, INPUT);

  delay(2000);
}

int mode = 0;
//0-COLOR DETECT
//1-MAZE SOLVE
//2-COLOR LINE FOLLOW
//3-GRAP THE LOAD
void loopx() {
  forward(200, 70);
  Serial.print(" -right");
  delay(2000);

  forward(70, 200);
  Serial.print(" -left");
  delay(2000);
}
void loop() {
  // put your main code here, to run repeatedly:
  cmLF = left.ping_cm();
  cmRF = right.ping_cm();
  cmF = front.ping_cm();

  cmRB = rightB.ping_cm();
  //Algorithem..
  //right
  //forward
  //left

  Serial.print("  LF :");
  Serial.print(cmLF);
  Serial.print(" F :");
  Serial.print(cmF);

  //Serial.print(" RB :");
  //Serial.print(cmRB);

  Serial.print("  RF :");
  Serial.println(cmRF);




  if (cmF > 0 && cmLF > 0 && cmRF > 0) {
    if (cmF > 6)
    {


      if (cmRF > 13 && cmRF < 15) //6 9
      {
        forward(130, 130);
        Serial.print(" -forward");
      }

      if (cmRF >= 15)
      {
        if (cmRF >= 25)
        {//////////////////////////////////////////////
          if (cmLF > 13 && cmLF < 15) //6 9
          {
            forward(130, 130);
            Serial.print(" -forward");
          } if (cmLF >= 15)
          { forward(110, 140);
            Serial.print(" -turn slow left");

          }
          if (cmLF <= 13)
          {
            forward(140, 110);
            Serial.print(" -turn slow right");
          }
//////////////////////////////////////////////

          
        } else {
          forward(140, 100);
          Serial.print(" -turn slow right");
        }


      }


      if (cmRF <= 13)
      {
        forward(100, 140);
        Serial.print(" -turn slow left");
      }
    }
    if (cmF <= 6) {
      back(100, 100);
      delay(60);
      stop(1000);
      //    resetPosition();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


      if (cmLF <= 20 && cmRF > 30 && cmF > 0) {
        Serial.print("right 90");
        encoderWrite(200, -1);
      }

      if (cmLF > 30 && cmRF > 30 && cmF > 0) {
        Serial.print("right 90");
        encoderWrite(200, -1);
      }

      if (cmRF <= 30 && cmLF > 30 && cmF > 0) {
        Serial.print("left 90");
        encoderWrite(230, 1);
      }

      if (cmRF <= 20 && cmLF <= 20 && cmF > 0) {
        Serial.print("180 ");
        encoderWrite(460, -1);
      }

    }


  }




}


long getDistance(int tp, int ep) {
  digitalWrite(tp, HIGH);
  delayMicroseconds(10);
  digitalWrite(tp, LOW);
  duration = pulseIn(ep, HIGH, 100000);
  return (duration / 2) / 29.1;
}

void mpower(int motor, int rotation, int spd) {
  int pwm;
  int pA;
  int pB;
  if (motor == 1) {
    pwm = enA;
    pA = in1;
    pB = in2;
  } else if (motor == 2) {
    pwm = enB;
    pA = in3;
    pB = in4;
  } else {
    return;
  }
  if (rotation == 0) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, LOW);
  } else if (rotation == 1) {
    digitalWrite(pA, HIGH);
    digitalWrite(pB, LOW);
  } else if (rotation == -1) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, HIGH);
  }
  analogWrite(pwm, spd);

}

void stop(int d) {
  mpower(1, 0, 100);
  mpower(2, 0, 100);
  delay(d);
}

void encoderWrite(int val, int d) { //left d 1 righr d -1
  encoderValue = 0;
  Serial.print("Encoder Value=");
  Serial.println(encoderValue);
  while (val > encoderValue) {
    mpower(1, d, 120);
    mpower(2, -d, 120);
  }
  // mpower(1,-1,100);
  //mpower(2,1,100);
  //delay(50);
  mpower(1, 0, 100);
  mpower(2, 0, 100);


}

void forward(int l, int r) {

  mpower(1, 1, r);
  mpower(2, 1, l);
}
void back(int l, int r) {

  mpower(1, -1, r);
  mpower(2, -1, l);
}

void count() {
  encoderValue++;
}
void beep(int d) {
  digitalWrite(beepPin, HIGH);
  delay(d);
  digitalWrite(beepPin, LOW);
}

void leftbackCheck() {
  //   cmLF = getDistance(trigLF,echoLF);
  //cmLB = getDistance(trigLB ,echoLB);
  //cmRF = getDistance(trigRF,echoRF);
  //cmRB = getDistance(trigRB,echoRB);
  //cmF = getDistance(trigF,echoF);
  if (cmLB >= 7) {
    //go left
    //forward(70,80);
    forward(50, 60);
    Serial.print(" -turn slow left");
  }



  else if (cmLB <= 5) {
    //go right
    //forward(80,70);
    forward(60, 50);
    Serial.print(" -turn slow right");
  }

}
void resetPosition(int a) {

  //right wall here
  //a=1// a=2//right//a=3 skip

  while (setp == 0) {
    cmLF = left.ping_cm();
    cmRF = right.ping_cm();
    cmF = front.ping_cm();

    cmRB = rightB.ping_cm();

    if ((cmRF > 6 && cmRF < 9) && (cmRB > 6 && cmRB < 9))
    {
      forward(0, 0);
      delay(1000);
      setp = 1;
    }

    if (cmRF >= 9 && cmRB <= 6)
    {
      forward(60, 0);
      Serial.print(" -turn slow right Reset");
    }


    if (cmRF <= 6 && cmRB >= 9 )
    {
      forward(0, 60);
      Serial.print(" -turn slow left Reset");
    }

  }
  setp = 0;
}

