#include <Servo.h> //librería para servo motor
int abrir = 180;
int cerrar = 0;
int pos = 0; 

//INICIO BLOQUE DE CONEXION

int flota1=13;
int flota2=12;
int rojo=2;
int verde=4;
int emergencia=7;
int servo1=9;
int servo2=10;
int servo3=11;
int motor=8;

// FIN BLOQUE DE CONEXION
Servo valv1;  //se define a la valvula 1 como variable servo
Servo valv2;  //se define a la valvula 2 como variable servo
Servo valv3;  //se define a la valvula 3 como variable servo
float adc2, temp2, mV2; //se declaran variables para los sensores de temperatura
float adc1,temp1,mV1;

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


void setup() {
    pinMode(rojo,OUTPUT);        //configura pin 2 como salida digital
    pinMode(verde,OUTPUT);       //configura pin 3 como salida digital
    pinMode(emergencia,INPUT);   //configura boton emergencia como entrada digital
    pinMode(motor, LOW);         // Condición inicial del motor (apagado)
    valv1.attach(servo1);
    valv2.attach(servo2);
    valv3.attach(servo3);
    Serial.begin(9600);
}

void loop() {
  float estanque1 = 0.01723 * readUltrasonicDistance(flota1, flota1);
  Serial.print("estanque1: ");
  Serial.println(estanque1);
  float estanque2 = 0.01723 * readUltrasonicDistance(flota2, flota2);
  Serial.print("estanque2: ");
  Serial.println(estanque2);

  if (estanque1>30) //
  {
    valv1.write(abrir);//--|
    valv2.write(cerrar);//-| llenar estanque 1 
  }
  else
  {
    valv1.write(cerrar);// estanque lleno
    digitalWrite(motor,HIGH);
    while(temp1<=40){
      adc1 = analogRead(A0);   //
      mV1= (adc1*5)/1024;      //
      temp1 = ((mV1 * 100)-50.541)/0.9904; //con factor de correccion
      Serial.print("la temperatura es: ");
      Serial.println(temp1);
      delay(30);
    }
    digitalWrite(motor,LOW);
    // medicion temperatura estanque 1
 
    //
    if (estanque2>30 && temp1>=40)
    {
     valv2.write(abrir);
    }
    else 
    {
      valv2.write(cerrar);
   
    }
  }
  adc2 = analogRead(A1);   //se lee señal analogica 0-1023 (cambias A0)
  mV2= (adc2*5)/1024;      //se convierte valor digital a mV
  temp2 = ((mV2 * 100)-50.541)/0.9904;         //convertimos los mV a temp
  Serial.print("la temperatura 2.0: ");
  Serial.println(temp2);
  if(temp2 < 80)   //si la temperatura es menor a 80 se cierra la valvula 3
  {
    valv3.write(cerrar);
  }
    if(temp2 >=80)  //si la temperatura es mayor o igual a 80 se abre la valvula 3
  {
    valv3.write(abrir);
  }
}
