# ECE_4180_Smart_Walking_Stick
The repository contains details about the implementation of the smart walking stick for visually impaired or old people.
The main objective of the project is to develop a prototype of a smart walking stick to assist visually impaired and old people to move around as independently as possible. The smart stick has the following features:
* Haptic feedback is sent to user based on real-time measurements of obstacles from **ultrasonic sensors**.
* Additionally the real time distance is also conveyed to the user using TEXT-TO-SPEECH on a Raspberry Pi.
* The stick can also detect terrain differences such as sudden slope, staircases etc alerting the user with the help of accelerator or IMU sensor.
* A simple fall detection mechanism using IMU sensor data ,alerting using a speaker when the user encounters a fall. 
* The stick has two modes:                                      
                                                                                                      
        1. **Companion mode**: Helps the user navigate with the data from above mentioned sensors.                                                                      
        2. **Location mode** : Continuously emits a sound via speaker when the user presses the key via Bluefruit Connect.
        
## HARDWARE COMPONENTS
1. Mbed - LPC1768
2. Raspberry Pi 4B
3. Ultrasonic sensor
4. Adafruit Bluetooth module
6. LSM9DS1 IMU sensor
7. DC Motor
8. MOSFET motor driver
9. Buzzer/speaker
10. Class D amplifier


## SOFTWARE
1. Embedded C
2. Python
	




## BLOCK DIAGRAM

## WORKING/ OPERATION

###  ULTRASONIC SENSOR 

Ultrasonic Sensor Pin Lookup


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
* Wait time of 500 ms is added to ensure that the device can receive longest possible time for echo to return and hence avoiding any collision between the previous and the next signal.
* We have used two ultrasonic sensors in our project. One is to detect the obstacles in front of the user and the other sensor is used as a depth sensor which can detect stairs/holes in the path.


#### Photo:
<p align="center">
  <img src="Images/Ultrasonic_sensor.jpeg" width="250"/>  
</p>

#### Code Snippet:

<code>float distance_ultra; </code> <br />
<code> void dist(int distance)</code> <br />
<code>{</code> <br />
<code>    distance_ultra = distance*32*30/10000;</code> <br />
<code>}</code> <br />
<code>ultrasonic mu(p6, p7, .1, 1, &dist);</code> <br />   
<code>void thread3(void const* args){ </code> <br />
<code>    { </code> <br />  
<code>       mu.checkDistance();   </code> <br /> 
<code>        Thread::wait(500); </code> <br />  
<code>    } </code> <br />  



### ADAFRUIT Bluetooth Module

Adafruit Bluetooth  Pin Lookup

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

#### Photo:
<p align="center">
  <img src="Images/Adafruit.jpeg" width="250"/>  
</p>

#### Code Snippet:
<code>  if (blue.readable()) {  </code> <br />
<code>            Serial_mutex.lock();</code><br />
<code>            if (blue.getc() == '!') {</code><br />
<code>                if (blue.getc() == 'B') { //button data</code><br />
<code>                    bnum = blue.getc(); //button number</code><br />
<code>                }}</code><br />           
<code>           if (bnum == '4') {</code><br />
<code>               locate_mode = true;</code><br />
<code>            }</code><br />
<code>            else if (bnum == '3')</code><br />
<code>            {</code><br />
<code>               locate_mode = false;</code><br />
<code>           }</code><br />
<code>            Serial_mutex.unlock(); Thread::wait(700);</code><br />
<code>        }</code><br />
<code>        if (locate_mode == true)</code><br />
<code>            buzzer = true;</code><br />
<code>        else</code><br />
<code>            buzzer = false;</code><br />
<code>    }}</code><br />




### LSM9DS1 IMU sensor

LSM9DS1 I2C Pin Lookup

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
*  We have used IMU sensor for fall detection. When there is high acceleration , that is sudden change in velocity, we will activate the Digital Out Pin p24 where buzzer is connected, indicating that stick/person has fallen.

#### Photo:
<p align="center">
  <img src="Images/IMU.jpeg" width="250"/>  
</p>

#### Code Snippet:
<code> LSM9DS1 imu(p28, p27, 0xD6, 0x3C);</code> <br />
<code>imu.readAccel();</code> <br />
<code>float thresh_mag = 1.2;</code> <br />
<code>float ax = imu.calcAccel(imu.ax);</code> <br />
<code>float ay = imu.calcAccel(imu.ay);</code> <br />
<code>float az = imu.calcAccel(imu.az);</code> <br />
<code>float accel_mag = ax * ax + ay * ay + az * az;</code> <br />
<code>if (accel_mag > thresh_mag) </code> <br />
<code>        {</code> <br />
<code>            has_fallen = true;</code> <br />
<code>       }</code> <br />
<code>        else</code> <br />
<code>            has_fallen = false;</code> <br />



### MOSFET DRIVER 
Mosfet Driver Pin Lookup
|Mbed	  | MOSFET PCB     | External Device  |
|------   | -----------    | ---------------  |
| gnd	  | JP2-2 gnd	   |                  |
| 5V	  | JP2-1 RAW	   |                  |
| P23     | JP2-3 Control  |	              |
|         |	JP1-1      |   Device(neg)    |
|     	  |     JP1-2      |   Device(pos)    |

*  p23 digital out pin can be used for control, connects to the control input of the driver circuit. 
* Mbed can only supply about 200mA of current for external devices via USB power, so an external DC power supply is needed for large loads.
* MOSFET has a very low gate input voltage that works with 3.3V logic signals like those on mbed.
* MOSFET driver ICs such as the LTC1155 use a charge pump circuit to drive the gate voltage higher on higher voltage MOSFET driver circuits using a normal digital logic level control signal (i.e., useful when load voltage (RAW in schematic) is larger then the logic supply voltage). 

#### Photo:
<p align="center">
  <img src="Images/Mosfet_Driver.jpeg" width="250"/>  
</p>



### BUZZER AND CLASS D AMPLIFIER 
Buzzer and Class D amplifier Pin Lookup
|Mbed	             |TPA2005D1             |Speaker
|-----------------   |-----------------     |------------
|Gnd	             |pwr - (gnd), in -     |	
|Vout (3.3V) or 5V   |pwr +	            |
|p24 (PWM)           |in +	            |
|                    |out+	            | +
|                    |out	-	    | -

Buzzer: 

*  p24 digital pin is connected to buzzer.
*  Buzzer is used for two applications in our prototype. a) For fall detection and b) Locating the stick.
*  When sudden fall is detected with the help of values from IMU, the buzzer is set to 1 - Fall detection
*  8-button control game pad in bluefruit App is used to switch on/off the buzzer. The sound emitted by the buzzer can be used to locate the stick.

#### Photo:
<p align="center">
<img src="Images/Buzzer.jpeg" width="250"/> 
</p>

####  Code Snippet:

<code>if (buzzer == true || has_fallen == true)</code> <br />
<code>        {</code> <br />
<code>            buzz = 0.5;</code> <br />
<code>            Thread::wait(500);</code> <br /></code> <br />
<code>            buzz = 0.0;</code> <br />
<code>        }</code> <br />
<code>        else</code> <br />
<code>            buzz = 0;</code> <br />
<code>    }</code> <br />


TPA2005D1 Class D Audio Amplifier

*   TI TPA2005D1 is a 1.4-W mono filter-free class D audio power amplifier with a differential input.
*   Class D amplifiers use PWM for power efficiency. It uses a 250 kHz PWM output with an H-bridge output driver to drive a small speaker.
*   The audio passes through a simple low-pass filter into the loudspeaker. The high-frequency pulses are blocked. 
*   The pairs of output transistors never conduct at the same time, there is no other path for current flow apart from the low-pass filter/loudspeaker,due to this    efficiency can exceed 90%. 

<p align="center">
<img src="Images/D Amplifier.jpeg" width="250"/>
</p>

### Raspberry Pi 4 Model B 


*   We have used Raspberry Pi 4 for text to speech with the help of PYTTX and espeak.
*   The real time sonar distance is sent as a voice signal to user through earphones after delay of every 10 secs.
<p align="center">
<img src="Images/Raspbery Pi 4 with Sonar.jpeg" width="250"/>
</p>

### Motor

*   We have used motor to give haptic feedback based on the distance measured by the depth measuring sonar.
*   The duty cycle of PWM signal which drives the motor increases with increase in the difference between the distance measured by the sonar and threshold distance.
*   So, as the distance increases, the speed at which motor rotates increases.
<p align="center">
<img src="Images/Motor.jpeg" width="250"/>
</p>


<code>   void thread4(void const* args)</code> <br />
<code>{</code> <br />
<code>   while (1)</code> <br />
<code>   {</code> <br />
<code>       if (distance_ultra > 40 && distance_ultra < 100)</code> <br />
<code>        {</code> <br />
<code>            vibration = (distance_ultra - 40) / 60; </code> <br /> 
<code>            mymotor = vibration; </code> <br />
<code>        }</code> <br />
<code>        else</code> <br />
<code>        {</code> <br />
<code>            vibration = 0;</code> <br />
<code>            mymotor = vibration;</code> <br />
<code>        }</code> <br />
<code>        Thread::wait(1000);</code> <br />
<code>    }}}</code> <br />

























