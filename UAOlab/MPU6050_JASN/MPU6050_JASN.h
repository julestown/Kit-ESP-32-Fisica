/*
 blink.h - Simple example in creating your own Arduino Library
 Copyright (c) 2017 op of TMM. All right reserved.
 
 A pin is blinked automatically by one second intervals or by a specified interval
 Methods:
 
 Blink(pin) - Constructor. Specify pin to blink
 blink(value) - Enable blinking. Accepts 1, 0 or ON and OFF
 blink(value, length) - Enable blinking and specify interval of blinking.
*/
 
#ifndef MPU6050_JASN
#define MPU6050_JASN
 
#include "Arduino.h"
#include "Wire.h"
 
#define MPU6050_address 0x68
#define Led_pin 2
#define OFF false
 
class MyMPU{
public:

 MyMPU();

 void begin();
 void begin(int accOption, int angleOption);
 
 void rawData();
  int16_t  getRawAccX(){ return rawAccX; };
  int16_t  getRawAccY(){ return rawAccY; };
  int16_t  getRawAccZ(){ return rawAccZ; };
  int16_t  getTemp(){ return rawTemp; };
  int16_t  getRawGyroX(){ return rawGyroX; };
  int16_t  getRawGyroY(){ return rawGyroY; };
  int16_t  getRawGyroZ(){ return rawGyroZ; };
  
 void offSet();
  float getOffSetAX(){ return angle_pitch_acc_cal; };
  float getOffSetAY(){ return angle_roll_acc_cal; };
  float getOffSetAZ(){ return angle_yaw_acc_cal; };
  float getOffSetAnX(){ return gyro_X_cal;};
  float getOffSetAnY(){ return gyro_Y_cal;};
  float getOffSetAnZ(){ return gyro_Z_cal;};

 void procesData();
  float getProcesAccX(){return proAccX;};
  float getProcesAccY(){return proAccY;};
  float getProcesAccZ(){return proAccZ;}
  float getProcesGyroX(){return proGyroX;};
  float getProcesGyroY(){return proGyroY;};
  float getProcesGyroZ(){return proGyroZ;};
  float getPitchAngle(){return angulo_pitch;};
  float getRollAngle(){return angulo_roll;};
  float getUIX(){return UIX;};
  float getUIY(){return UIY;};
  float getUIZ(){return UIZ;};
  float getPitch(){return pitchGyro;};
  float getRoll(){return rollGyro;};
  float getYaw(){return yawGyro;};
  float getXangle(){return angulo_pitch;};
  float getYangle(){return angulo_roll;};
  float getIncX(){return ang_x;};
  float getIncY(){return ang_y;};


  
 
private:
 int16_t  rawAccX, rawAccY, rawAccZ, rawTemp,rawGyroX, rawGyroY; 
 float lbsAcc,lbsAngl,lbsAngl2;
 float angulo_pitch, angulo_roll, yawGyro, pitchGyro, rollGyro, angle_pitch_acc, angle_roll_acc, temperature, gyro_X_cal, gyro_Y_cal, gyro_Z_cal;
 float angle_yaw_acc_cal, angle_roll_acc_cal, angle_pitch_acc_cal;
 int   cal_int;
 float acc_total_vector;
 long tiempo_ejecucion, loop_timer;
 float rawGyroZ,proAccX,proAccY,proAccZ,proGyroX,proGyroY,proGyroZ;
 float UIOX,UIOY,UIOZ,UIX,UIY,UIZ;
 float accel_ang_x,accel_ang_y;
 float dt;
 long tiempo_prev;
 float accCoef;
 float gyroCoef;
 float ang_x, ang_y;
 float ang_x_prev,ang_y_prev;

};
#endif