/*
blink.cpp - Simple example in creating your own Arduino Library
Copyright (c) op from TMM. All right reserved.
 
*/
#include "Arduino.h"
#include "Wire.h"
#include "MPU6050_JASN.h"

MyMPU::MyMPU(){
  accCoef = 0.02f;
  gyroCoef = 0.98f;
  
}


void MyMPU::begin(){
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x6B);                           // PWR_MGMT_1 registro 6B hex
  Wire.write(0x00);                           // 00000000 para activar
  Wire.endTransmission();
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1B);                           // GYRO_CONFIG registro 1B hex
  Wire.write(0x08);                           // 00001000: 500dps
  Wire.endTransmission();
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1C);                          // ACCEL_CONFIG registro 1C hex
  Wire.write(0x10);                          // 00010000: +/- 8g
  Wire.endTransmission();
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1A);                        // LPF registro 1A hex
  Wire.write(0x03);                        // 256Hz(0ms):0x00 - 188Hz(2ms):0x01 - 98Hz(3ms):0x02 - 42Hz(4.9ms):0x03 - 20Hz(8.5ms):0x04 - 10Hz(13.8ms):0x05 - 5Hz(19ms):0x06
  Wire.endTransmission();
  lbsAcc = 4096;
  lbsAngl = 500;
  lbsAngl2 = 65.5;
  
}

void MyMPU::begin(int accOption, int angleOption){
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x6B);                           // PWR_MGMT_1 registro 6B hex
  Wire.write(0x00);                           // 00000000 para activar
  Wire.endTransmission();

  if(accOption==0){
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1C);                          // ACCEL_CONFIG registro 1C hex
  Wire.write(0x00);                          // 00000000: +/- 2g
  Wire.endTransmission();
  lbsAcc = 16384;
    }else if(accOption==1){
      Wire.beginTransmission(MPU6050_address);
      Wire.write(0x1C);                          // ACCEL_CONFIG registro 1C hex
      Wire.write(0x08);                          // 00001000: +/- 4g
      Wire.endTransmission();
      lbsAcc = 8192;
        }else if(accOption==2){
          Wire.beginTransmission(MPU6050_address);
          Wire.write(0x1C);                          // ACCEL_CONFIG registro 1C hex
          Wire.write(0x10);                          // 00010000: +/- 8g
          Wire.endTransmission();
          lbsAcc = 4096;
           }else if(accOption==3){
             Wire.beginTransmission(MPU6050_address);
             Wire.write(0x1C);                          // ACCEL_CONFIG registro 1C hex
             Wire.write(0x18);                          // 00011000: +/- 16g
             Wire.endTransmission();
             lbsAcc = 2048;
             }

 if(angleOption==0){
  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1B);                          // ACCEL_CONFIG registro 1C hex
  Wire.write(0x00);                          // 00000000: 250
  Wire.endTransmission();
  lbsAngl = 250;
  lbsAngl2 = 131;
    }else if(angleOption==1){
      Wire.beginTransmission(MPU6050_address);
      Wire.write(0x1B);                          // ACCEL_CONFIG registro 1C hex
      Wire.write(0x08);                          // 00001000: 500
      Wire.endTransmission();
      lbsAngl = 500;
      lbsAngl2 = 65.5;
        }else if(angleOption==2){
          Wire.beginTransmission(MPU6050_address);
          Wire.write(0x1B);                          // ACCEL_CONFIG registro 1C hex
          Wire.write(0x10);                          // 00010000: 1000
          Wire.endTransmission();
          lbsAngl = 1000;
          lbsAngl2 = 32.8;
          }else if(angleOption==3){
            Wire.beginTransmission(MPU6050_address);
            Wire.write(0x1B);                          // ACCEL_CONFIG registro 1C hex
            Wire.write(0x18);                          // 00011000: 2000
            Wire.endTransmission();
            lbsAngl = 2000;
            lbsAngl2 = 16.4;
          }

  accCoef = 0.02f;
  gyroCoef = 0.98f;

  Wire.beginTransmission(MPU6050_address);
  Wire.write(0x1A);                        // LPF registro 1A hex
  Wire.write(0x03);                        // 256Hz(0ms):0x00 - 188Hz(2ms):0x01 - 98Hz(3ms):0x02 - 42Hz(4.9ms):0x03 - 20Hz(8.5ms):0x04 - 10Hz(13.8ms):0x05 - 5Hz(19ms):0x06
  Wire.endTransmission();
  
}


void MyMPU::rawData(){
  
  tiempo_ejecucion = (micros() - loop_timer) / 1000;
  
  Wire.beginTransmission(MPU6050_address);       // Empezamos comunicación
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050_address, 14);         // Pedimos 14 bytes
  while (Wire.available() < 14);              // Espramos hasta recibir las informacion
 
  rawAccX = Wire.read() << 8 | Wire.read();
  rawAccY = Wire.read() << 8 | Wire.read();
  rawAccZ = Wire.read() << 8 | Wire.read();
  rawTemp = Wire.read() << 8 | Wire.read();
  rawGyroX = Wire.read() << 8 | Wire.read();
  rawGyroY = Wire.read() << 8 | Wire.read();
  rawGyroZ = Wire.read() << 8 | Wire.read();
  loop_timer = micros();
  
 
}

void MyMPU::offSet(){
    Serial.println();
    Serial.println("========================================");
    Serial.println("calculando  offsets");
    Serial.println("no muevas el equipo");
  pinMode(Led_pin,OUTPUT);
  digitalWrite(Led_pin,HIGH);
  loop_timer = micros();
  //====Calcular offSet giroscopio
  for (cal_int = 0; cal_int < 3000 ; cal_int ++) {
    rawData();            // Leemos los datos del MPU6050 3000 veces y sacamos el valor medio para obtener los offset del giroscopio
    gyro_X_cal += rawGyroX;
    gyro_Y_cal += rawGyroY;
    gyro_Z_cal += rawGyroZ;
    delayMicroseconds(1000);
  }
  gyro_X_cal = gyro_X_cal / 3000;       // valor medio de las 3000 muestras
  gyro_Y_cal = gyro_Y_cal / 3000;
  gyro_Z_cal = gyro_Z_cal / 3000;
 
  //==== Calcular offSet acelerometro



  for (cal_int = 0; cal_int < 3000 ; cal_int ++) {
    rawData();
    angle_pitch_acc_cal += rawAccX;
    angle_roll_acc_cal += rawAccY;
    angle_yaw_acc_cal += rawAccZ;
  }

  UIOX= angle_pitch_acc_cal/lbsAcc;
  UIOY = angle_roll_acc_cal/lbsAcc;
  UIOZ = angle_yaw_acc_cal/lbsAcc;

  UIOX= UIOX/3000;
  UIOY = UIOY/3000;
  UIOZ = UIOZ/3000;
  
  angle_pitch_acc_cal = angle_pitch_acc_cal / 3000;
  angle_roll_acc_cal = angle_roll_acc_cal / 3000;
  angle_yaw_acc_cal = angle_yaw_acc_cal / 3000;

  Serial.println("offSet RAW  "+String(angle_pitch_acc_cal)+"   "+String(angle_roll_acc_cal)+"   "+String(angle_yaw_acc_cal));
  Serial.println("offSets     "+String(UIOX)+"   "+String(UIOY)+"   "+String(UIOZ));
  loop_timer = micros();
  digitalWrite(Led_pin,LOW);
   
  
}




void MyMPU::procesData(){
  

  /**********Calibracion***********/
  proAccX = rawAccX-angle_pitch_acc_cal;
  proAccY = rawAccY-angle_roll_acc_cal;
  proAccZ = rawAccZ-angle_yaw_acc_cal;
  proAccZ = proAccZ + lbsAcc;

 /*******calculo aceleraciones m/s2*******/
  UIX=proAccX*(9.81/ lbsAcc);
  UIY=proAccY*(9.81/ lbsAcc);
  UIZ=proAccZ*(9.81/ lbsAcc);
  /***********************************/

  proGyroX  = rawGyroX-gyro_X_cal;
  proGyroY  = rawGyroY-gyro_Y_cal;
  proGyroZ  = rawGyroZ-gyro_Z_cal;
  
  /**********Calculo Velocidad angular deg/s***********/

  pitchGyro = (proGyroX) / lbsAngl;
  rollGyro =  (proGyroY) / lbsAngl;
  yawGyro =  (proGyroZ) /  lbsAngl;

  pitchGyro = ((float)proGyroX)*(lbsAngl/32768.0);
  rollGyro = ((float)proGyroY)*(lbsAngl/32768.0);
  yawGyro = ((float)proGyroZ)*(lbsAngl/32768.0);
  /************Calculo angulos de inclinacion*****************/
  dt = (millis()-tiempo_prev)/1000.0;
  tiempo_prev=millis();
  accel_ang_x=atan(proAccX/sqrt(pow(proAccY,2) + pow(proAccZ,2)))*(180.0/3.14);
  accel_ang_y=atan(proAccY/sqrt(pow(proAccX,2) + pow(proAccZ,2)))*(180.0/3.14);

  ang_x = 0.98*(ang_x_prev+(proGyroX/lbsAngl2)*dt) + 0.02*accel_ang_x;
  ang_y = 0.98*(ang_y_prev+(proGyroY/lbsAngl2)*dt) + 0.02*accel_ang_y;
  
  ang_x_prev=ang_x;
  ang_y_prev=ang_y;

  /**********Calculo ang*********/
  angulo_pitch += pitchGyro * tiempo_ejecucion / 1000 ;
  angulo_roll += rollGyro * tiempo_ejecucion / 1000 ;
  angulo_pitch += angulo_roll * sin((proGyroZ) * tiempo_ejecucion  * 0.000000266);      // tiempo_ejecucion/1000 /65.5 * PI/180
  angulo_roll -= angulo_pitch * sin((proGyroZ) * tiempo_ejecucion  * 0.000000266);
 
  acc_total_vector = sqrt(pow(proAccY, 2) + pow(proAccX, 2) + pow(proAccZ, 2));
  angle_pitch_acc = asin((float)proAccY / acc_total_vector) * 57.2958;     // 57.2958 = Conversion de radianes a grados 180/PI
  angle_roll_acc = asin((float)proAccX / acc_total_vector) * -57.2958;
 
  angulo_pitch = angulo_pitch * 0.99 + angle_pitch_acc * 0.01;     // Corregimos en drift con filtro complementario 
  angulo_roll = angulo_roll * 0.99 + angle_roll_acc * 0.01;        
}