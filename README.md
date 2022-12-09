# ECE_4180_Smart_Walking_Stick
The repository contains details about the implementation of the smart walking stick for visually impaired or old people.
The main objective of the project is to develop a prototype of a smart walking stick to assist visually impaired and old people to move around as independently as possible. The smart stick has the following features:
* Haptic feedback is sent to user based on real-time measurements of obstacles from **LIDAR** and **ultrasonic senors**.
* Additionally the real time distance is also relayed to the user using TEXT-TO-SPEECH on a Raspberry Pi.
* The stick can also detect terrain differences such as sudden slope, staircases etc alerting the user with the help of accelarator or IMU sensor.
* A simple fall detection mechanism using IMU sensor data alerting that the user has encountered a fall. //checkkkk
* The stick has two modes:                                                                                                                                             
        1. **Companion mode**: Helps the user navigate with the data from above mentioned sensors.                                                                      
        2. **Location mode** : Continuously emits a sound via speaker when the user presses the key via Bluefruit Connect.
        
## HARDWARE COMPONENTS
1. Mbed - LPC1768
2. Raspberry Pi 4B
3. Ultrasonic sensor
4. Adafruit Bluetooth module
5. TOF sensor
6. Accelorometer
7. Servo motor
8. MOSFET motor driver
9. Buzzer/speaker
10. Class D amplifier


## SOFTWARE
1. C++
2. Python

## PIN LAYOUTS/WIRING



### LIDAR TOF PIN LOOKUP

| Mbed     | TOF VL53L0X  |
| -------  | ------------ |
| 3.3 v    | VIN          |
| GND      |  GND         |
| P28      | SDA          |
| P27      | SCL          |
| P26      | SHDN         |

### MOSFET AND SERVO PIN LOOKUP
|Mbed	  | MOSFET PCB   | External Device  |
|------ | -----------  | ---------------  |
| gnd	  | JP2-2 gnd	   |                  |
| 5V	  | JP2-1 RAW	   |                  |
| P23   | JP2-3 Control|	                |
|       |	JP1-1        |   Device(neg)    |
|     	| JP1-2        |   Device(pos)    |

### ULTRASONIC SENSOR PIN LOOKUP
| Mbed     | HC-SR04  |
| -------  | -------- |
| Vu (5V)	 | Vcc      | 
| Gnd	     | Gnd      |
| p6	     | trig     |
| p7	     | echo     |
### BUZZER AND CLASS D AMPLIFIER PIN LOOKUP
|Mbed	             |TPA2005D1         |Speaker
|----------------- |----------------- |------------
|Gnd	             |pwr - (gnd), in - |	-
|Vout (3.3V) or 5V |pwr +	            |
|p24 (PWM)         |in +	            |
|                  |out+	            | +
|                  |out	-	            |
	
### ACCELEROMETER



## BLOCK DIAGRAM

## WORKING/ OPERATION

1. ULTRASONIC SENSOR

<div style="display:flex">
     <div style="flex:1;padding-right:10px;">
          <img src="![image](https://user-images.githubusercontent.com/117770245/206371733-b27621d3-05ea-4ea0-a681-be7d265ae249.png)
" width="200"/>
     </div>
     <div style="flex:1;padding-left:10px;">
          <img src="![image](https://user-images.githubusercontent.com/117770245/206371854-1319f57e-602a-48d7-bd19-ae6aa09e3bdd.png)
" width="300"/>
     </div>
</div>
###  ULTRASONIC SENSOR PIN LOOKUP

| Mbed     | HC-SR04  |
| -------  | -------- |
| Vu (5V)	 | Vcc      | 
| Gnd	     | Gnd      |
| p6	     | trig     |
| p7	     | echo     |

* Ultrasonic sensor works with the help of trigger and echo signals. 
* Initally trigger pin sends out an ultrasonic signal and the echo pin waits to receive the signal back. 
* The distance can be calculated by measuring the width of the echo distance. 
* Typically a hardware timer is needed but simple divide operation can calibrate the sensor to obtain real time measurement values.
* Wait time of ------ms is added to ensure that the device can receive longest possible time for echo to return and hence avoiding any collision between the previous and the next signal.
* We have used two ultrasonic sensors in our project. One is to detect the obstacles in front of the user and the other sensor is used as a depth sensor which can detect stairs/holes in the path.


2. ADAFRUIT Bluetooth Module

### ADAFRUIT BLUETOOTH PIN LOOKUP

| Mbed            | Adafruit BLE   |
| --------------  | -------------  |
| Gnd             | Gnd            |
| VU (5V)         | Vin (3.3-1.6V) |
| nc              | RTS            |
| Gnd             | Gnd            |
| P14 (Serial RX) | TXO            |
| P13 (Serial TX) | RXI            |

* The Adafruit Bluefruit BLE board features an ARM Cortex M0 core running at 16MHz with 256KB flash memory and 32KB SRAM
* The Bluefruit LE UART Friend makes it easy to add Bluetooth Low Energy connectivity
to anything with a hardware or software serial port.
* Bluefruit App has a color picker, quaternion/accelerometer/gyro/magnetometer or location (GPS) (), and an 8-button control game pad ().
* We have used 8-button control game pad for locate mode for our project. Through bluetooth we will give command to switch on/off buzzer.

3. LSM9DS1 IMU sensor

### LSM9DS1 I2C PIN LOOKUP

| Pin on Breakout | Pin on Mbed    |
| --------------  | -------------  |
| Gnd             | Gnd            |
| VDD	          | Vout           |
| SDA	          | p9 (SDA)       |
| SCL             | p10 (SCL)      |

Brief on I2C

*   Inter-Integrated Circuit (I2C) Protocol is a protocol intended to allow multiple "peripheral" digital integrated circuits ("chips") to communicate with one or more "controller" chips.
*  It is only intended for short distance communications within a single device.
*  It only requires two signal wires to exchange information. SDA (Serial Data) is the data signal and SCL (Serial Clock) is the clock signal. 
*  Each signal line has a pull-up resistor on it, to restore the signal to high when no device is asserting it low.
*  I2C  can be used with short runs of wire (2-3m)

LSM9DS1 IMU sensor

*  The LSM9DS1 is motion-sensing soc. It has a 3-axis accelerometer, 3-axis gyroscope, and 3-axis magnetometer and nine degrees of freedom (9DOF).
*  It measures its acceleration in g's, and its scale can be set to either ± 2, 4, 8, or 16 g.
*  It measure three key properties of movement – angular velocity, acceleration, and heading.
*  We have used IMU sensor for fall detection. When there is high acceleration , that is sudden change in velocity, we will activate the buzzer, indicating that stick/person has fallen.

### MOSFET AND MOTOR LOOKUP
|Mbed	  | MOSFET PCB     | External Device  |
|------   | -----------    | ---------------  |
| gnd	  | JP2-2 gnd	   |                  |
| 5V	  | JP2-1 RAW	   |                  |
| P23     | JP2-3 Control  |	              |
|         |	JP1-1      |   Device(neg)    |
|     	  | JP1-2          |   Device(pos)    |












