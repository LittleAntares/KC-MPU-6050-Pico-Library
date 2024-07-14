#ifndef KCMPU6050_H_
#define KCMPU6050_H_


#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "kcmpu6050.h"
#include <iostream>



extern i2c_inst_t* I2C_PORT;


class MPU6050 {
    // List of Addresses
    const uint8_t addr = 0x68;
    const uint8_t WHO_AM_I = 0x75;
    const uint8_t DLPF = 0x1A;
    const uint8_t GYRO_CONFIG = 0x1B;
    const uint8_t GYRO_OUT = 0x43;
    const uint8_t PWR_MGMT_1 = 0x6B;
    const uint8_t ACCEL_CONFIG = 0x1C;
    const uint8_t ACCEL_OUT = 0x3B;

    // Setting LSB Value to config of 0;
    float gyro_lsb = 131.0f;
    float accel_lsb = 16384.0f;

    // Raw values
    int16_t GyroRawX = 0, GyroRawY = 0, GyroRawZ = 0;
    int16_t AccelRawX = 0, AccelRawY = 0, AccelRawZ = 0;
    
public:
    // Setting Output values
    float GyroX = 0, GyroY = 0, GyroZ = 0;
    float AccelX = 0, AccelY = 0, AccelZ = 0;

    // Constructor
    MPU6050() = default;

    // Member functions
    int mpu_init(int sda, int sdl); // Setting up Pin and Checking if the device is connected
    int mpu_DLPF(uint8_t dlpf_cfg); // Setting up Low pass filter
    int mpu_GYRO_CONFIG(uint8_t fs_sel); // Setting up GYRO_CONFIG
    int mpu_ACCEL_CONFIG(uint8_t afs_sel); // Setting up ACCEL_CONFIG
    void get_gyro(); // Getting Gyro Data
    void get_accel(); // Getting Acceleration data
    void power_mode(); // Setting Power Mode
};

#endif // KCMPU6050_H_
