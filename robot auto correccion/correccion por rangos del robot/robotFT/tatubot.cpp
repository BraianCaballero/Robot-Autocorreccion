#include <ArduinoQueue.h>
#include "tatubot.h"
#include "LedControl.h"     
#include "caritas.h"

Motor motor0(IN1, IN2, ENA);
Motor motor1(IN4, IN3, ENB);
Led led1(PIN_R_A,PIN_G_A,PIN_B_A);
Led led2(PIN_R_B,PIN_G_B,PIN_B_B);
ArduinoQueue<int> colaOrdenes(CANTIDAD_ORDENES);
LedControl matrizLeds=LedControl(DIN,CLK,CS,MATRICES);

bool contador0EnHigh = false;
bool contador1EnHigh = false;

int cantidadDeImanesPrueba0;
int cantidadDeImanesPrueba1;
int VELOCIDAD_MOTOR0 = 150;
int VELOCIDAD_MOTOR1 = 150; 
int velocidadFinal0 = 0;
int velocidadFinal1 = 0;

void Robot::iniciar() {
  pinMode(pinEscucharOrdenes, INPUT_PULLUP);
  pinMode(pinAvanzar, INPUT_PULLUP);
  pinMode(pinReversa, INPUT_PULLUP);
  pinMode(pinGiroDerecha, INPUT_PULLUP);
  pinMode(pinGiroIzquierda, INPUT_PULLUP);
  pinMode(pinFinOrdenes, INPUT_PULLUP);
  pinMode(SENSOR0, INPUT_PULLUP);
  pinMode(SENSOR1, INPUT_PULLUP);
  Serial.begin(115200);
  motor0.iniciar();
  motor1.iniciar();
  led1.iniciar();
  led2.iniciar();
  matrizLeds.shutdown(0,false);     
  matrizLeds.setIntensity(0,4);    
  matrizLeds.clearDisplay(0);    
  matrizLeds.shutdown(1,false);    
  matrizLeds.setIntensity(1,4);    
  matrizLeds.clearDisplay(1);    
}


void Robot::avanzar(int velocidad, int velocidad2) {
  delay(500);
  cantidadDeImanesPrueba0 = 0;
  cantidadDeImanesPrueba1 = 0;
  delay(20);
  while (digitalRead (SENSOR0) == HIGH && digitalRead (SENSOR1) == HIGH) {
      motor0.adelante(255);
      motor1.adelante(255);
      delay (10);
      motor0.atras(150);
      motor1.atras(150);
      delay(5);
      pararRobot();
  }
  delay (2000);
  for(int ciclo=0; ciclo<80; ciclo++)
  {
      motor0.adelante(velocidad);
      motor1.adelante(velocidad2);
      delay (15);
      led1.rgb('g');
      led2.rgb('g');
      contador();
  }
  compararYCorregir();
  delay(10);
  motor0.atras(160);
  motor1.atras(185);
  delay(13);
  pararRobot();
  //Serial.print("contador0 = ");
  //Serial.println(cantidadDeImanesPrueba0);
  //Serial.print("contador1 = ");
  //Serial.println(cantidadDeImanesPrueba1);
  led1.rgb('a');
  led2.rgb('a');
}


void Robot::retroceder(int velocidad, int velocidad2) {
  delay(500);
  cantidadDeImanesPrueba0 = 0;
  cantidadDeImanesPrueba1 = 0;
  delay(20);
  while (digitalRead (SENSOR0) == HIGH && digitalRead (SENSOR1) == HIGH) {
      motor0.atras(255);
      motor1.atras(255);
      delay (10);
      motor0.adelante(150);
      motor1.adelante(150);
      delay(5);
      pararRobot();
  }
  delay (2000);
  for(int ciclo=0; ciclo<40; ciclo++)
  {
      motor0.atras(velocidad);
      motor1.atras(velocidad2);
      delay (15);
      led1.rgb('r');
      led2.rgb('r');
      //Serial.print("Retocediendo \n");
      contador();
  }
  compararYCorregir();
  delay(10);
  motor0.adelante(170);
  motor1.adelante(170);
  delay(15);
  pararRobot();
  led1.rgb('a');
  led2.rgb('a');
  
}

void Robot::girarDerecha(int velocidad) {
  cantidadDeImanesPrueba0 = 0;
  cantidadDeImanesPrueba1 = 0;
  while (digitalRead (SENSOR0) == HIGH && digitalRead (SENSOR1) == HIGH) {
      motor0.adelante(255);
      motor1.adelante(255);
      delay (10);
      motor0.atras(150);
      motor1.atras(150);
      delay(5);
      pararRobot();
  }
  while (cantidadDeImanesPrueba1 < 13) {
      motor0.parar();
      motor1.adelante(250);
      delay(15);
      led2.rgb('b');
      contador();
      Serial.print("contador1 = ");
      Serial.println(cantidadDeImanesPrueba1);
      //Serial.print("Girando a la derecha \n");
  }
  motor1.atras(250);
  delay(15);
  pararRobot();
  led2.rgb('a');
}

void Robot::girarIzquierda(int velocidad) {
  cantidadDeImanesPrueba0 = 0;
  cantidadDeImanesPrueba1 = 0;
  while (digitalRead (SENSOR0) == HIGH && digitalRead (SENSOR1) == HIGH) {
      motor0.adelante(255);
      motor1.adelante(255);
      delay (10);
      motor0.atras(150);
      motor1.atras(150);
      delay(5);
      pararRobot();
  }
  while (cantidadDeImanesPrueba0 < 13) {
      motor0.adelante(250);
      motor1.parar();
      delay(15);
      led1.rgb('b');
      contador();
      Serial.print("contador0 = ");
      Serial.println(cantidadDeImanesPrueba0);
      //Serial.print("Girando a la izquierda \n");
    }
    motor0.atras(250);
    delay(15);
    pararRobot();
    led1.rgb('a');
  }

void Robot::pararRobot() {
  motor0.parar();
  motor1.parar();
  led1.rgb('a');
  led2.rgb('a');
}

void Robot::frenar() {
  while (VELOCIDAD_MOTOR0 > 0 && VELOCIDAD_MOTOR1 > 0) {
      VELOCIDAD_MOTOR0 - 5;
      VELOCIDAD_MOTOR1 - 15;
      led1.rgb('r');
      led2.rgb('r');
  }
  pararRobot();
}

void Robot::frenarAvance(int velocidad, int velocidad2) {
  for(int ciclo = 0; ciclo < 35; ciclo++) {
      VELOCIDAD_MOTOR0 - 5;
      VELOCIDAD_MOTOR1 - 15;
      motor0.atras(velocidad);
      motor1.atras(velocidad2);
      delay (3);
      led1.rgb('r');
      led2.rgb('r');
  }
  pararRobot();
}

void Robot::compararYCorregir() {
  
  int diferenciaDeVelocidad = 0;
  int contador0 = cantidadDeImanesPrueba0;
  int contador1 = cantidadDeImanesPrueba1;
  


  diferenciaDeVelocidad = contador0 - contador1;
  if(velocidadFinal0 == 0 && velocidadFinal1 == 0 )
  {
    
  
        if (diferenciaDeVelocidad >= 0) {
            corregir0(diferenciaDeVelocidad);
        }
        else{
            velocidadFinal0 = VELOCIDAD_MOTOR0;
            velocidadFinal1 = VELOCIDAD_MOTOR1;
            }
  }
  else 
  {
      VELOCIDAD_MOTOR0 = velocidadFinal0;
      VELOCIDAD_MOTOR1 = velocidadFinal1;
      Serial.println("");
      Serial.print("velocidades 0 = " );
      Serial.println(VELOCIDAD_MOTOR0);
      Serial.println("");
      Serial.print("velocidades 1 = " );
      Serial.println(VELOCIDAD_MOTOR1);
  } 
  Serial.println("");
  Serial.print("rueda0 =");
  Serial.println(contador0);
  Serial.println("");
  Serial.print("rueda1 =");
  Serial.println(contador1);
  
}
void Robot::corregir0(int caso) {
  // Aumentar motor 2 lento por que el 1 es mas rapido
  float ganancia = 0.55555;
  float ganancia2 = -8;
  //Serial.println(caso);
  //velocidad = ganancia * (contador1 - contador2) + velocidad_deseada
  VELOCIDAD_MOTOR0 = ganancia * caso + VELOCIDAD_MOTOR0;
 
  if(VELOCIDAD_MOTOR0 < 130){VELOCIDAD_MOTOR0 = 130;}
  if(VELOCIDAD_MOTOR0 > 250){VELOCIDAD_MOTOR0 = 255;} 
  Serial.print("velocidad motor 0 =");
  Serial.println(VELOCIDAD_MOTOR0);
            
  VELOCIDAD_MOTOR1 = ganancia2 * (-caso) + VELOCIDAD_MOTOR1;
  
  if(VELOCIDAD_MOTOR1< 130){VELOCIDAD_MOTOR1 = 130;}
  if(VELOCIDAD_MOTOR1>250){VELOCIDAD_MOTOR1 = 255;}
  Serial.print("velocidad motor 1 =");
  Serial.println(VELOCIDAD_MOTOR1);
}

void Robot::contador() {

  if (digitalRead(SENSOR1) == LOW && contador1EnHigh == false ) 
  {
      cantidadDeImanesPrueba1++; 
      contador1EnHigh = true;
      //Serial.print ("Cantidad de imanes sensor 1="); 
      //Serial.println (cantidadDeImanesPrueba1);   
  }
  if (digitalRead(SENSOR1) == HIGH)
  {
      contador1EnHigh = false;
  }
  //delay(10);
  if (digitalRead(SENSOR0) == LOW && contador0EnHigh == false )
  {
      cantidadDeImanesPrueba0++;  
      contador0EnHigh = true;
      //Serial.print ("Cantidad de imanes sensor 0=");
       //Serial.println (cantidadDeImanesPrueba0); 
  }
  if (digitalRead(SENSOR0) == HIGH)
  {
       contador0EnHigh = false;
  }
 
}

void Robot::escucharOrdenes() {
  while(colaOrdenes.itemCount() <= CANTIDAD_ORDENES && digitalRead(pinFinOrdenes) != LOW) {
    if (digitalRead(pinAvanzar) == LOW) {
          dibujarCaritaSorprendida();          
          colaOrdenes.enqueue(1);
          led1.rgb('g');
          led2.rgb('g');
          Serial.print("Orden para avanzar en cola \n");
          delay(500);
          led1.rgb('a');
          led2.rgb('a');
          dibujarCaritaFeliz();      
    }
    if (digitalRead(pinReversa) == LOW) {
          dibujarCaritaSorprendida();           
          colaOrdenes.enqueue(2);
          led1.rgb('r');
          led2.rgb('r');
          Serial.print("Orden para retroceder en cola \n");
          delay(500);
          led1.rgb('a');
          led2.rgb('a');
          dibujarCaritaFeliz(); 
    }
    if (digitalRead(pinGiroDerecha) == LOW) {  
          dibujarCaritaSorprendida();         
          colaOrdenes.enqueue(3);
          led2.rgb('b');
          Serial.print("Orden para girar a la derecha en cola \n");
          delay(500);
          led2.rgb('a');
          dibujarCaritaFeliz(); 
    }
    if (digitalRead(pinGiroIzquierda) == LOW) {    
          dibujarCaritaSorprendida();       
          colaOrdenes.enqueue(4);
          led1.rgb('b');
          Serial.print("Orden para girar a la izquierda en cola \n");
          delay(500);
          led1.rgb('a');
          dibujarCaritaFeliz(); 
    }
  }
  if(colaOrdenes.itemCount() < 3) {
        Serial.print("Se enoja por cancelar antes de las 3 ordenes");
        dibujarCaritaEnojada();
        delay(500);
  }
  Serial.print("Se ejecutan las ordenes \n");
  Serial.print(colaOrdenes.itemCount());
  ejecutarTodasLasOrdenes();
}

void Robot::ejecutarTodasLasOrdenes() {
  int cantidadOrdenes = colaOrdenes.itemCount();
  
  for(int i=0; i<cantidadOrdenes; i++) {
        Serial.print(colaOrdenes.itemCount());
        dibujarCaritaEntusiasmada();
        
        ejecutarOrden(colaOrdenes.dequeue());
        dibujarCaritaFeliz();
        delay(1000);
  }
}


void Robot::ejecutarOrden(byte caso) {
  switch(caso) {
    case 1:
      avanzar(VELOCIDAD_MOTOR0,VELOCIDAD_MOTOR1);
      break;
    case 2:
      retroceder(VELOCIDAD_MOTOR0,VELOCIDAD_MOTOR1);
      break;
    case 3:
      girarDerecha(VELOCIDAD_MOTOR1);
      break;
    case 4:
      girarIzquierda(VELOCIDAD_MOTOR0);
      break;
  }
}
void Robot::dibujarCarita(byte ojos[8], byte boca[8]) {     
  for (int i = 0; i < 8; i++)     
  {
    matrizLeds.setRow(0,i,ojos[i]);   
  }
  for (int i = 0; i < 8; i++)     
  {
    matrizLeds.setRow(1,i,boca[i]);    
  }
}


void Robot::dibujarCaritaFeliz() { dibujarCarita(ojoFeliz,  ojoFeliz); }
void Robot::dibujarCaritaSorprendida() { dibujarCarita(ojoDefault,  ojoDefault); }
void Robot::dibujarCaritaEntusiasmada() { dibujarCarita(ojoCorazonCabeza,  ojoCorazonCabeza); }
void Robot::dibujarCaritaTriste() { dibujarCarita(ojoCerrado,  ojoCerrado); }
void Robot::dibujarCaritaEnojada() { dibujarCarita(ojoEnojadoIzqCabeza,  ojoEnojadoDerCabeza); }
void Robot::dibujarCaritaGuiniando() { dibujarCarita(ojoFeliz,  ojoCerrado); }
void Robot::despertar() {
  dibujarCarita(ojoCerrado,  ojoCerrado);
  delay(300);
  dibujarCarita(ojoDefault,  ojoDefault);
  delay(500);
  dibujarCarita(ojoCerrado,  ojoCerrado);
  delay(300);
  dibujarCarita(ojoFeliz,  ojoFeliz);
  delay(1000);
  dibujarCaritaGuiniando();
  delay(250);
  dibujarCaritaFeliz();
  delay(500);
}

//---------------------------------------------
Motor::Motor(byte pinAtras, byte pinAdelante, byte pinVelocidad) {
  this->pinAtras = pinAtras;
  this->pinAdelante = pinAdelante;
  this->pinVelocidad = pinVelocidad;
}

void Motor::iniciar() {
  pinMode(pinAtras, OUTPUT);
  pinMode(pinAdelante, OUTPUT);
  pinMode(pinVelocidad, OUTPUT);
  this->parar();
}

void Motor::adelante(int velocidad) {
  analogWrite(pinVelocidad, velocidad);
  digitalWrite(pinAtras, LOW);
  digitalWrite(pinAdelante, HIGH);
}

void Motor::atras(int velocidad) {
  analogWrite(pinVelocidad, velocidad);
  digitalWrite(pinAdelante, LOW);
  digitalWrite(pinAtras, HIGH);
}

void Motor::parar() {
  analogWrite(pinVelocidad, 0);
}


//-------------------------------------------------------
Led::Led(byte ledR, byte ledG, byte ledB) {
  this->ledR = ledR;
  this->ledG = ledG;
  this->ledB = ledB;
}

void Led::iniciar() {
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  this->rgb('a');
}


void Led::rgb(char color) {
  switch (color) {
    case 'r': //red
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, HIGH);
      break;
    case 'g': //green
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, HIGH);
      break;
    case 'y': //yellow
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, HIGH);
      break;
    case 'b': //blue
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, LOW);
      break;
    case 'a': //apagado
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, HIGH);
      break;
  }
}
