#include "mbed.h"
#include "rtos.h"
#include "stdio.h"
#include "ultrasonic.h"
#include "Motor.h"
#include "LSM9DS1.h"



LSM9DS1 imu(p28, p27, 0xD6, 0x3C); // Variable declaration for IMU Sensor
bool has_fallen = false;

RawSerial blue(p13, p14); // Variable declaration for Bluetooth module

// Mutex Declaration
Mutex i2c_mutex;   
Mutex Serial_mutex;

#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27

Serial pc(USBTX, USBRX); //For debug

Timer dt;
float vibration = 0.5;
float distance_ultra;

int status;
bool locate_mode = 0;

float prev_value = 0.0;
float curr_value = 0.0;

// Below function calibrates the Ultra sonic sensor output to a reasonable value and required unit of measurement
void dist(int distance)
{
    distance_ultra = distance * 32 * 30 / 10000;
}

ultrasonic mu(p6, p7, .1, 1, &dist);    //Set the trigger pin to p6 and the echo pin to p7

PwmOut mymotor(p23); //Motor connected to pin 23
PwmOut buzz(p24); //Buzzer connected to pin 24

bool buzzer = false;


void thread1(void const* args) // This thread is measure distance from Ultrasonic sensor after every 0.5 s
{
    while (1)
    {
        mu.checkDistance();     //call checkDistance() as much as possible, as this is where
        Thread::wait(500);
    }
}

void thread2(void const* args) 
{ // This thread assigns a PWM output which is scaled based on the distance between ultrasonic sonic sensor and obstacle to the motor
  // This thread run after every 1 sec
    while (1)
    {
        if (distance_ultra > 40 && distance_ultra < 100)
        {
            vibration = (distance_ultra - 40) / 60;// Calibrating value for haptic feedback
            mymotor = vibration;  // to be based on distance  
        }
        else
        {
            vibration = 0;
            mymotor = vibration;
        }
        Thread::wait(1000);
    }
}


void thread3(void const* args)
{   // This thread waits for input command from Bluefruit app, based on the input it will switch on/off the buzzer
    char bnum;
    while (true)
    {
        if (blue.readable()) {
            Serial_mutex.lock();
            if (blue.getc() == '!') {
                if (blue.getc() == 'B') { //button data
                    bnum = blue.getc(); //button number
                }
            }
            if (bnum == '4') {
                locate_mode = true;
            }
            else if (bnum == '3')
            {
                locate_mode = false;
            }
            Serial_mutex.unlock(); Thread::wait(700);
        }
        if (locate_mode == true)
            buzzer = true;
        else
            buzzer = false;
    }

}

int main() {

    mu.startUpdates();//start measuring the distance
    pc.baud(9600);
    imu.begin();

    int status;
    if (!imu.begin()) {
        pc.printf("Failed to start IMU.");
    }
    else {
        imu.calibrate();
        buzz = 0;
    }

    Thread t1(thread1); //start thread1
    Thread t2(thread2); //start thread2
    Thread t3(thread3); //start thread3

    while (1)
    {
        float thresh_mag = 1.6;        
        imu.readAccel();
        float ax = imu.calcAccel(imu.ax);
        float ay = imu.calcAccel(imu.ay);
        float az = imu.calcAccel(imu.az);

        float accel_mag = ax * ax + ay * ay + az * az;
        
        if (accel_mag > thresh_mag) // If there is a acceleration beyond the threshold magnitude, fall is detected
        {
            has_fallen = true;

        }
        else
            has_fallen = false;

        Thread::wait(1000);

        // Buzzer is turned on if Locate mode is enabled or if fall is detected
        if (buzzer == true || has_fallen == true)
        {
            buzz = 0.5;
            Thread::wait(500);
            buzz = 0.0;
        }
        else
            buzz = 0;
    }

}