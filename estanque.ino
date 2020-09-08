#include <Servo.h> //librería para servo motor

// Grados de apertura para las válvulas
int abrir = 180; 
int cerrar = 0;

//INICIO BLOQUE DE CONEXION

int rojo1=2;
int azul1=3;
int rojo2=4;
int azul2=5;
int boton2=6;  // para vaciar estanque 2
int boton=7;   // para iniciar el sistema
int motor=8;
int servo1=9;  // simula valvula 1
int servo2=10; // simula valvula 2
int servo3=11; // simula valvula 3
int flota1=13; // simula flotador en el estanque 1
int flota2=12; // simula flotador en el estanque 2

// FIN BLOQUE DE CONEXION
Servo valv1;  //se define a la valvula 1 como variable servo
Servo valv2;  //se define a la valvula 2 como variable servo
Servo valv3;  //se define a la valvula 3 como variable servo
float adc1, temp1, mV1; //se declaran variables para los sensores de temperatura estanque 1
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
        
    Serial.begin(9600);
    pinMode(motor, LOW);         // Condición inicial del motor (apagado)
    valv1.attach(servo1);        // Asignación de pin para la válvula 1
    valv2.attach(servo2);        // Asignación de pin para la válvula 2
    valv3.attach(servo3);        // Asignación de pin para la válvula 3
    valv1.write(cerrar);
  	valv2.write(cerrar);
    valv3.write(cerrar);
  	pinMode(azul1, LOW);
    pinMode(rojo1, LOW);
    pinMode(azul2, LOW);
    pinMode(rojo2, LOW);
    
}

void loop() {
  //MEDICIÓN DE FLOTADORES
  if (digitalRead(boton)==true){      
  float estanque1 = 0.01734 * readUltrasonicDistance(flota1, flota1); // se calibra lectura de distancia y se asigna a estanque 1
  Serial.print("estanque1: ");
  Serial.println(estanque1);    // se imprime el valor de la distancia en estanque 1
  float estanque2 = 0.01734 * readUltrasonicDistance(flota2, flota2); // se calibra lectura de distancia y se asigna a estanque 1
  Serial.print("estanque2: ");
  Serial.println(estanque2);    // se imprime el valor de la distancia en estanque
  //FIN MEDICIÓN DE FLOTADORES
  
  //ESTANQUE 1
  if (estanque1>30)             // Condición de llenado para estanque 1, 
  {
    digitalWrite(azul1, HIGH);   // Enciende luz1 azul para indicar que se está llenando
    valv1.write(abrir);         // Se abre la válvula 1 para ingreso de líquido
    valv2.write(cerrar);        // se cierra la válvula 2 para llenar estanque 1 
  }
  if(estanque1<=30)                          // Estanque capacidad máxima
  //FIN DE LLENADO DE ESTANQUE 1
  {
    valv1.write(cerrar);        // se cierra la válvula 1
    adc1 = analogRead(A0);    // Se lee señal analogica 0-1023 (cambias A0)
    mV1= (adc1*5)/1024;       // Conversión de valor digital a mV
    temp1 = ((mV1 * 100)-50.541)/0.9904;
    while(temp1<=40){           // Ciclo while para precalentado del contenido del estanque 1
      digitalWrite(azul1, LOW);
      digitalWrite(rojo1, HIGH);  // Luz roja1 indica que el estaque 1 se llenó
      digitalWrite(motor,HIGH);   // Gira motor para revolver contenido estanque 1
      
      adc1 = analogRead(A0);    // Se lee señal analogica 0-1023 (cambias A0)
      mV1= (adc1*5)/1024;       // Conversión de valor digital a mV
      temp1 = ((mV1 * 100)-50.541)/0.9904; //Calibración de sensor de temperatura. Convertimos los mV a temp
      Serial.print("la temperatura es: "); 
      Serial.println(temp1);    // Se imprime valor de temperatura de temperatura
      delay(30);
    }
    digitalWrite(rojo1, LOW);
    digitalWrite(motor, LOW);    // se para el motor cuando se alcanza la temperatura de pre calentado
  }
    
    //SE LLENA ESTANQUE 2
        while(estanque2>30 && temp1>=40) // condición de llenado de estanque 2 
    {
     digitalWrite(azul2, HIGH);  //Enciende luz azul2 para indicar que el estanque 2 se está llenando
     valv2.write(abrir);
     estanque2 = 0.01734 * readUltrasonicDistance(flota2, flota2); // se abre válvula 2 (ingresa fluido a estanque 2)
    }
    if(estanque2<=30)                        // si no se cumple la condición anterior, seguirá cerrada la válvula 2
    //ESTANQUE 2 LLENO
    {
      digitalWrite(azul2, LOW);
      digitalWrite(rojo2, HIGH);  // Luz roja2 indica que el estanque 2 se llenó
      valv2.write(cerrar);
   
    }
  adc2 = analogRead(A1);        // Se lee señal analogica 0-1023 (cambias A1)
  mV2= (adc2*5)/1024;           // Se convierte valor digital a mV
  temp2 = ((mV2 * 100)-50.541)/0.9904;    //Calibración de sensor de temperatura. Convertimos los mV a temp
  Serial.print("la temperatura 2.0: ");
  Serial.println(temp2);        // Se imprime valor de la temperatura del estanque 2
        
  if(temp2 < 80 )                // Si la temperatura es menor a 80 se cierra la valvula 3
  {
    valv3.write(cerrar);
  }
  while(temp2 >=80 && digitalRead(boton2)==true )              //si la temperatura es mayor o igual a 80 se abre la valvula 3
  //ESTANQUE 2 VACIANDOSE
  {
    digitalWrite(rojo2, LOW);  
    valv3.write(abrir);
  }
  valv3.write(cerrar);
  }
}
