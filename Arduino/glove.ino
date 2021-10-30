#include<Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

//MPU6050 I2C addresses: AD0=LOW => add= 0b1101000, AD0 = HIGH => add = 0b1101001
int MPU = 0b1101000;//0x68

int c = 0;
int WINDOW_SIZE = 30;

const int DPS = 250;// +/-250 deg/s
const uint8_t g = 2; // +/-2g
float acc[3];
float omega[3];
float omegaCalibrated[3];
float accCalibrated[3];

float offsetGyro[3];
float offsetAcc[3];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //setup_mpu();
  //configure_gyro(DPS);
  //configure_accelerometer(g);
  //calculate_IMU_error(DPS,g,offsetGyro,offsetAcc);
  
  //delay(20);
}

void loop() {
  // put your main code here, to run repeatedly:


  //reading flex sensors
  float flex1avg, flex2avg, flex3avg, flex4avg, flex5avg;
  int flex1, flex2, flex3, flex4, flex5;

  float sample1 = 0.0,sample2 = 0.0,sample3 = 0.0,sample4 = 0.0,sample5 = 0.0;
  float avgVal1 = 0.0, avgVal2 = 0.0, avgVal3 = 0.0, avgVal4 = 0.0, avgVal5 = 0.0;

  for(int i = 0;i<WINDOW_SIZE;i++){
    flex1 = analogRead(A0);   
    flex2 = analogRead(A1); 
    flex3 = analogRead(A2);
    flex4 = analogRead(A3);
    flex5 = analogRead(A4);

    sample1 = sample1 + flex1;
    sample2 = sample2 + flex2;
    sample3 = sample3 + flex3;
    sample4 = sample4 + flex4;
    sample5 = sample5 + flex5;
    delay(3);
  }

  avgVal1 = sample1/WINDOW_SIZE;
  avgVal2 = sample2/WINDOW_SIZE;
  avgVal3 = sample3/WINDOW_SIZE;
  avgVal4 = sample4/WINDOW_SIZE;
  avgVal5 = sample5/WINDOW_SIZE;

  //MPU6050 reading
  getAcceleration(g,&acc[0]);
  getAngularVelocities(&omega[0], DPS);

 
  for(int i = 0;i<3;i++){
    omegaCalibrated[i] = omega[i] - offsetGyro[i]/131;
    accCalibrated[i] = acc[i] - offsetAcc[i]/16384;
  }


  /* For logging purpose
 // Serial.println("\tXAccel\t\t\tYAccel\t\t\t\tZAccel\t\t\tXGyro\t\t\tYGyro\t\t\tZGyro");
  Serial.print("Flex 1: ");
  Serial.print(avgVal1);
  Serial.print("      Flex 2: ");
  Serial.print(avgVal2);
  Serial.print("      Flex 3: ");
  Serial.print(avgVal3);
  Serial.print("      Flex 4: ");
  Serial.print(avgVal4);
  Serial.print("      Flex 5: ");
  Serial.println(avgVal5);*/


  
  /*
  // 
  Serial.print("AccX: ");
  Serial.print(accCalibrated[0]);
  Serial.print("      AccY: ");
  Serial.print(accCalibrated[1]);
  Serial.print("      AccZ: ");
  Serial.println(accCalibrated[2]);

  Serial.print("OmegaX: ");
  Serial.print(omegaCalibrated[0]);
  Serial.print("     OmegaY: ");
  Serial.print(omegaCalibrated[1]);
  Serial.print("     OmegaZ: ");
  Serial.println(omegaCalibrated[2]);  
*/
  
// String sendData = String(avgVal1)+","+String(avgVal2)+","+String(avgVal3)+","+String(avgVal4)+","+String(avgVal5);

  // Serial.println(sendData);// Serial.println(",");
  Serial.println(avgVal2); Serial.println(",");
  Serial.println(avgVal3); Serial.println(",");
  Serial.println(avgVal4); Serial.println(",");
  Serial.println(avgVal5); Serial.print(","); 
  Serial.print(accCalibrated[0]); Serial.print(",");
  Serial.print(accCalibrated[1]); Serial.print(",");
  Serial.print(accCalibrated[2]); Serial.print(",");
  Serial.print(omega[0]); Serial.print(",");
  Serial.print(omega[1]); Serial.print(",");
  Serial.print(omega[2]); Serial.println(";");
  
  delay(3000);
}




void setup_mpu()
{
  Wire.beginTransmission(MPU); // start communication
  Wire.write(0x6B); // Access power mngmnt reg
  Wire.write(0x00); // set to sleep
  Wire.endTransmission();// end transmission
 }

 /* Gyro configuration reg: 1B
  *  set sensitivity using AFS_SEL
  *  AFS_SEL    |     Full scale range   |   Sensitivity scale factor
  *   0         |       +/- 250deg/sec   |          131
  *   1         |       +/- 500deg/sec   |         65.5
  *   2         |       +/-1000deg/sec   |         32.8
  *   3         |       +/-2000deg/sec   |         16.4
 */
void configure_gyro(uint8_t dps)
 {
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  
  if(dps == 250)
    Wire.write(0x00);
  else if( dps == 500)
    Wire.write(0x01);
  else if(dps == 1000)
    Wire.write(0x02);
  else if(dps == 2000)
    Wire.write(0x03);
  else
    Serial.println("gyro configuration error: incorrect Full Scale Range");
 
  Wire.endTransmission();
  }
/* Accelerometer configuration  reg: 1C
 *  set sensitivity using FS_SEL
*    FS_SEL     |     Full scale range   |   Sensitivity scale factor
  *   0         |       +/- 2g           |         16384
  *   1         |       +/- 4g           |         8192
  *   2         |       +/-8g            |         4096
  *   3         |       +/-16g           |         2048
*/
void configure_accelerometer(uint8_t g)
{
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  
  if(g == 2)
    Wire.write(0x00);
  else if( g == 4)
    Wire.write(0x01);
  else if(g == 8)
    Wire.write(0x02);
  else if(g == 16)
    Wire.write(0x03);
  else
    Serial.println("accelerometer configuration error: incorrect Full Scale Range");
  
  
  Wire.endTransmission();
}
/* Access gyro readings buffer reg: 0x43
*/
void getGyroVal(int *rawGyro )
{
  //int rawGyro[3];
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU,6);  // request for 6 bytes from gyro reg (43-48)
  
  while(Wire.available()<6);
  
  
  rawGyro[0] = Wire.read()<<8 | Wire.read();
  rawGyro[1] = Wire.read()<<8 | Wire.read();
  rawGyro[2] = Wire.read()<<8 | Wire.read();
  
  }

void getACCVal(int *rawACC)
{
  //int rawACC[3];
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(MPU,6);  // request for 6 bytes from gyro reg (43-48)
  
  while(Wire.available()<6);
  
  
  rawACC[0] = Wire.read()<<8 | Wire.read();
  rawACC[1] = Wire.read()<<8 | Wire.read();
  rawACC[2] = Wire.read()<<8 | Wire.read();
  
  
  //return rawACC;
  }


void getAngularVelocities(float *angularVelocity, uint8_t dps)
{
  int rawGy[3];  
  getGyroVal(&rawGy[0]);

  //float[3] angularVelocity;
  
  
  if(dps == 250){
    angularVelocity[0] = rawGy[0]/131.0;
    angularVelocity[1] = rawGy[1]/131.0;
    angularVelocity[2] = rawGy[2]/131.0;
    }
  else if( dps == 500){
    angularVelocity[0] = rawGy[0]/65.5;
    angularVelocity[1] = rawGy[1]/65.5;
    angularVelocity[2] = rawGy[2]/65.5;
    }
  else if(dps == 1000){
    angularVelocity[0] = rawGy[0]/32.8;
    angularVelocity[1] = rawGy[1]/32.8;
    angularVelocity[2] = rawGy[2]/32.8;
    }
  else if(dps == 2000){
    angularVelocity[0] = rawGy[0]/16.4;
    angularVelocity[1] = rawGy[1]/16.4;
    angularVelocity[2] = rawGy[2]/16.4;
    }
  else
    Serial.println("gyro configuration error: incorrect Full Scale Range");

  }

void getAcceleration(uint8_t g,float *acceleration)
{
  int rawAcc[3];
  getACCVal(&rawAcc[0]);

  //float[3] acceleration;
  
  
  if(g == 2){
    acceleration[0] = rawAcc[0]/16384.0;
    acceleration[1] = rawAcc[1]/16384.0;
    acceleration[2] = rawAcc[2]/16384.0;
    }
  else if( g == 4){
    acceleration[0] = rawAcc[0]/8192.0;
    acceleration[1] = rawAcc[1]/8192.0;
    acceleration[2] = rawAcc[2]/8192.0;
    }
  else if(g == 8){
    acceleration[0] = rawAcc[0]/4096.0;
    acceleration[1] = rawAcc[1]/4096.0;
    acceleration[2] = rawAcc[2]/4096.0;
    }
  else if(g == 16){
    acceleration[0] = rawAcc[0]/2048.0;
    acceleration[1] = rawAcc[1]/2048.0;
    acceleration[2] = rawAcc[2]/2048.0;
    }
  else
    Serial.println("Accelerometer configuration error: incorrect Full Scale Range");
 //return acceleration;
}



void calculate_IMU_error(int dps,int g, float *offsetGyro,float *offsetAcc) {
  int rawGyro[3];
  int rawAcc[3];
  float FSRGyro,FSRAcc;


  float AccErrorX, AccErrorY, AccErrorZ;
  float GyroErrorX, GyroErrorY, GyroErrorZ;
  float GyroX, GyroY, GyroZ;
  float AccX, AccY, AccZ;

  
  if(dps == 250)
    FSRGyro = 131.0;
  else if(dps == 500)
    FSRGyro = 65.5;
  else if(dps == 1000 )
    FSRGyro = 32.8;
  else if(dps == 2000)
    FSRGyro = 16.4;
  else
    Serial.println(""); 

  if(g == 2)
    FSRAcc = 16384.0;
  else if(g == 4)
    FSRAcc = 8192.0;
  else if(g == 4)
    FSRAcc = 4096.0;
  else if(g == 4)
    FSRAcc = 2080.0;
  else
    Serial.println("");
  
  while (c < 200) {
   /* Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings*/
    getACCVal(&rawAcc[0]);
    AccX = rawAcc[0]/FSRAcc;
    AccY = rawAcc[1]/FSRAcc;
    AccZ = rawAcc[2]/FSRAcc;
   // AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
   // AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    //AccErrorZ??
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccX = AccX / 200;
  AccY = AccY / 200;
  AccZ = AccZ / 200;
  offsetAcc[0] = AccX;
  offsetAcc[1] = AccY;
  offsetAcc[2] = AccZ;
  //AccErrorZ = AccErrorZ / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
   /* Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();*/
    
    // Sum all readings
    getGyroVal( &rawGyro[0]);
    GyroX = rawGyro[0];
    GyroY = rawGyro[1];
    GyroZ = rawGyro[2];
    
    //GyroErrorX = GyroErrorX + (GyroX / FSRGyro);
    //GyroErrorY = GyroErrorY + (GyroY / FSRGyro);
   // GyroErrorZ = GyroErrorZ + (GyroZ / FSRGyro);
    c++;
  }

  GyroX = GyroX / 200;
  GyroY = GyroY / 200;
  GyroZ = GyroZ / 200;

  offsetGyro[0] = GyroX;
  offsetGyro[1] = GyroY;
  offsetGyro[2] = GyroZ;
 }