#include <MPU6050_JASN.h>

//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`



 

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
MyMPU myMPU;



int Led_built=2;
boolean led = false;
int comando=0;
String comand="";

  float accx=0;
  float accy=0;
  float accz=0;
  float photoD=0;
  float gravity =0;
  float distancia=0;
  float tiempo1=0;
  float tiempo2=0;
  float dTiempo=0;
  boolean expe=true;
  double millis1=0.000;
  double millis2=0.000;



void setup() {

  //inicializa bluetooth
  Serial.begin(115200);
  SerialBT.begin("UAOlab"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


  //inicializa giroscopio
  Wire.begin(22, 21);
  myMPU.begin();
  myMPU.offSet();
  myMPU.rawData();
  myMPU.procesData();
  gravity=myMPU.getUIZ();
  accx=myMPU.getUIX();
  Serial.println(" gravity  "+String(gravity)); 
  digitalWrite (2, HIGH);
  

  //inicializa led interno y lo prende y apaga
  pinMode (Led_built,OUTPUT);
  digitalWrite(Led_built,HIGH);
  delay(500);
  digitalWrite(Led_built,LOW);

}

void loop() {

  if (SerialBT.available()){
    comando=SerialBT.read();
    comand =String(comando);
    Serial.println(comand);
   if(comando==71){  
      millis1=millis();
      envioDatos(0);      
    }else if(comando==2){
     comprobarBlue();
    }
    comando=0;
  }
  
  
 
}

 void comprobarBlue(){
    
      if(led==false){
       digitalWrite(Led_built,HIGH);
       led=true;
       //SerialBT.print("prendido");
       }else{
        digitalWrite(Led_built,LOW);
       led=false;
       //SerialBT.print("apagado");
       }     
  
}


void envioDatos(double millis3){
  do{   
  myMPU.rawData();
  myMPU.procesData();
  accx=myMPU.getUIX();
  accy=myMPU.getUIY();
  accz=myMPU.getUIZ();
  photoD=73;
  float millis4=millis3/1000.000;
  SerialBT.print("{\"gravedad\":"+String(gravity)+",\"accX\":"+String(accx)+",\"accY\":"+String(accy)+",\"accZ\":"+String(accz)+",\"photoD\":"+String(photoD)+",\"tiempo\":"+String(millis4)+"}");
  Serial.println("{\"gravedad\":"+String(gravity)+",\"accX\":"+String(accx)+",\"accY\":"+String(accy)+",\"accZ\":"+String(accz)+",\"photoD\":"+String(photoD)+",\"tiempo\":"+String(millis4)+"}");
  millis3=millis()-millis1;
  
   if (SerialBT.available()){
    comando=SerialBT.read();
  }
 SerialBT.flush();
  delay(60);
   
  
}while(comando==71);
  
}
