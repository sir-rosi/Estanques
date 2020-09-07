#include <Servo.h> //librería para servo motor

// Grados de apertura para las válvulas
int abrir = 180; 
int cerrar = 0;

//INICIO BLOQUE DE CONEXION

int flota1=13; // simula flotador en el estanque 1
int flota2=12; // simula flotador en el estanque 2
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
float adc1,temp1,mV1; //se declaran variables para los sensores de temperatura estanque 1
float adc2, temp2, mV2; //se declaran variables para los sensores de temperatura estanque 2

long readUltrasonicDistance(int triggerPin, int echoPin) // función que permite lectura de los sensores de proximidad.
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
    valv1.attach(servo1);        // Asignación de pin para la válvula 1
    valv2.attach(servo2);        // Asignación de pin para la válvula 2
    valv3.attach(servo3);        // Asignación de pin para la válvula 3
    Serial.begin(9600);
}

void loop() {
        
  float estanque1 = 0.01734 * readUltrasonicDistance(flota1, flota1); // se calibra lectura de distancia y se asigna a estanque 1
  Serial.print("estanque1: ");
  Serial.println(estanque1);    // se imprime el valor de la distancia en estanque 1
  float estanque2 = 0.01734 * readUltrasonicDistance(flota2, flota2); // se calibra lectura de distancia y se asigna a estanque 1
  Serial.print("estanque2: ");
  Serial.println(estanque2);    // se imprime el valor de la distancia en estanque

  if (estanque1>30)             // Condición de llenado para estanque 1, 
  {
    valv1.write(abrir);         // Se abre la válvula 1 para ingreso de líquido
    valv2.write(cerrar);        // se cierra la válvula 2 para llenar estanque 1 
  }
  else                          // Estanque capacidad máxima
  {
    valv1.write(cerrar);        // se cierra la válvula 1
    digitalWrite(motor,HIGH);   // Gira motor para revolver contenido estanque 1
    while(temp1<=40){           // Ciclo while para precalentado del contenido del estanque 1
      adc1 = analogRead(A0);    // Se lee señal analogica 0-1023 (cambias A0)
      mV1= (adc1*5)/1024;       // Conversión de valor digital a mV
      temp1 = ((mV1 * 100)-50.541)/0.9904; //Calibración de sensor de temperatura. Convertimos los mV a temp
      Serial.print("la temperatura es: "); 
      Serial.println(temp1);    // Se imprime valor de temperatura de temperatura
      delay(30);
    }
    digitalWrite(motor,LOW);    // se para el motor cuando se alcanza la temperatura de pre calentado
 
    if (estanque2>30 && temp1>=40) // condición de llenado de estanque 2 
    {
     valv2.write(abrir);        // se abre válvula 2 (ingresa fluido a estanque 2)
    }
    else                        // si no se cumple la condición anterior, seguirá cerrada la válvula 2
    {
      valv2.write(cerrar);
   
    }
  }
  adc2 = analogRead(A1);        // Se lee señal analogica 0-1023 (cambias A1)
  mV2= (adc2*5)/1024;           // Se convierte valor digital a mV
  temp2 = ((mV2 * 100)-50.541)/0.9904;    //Calibración de sensor de temperatura. Convertimos los mV a temp
  Serial.print("la temperatura 2.0: ");
  Serial.println(temp2);        // Se imprime valor de la temperatura del estanque 2
        
  if(temp2 < 80)                // Si la temperatura es menor a 80 se cierra la valvula 3
  {
    valv3.write(cerrar);
  }
    if(temp2 >=80)              //si la temperatura es mayor o igual a 80 se abre la valvula 3
  {
    valv3.write(abrir);
  }
}
