# Ethiopian Sign Language to Speech translator

<p>The translator system has a pair gloves that are equipped with flex sensors, MPU6050(accelerator + gyroscope), Arduino Uno microcontrollers and HC-05 Bluetooth modules. The gesture data captured by the sensors is streamed to mobile device via the bluetooth module. Then, android application installed on the mobile device performs gesture classification and speech synthesis.</p>

## Hardware

#### Flex Sensor
Flex sensor is a transducer that measures the degree of deflection or curvature of the sensor. It is made of carbon and plastic in such a way that when the sensor is bent, the resistance of a carbon surface on plastic changes accordingly. The change in resistance is directly proportional to the amount of bend. This makes it a better candidate for applications requiring measurement of bend angles such as security system, rehabilitation, human-computer interface. Flex sensor's operating voltage is in the range from 0-5v. It can function on low voltages too. And the operating temperature ranges from -45 to +80 oc. The reading of the flat surface resistance is 25K ohm. The bending of the surface will range from 45K-125K ohm.

#### MPU6050
The MPU6050 is a sensor usually used for motion detecting and processing devices. It has six-dimension motions tracking a device's motion and position. It is a MEMS (micro electromechanical sensor) based sensor having a 3-axis accelerometer and digital motion processor integrated on the chip. It is available as a small 4x4x0.9 mm package weighing 2.1g and having the MEMS structure sealed and bonded at wafer level. MPU6050 operates on power supply of 3v-5v and 3.6-3.9mA current when its six motion sensing axes and DMP are in motion. It uses I2C protocol for communication and transfer of data. The I2C is a bidirectional two-wire bus used to send data between integrated circuits. It consists of three data transfer speeds which are standard, fast-mode and high-speed mode.

#### HC-05
Hc-06 is a serial Bluetooth module used for Arduino and other microcontrollers. It operates in the voltage range of 4V to 6v and a 30mA current. It is TTL compatible and works with Serial Communication (USART). In addition to these, it follows the IEEE 802.15.1 standardized protocol. It can operate in Master, Slave,
Slave or Master/Slave mode. It can also be easily interfaced with Laptop or Mobile phones with Bluetooth access. The supported baud rates include 9600,19200,38400,57600,115200,230400,460800 used for synchronizing devices to be connected.

## Software
<p>Random Forest(RF) and SVM based classifiers are used to identify and classify gesture data. To train these models, gestire data is first gathered and cleaned. Then, on this dataset the RF classifier and three variations of SVM classifiers(polynomial, polynomial-kernel and gaussian-rbf-kernal) are trained and evaluated.  Finally, the trained models are exported and loaded to [android application](https://github.com/yamlakyam/Enawra-Sign-Language-Assistance-App) to perform real-time translation.</p>



