#include <iostream>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "kcmpu6050.h"

#define I2C_PORT i2c1


int main(int argc, char** argv){

    stdio_init_all();

    sleep_ms(1000);

    int start = 0;
    printf("Starting MPU Progarm\n");
    while(start == 0){
        printf("Please Enter a number different from 0 to start :\n");
        scanf("%d",&start);
        sleep_ms(1000);
    }
    
    MPU6050 MPU;
    

    if(!MPU.mpu_init(2,3))
        return -1;

    //Choosing Low pass filter
    if(!MPU.mpu_DLPF(7))
        return -1;

    if(!MPU.mpu_GYRO_CONFIG(1))
        return -1;
    
    if(!MPU.mpu_ACCEL_CONFIG(1))
        return -1;

    while (true)
    {
        MPU.get_gyro();
        std::cout << "The Gyro Scope Data for (X,Y,Z) in degree/s is (" << MPU.GyroX << "," << MPU.GyroY << "," << MPU.GyroZ << ") degree/s\n";
        MPU.get_accel();
        std::cout << "The Accelerometer Data for (X,Y,Z) in g is (" <<MPU.AccelX << "," << MPU.AccelY <<"," << MPU.AccelZ << ") g\n";
        sleep_ms(500);
    }
    
}