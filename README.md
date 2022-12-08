# ECE_4180_Smart_Walking_Stick
The repository contains details about the implementation of the smart walking stick for visually impaired or old people.
The main objective of the project is to develop a prototype of a smart walking stick to assist visually impaired and old people to move around as independently as possible. The smart stick has the following features:
* Haptic feedback is sent to user based on real-time measurements of obstacles from **LIDAR** and **ultrasonic senors**.
* Additionally the real time distance is also relayed to the user using TEXT-TO-SPEECH on a Raspberry Pi.
* The stick can also detect terrain differences such as sudden slope, staircases etc alerting the user with the help of accelarator or IMU sensor.
* A simple fall detection mechanism using IMU sensor data alerting that the user has encountered a fall. //checkkkk
* The stick has two modes:                                                                                                                                             
        1. **Companion mode**: Helps the user navigate with the data from above mentioned senors.                                                                       
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
11. Accelorometer

## SOFTWARE
1. C++
2. Python

## PIN LAYOUTS/WIRING

### ADAFRUIT BLUETOOTH PIN LOOKUP

| Mbed            | Adafruit BLE   |
| --------------  | -------------  |
| Gnd             | Gnd            |
| VU (5V)         | Vin (3.3-1.6V) |
| nc              | RTS            |
| Gnd             | Gnd            |
| P14 (Serial RX) | TXO            |
| P13 (Serial TX) | RXI            |

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

* Ultrasonic sensor works with the help of trigger and echo signals. 
* Initally trigger pin sends out an ultrasonic signal and the echo pin waits to receive the signal back. 
* The distance can be calculated by measuring the width of the echo distance. 
* Typically a hardware timer is needed but simple divide operation can calibrate the sensor to obtain real time measurement values.
* Wait time of ------ms is added to ensure that the device can receive longest possible time for echo to return and hence avoiding any collision between the previous and the next signal.

