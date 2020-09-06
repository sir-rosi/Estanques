#include <Servo.h> //librería para servo motor
int abrir = 180;
int cerrar = 0;
int pos = 0; 
int servo1=9;
int servo2=10;
int servo3=11;
int motor=13;
Servo valv1;  //se define a la valvula 1 como variable servo
Servo valv2;  //se define a la valvula 2 como variable servo
Servo valv3;  //se define a la valvula 3 como variable servo

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Inicializar LOW para limpiar trigger por 2 microsegundos
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Inicializar trigger en HIGH por 8 microsegundos para comenzar
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(8);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // lectura de pin echo con el retorno de la señal
  return pulseIn(echoPin, HIGH);
}

float adc0, temp, mV; //se declaran variables para sensor de temperatura

void setup() {
    pinMode(2,OUTPUT);    //configura pin 2 como salida
    pinMode(3,OUTPUT);    //configura pin 3 como salida digital
    valv1.attach(servo1);
    valv2.attach(servo2);
    valv3.attach(servo3);
}

void loop() {
  float estanque1 = 0.01723 * readUltrasonicDistance(2, 2);
  float estanque2 = 0.01723 * readUltrasonicDistance(3,3);
  if (estanque1>30); //
  {
    valv1(abrir);//--|
    valv2(cerrar);//-| llenar estanque 1 
  }
  else (
  {
    valv1(cerrar);// estanque lleno
    digitalWrite(motor,HIGH);
  }
  adc0 = analogRead(0);   //se lee señal analogica 0-1023
  mV= (adc0*5)/1024;      //se convierte valor digital a mV
  temp = mV * 100;        //convertimos los mV a temp

  if(temp < 80)   //si la temperatura es menor a 80 se cierra la valvula 3
  {
    valv3.write(cerrar);
  }
    if(temp >=80)  //si la temperatura es mayor o igual a 80 se abre la valvula 3
  {
    valv3.write(abrir);
  }
}
