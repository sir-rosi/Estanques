#include <Servo.h>
int pos = 0;
int servo1=9;
int servo2=10;
int servo3=11;
Servo valv1;
Servo valv2;
Servo valv3;
float adc0, temp, mV; //se declaran variables para sensor de temperatura
void setup() {
    pinMode(2,OUTPUT);    //configura pin 2 como salida
    pinMode(3,OUTPUT);    //configura pin 3 como salida digital
    valv1.attach(servo1);
      valv2.attach(servo2);
      valv3.attach(servo3);
}

void loop() {
  adc0 = analogRead(0);   //se lee señal analogica 0-1023
  mV= (adc0*5)/1024;      //se convierte valor digital a mV
  temp = mV * 100;        //convertimos los mV a temp

  if(temp < 80)
  {
    valv3.write(0);
  }
    if(temp >=80)
  {
    valv3.write(180);
  }
}
