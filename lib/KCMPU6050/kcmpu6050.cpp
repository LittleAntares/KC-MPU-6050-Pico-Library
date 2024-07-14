#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "kcmpu6050.h"
#include <iostream>

i2c_inst_t* I2C_PORT = i2c0;


int MPU6050::mpu_GYRO_CONFIG(uint8_t fs_sel){
    if(fs_sel < 0 || fs_sel > 3){
        std::cout << "Please enter a value betweem 0 and 3" << std::endl;
        return 0;
    }

    int range = 250;
    uint8_t data[2];
    data[0] = GYRO_CONFIG;

    switch (fs_sel)
    {
    case 0:
        data[1] = 0x00;
        gyro_lsb = 131;
        break;

    case 1:
        data[1] = 0x08;
        gyro_lsb = 65.5;
        range = 500;
        break;
    
    case 2:
        data[1] = 0x10;
        gyro_lsb = 32.8;
        range = 1000;
        break;
    
    case 3:
        data[1] = 0x18;
        gyro_lsb = 16.4;
        range = 2000;
        break;

    default:
        data[1] = 0x00;
        gyro_lsb = 131;
        break;
    }

    i2c_write_blocking(I2C_PORT,addr,data,2,false);
    std::cout << "Setting Gyro Full Scale Range to +-"<< range <<"degree/s" << std::endl;
    return 1;
}


void MPU6050::get_gyro(){

    uint8_t data[6];
    data[0] = GYRO_OUT;
    i2c_write_blocking(I2C_PORT,addr,&GYRO_OUT,1,true);
    i2c_read_blocking(I2C_PORT,addr,data,6,false);

    GyroRawX = (data[0] << 8) | data[1];
    GyroRawY = (data[2] << 8) | data[3];
    GyroRawZ = (data[4] << 8) | data[5];

    GyroX = GyroRawX/gyro_lsb;
    GyroY = GyroRawY/gyro_lsb;
    GyroZ = GyroRawZ/gyro_lsb;
    power_mode();
}

void MPU6050::power_mode(){
    uint8_t data[2];
    data[0] = PWR_MGMT_1;
    data[1] = 0x00;

    i2c_write_blocking(I2C_PORT,addr,data,2,false);
}


int MPU6050::mpu_ACCEL_CONFIG(uint8_t afs_sel){
    
    if(afs_sel < 0 || afs_sel > 3){
        std::cout << "Please enter a value between 0 and 3" << std::endl;
        return 0;
    }

    int range = 2;
    uint8_t data[2];
    data[0] = ACCEL_CONFIG;

    switch(afs_sel){
        case 0:
            data[1] = 0x00;
            accel_lsb = 16384;
            range = 2;
            break;
        
        case 1:
            data[1] = 0x08;
            accel_lsb = 8192;
            range = 4;
            break;

        case 2:
            data[1] = 0x10;
            accel_lsb = 4096;
            range = 8;
            break;
        
        case 3:
            data[1] = 0x18;
            accel_lsb = 2048;
            range = 16;
            break;
        
        default:
            data[0] = 0x00;
            accel_lsb = 16384;
            range = 2;
            break;
    }

    std::cout << "Setting Accelerometer range to +-" << range <<"g"<< std::endl;

    return 1;
}

void MPU6050::get_accel(){
    uint8_t data[6];
    data[0] = ACCEL_OUT;

    i2c_write_blocking(I2C_PORT,addr,&ACCEL_OUT,1,true);
    i2c_read_blocking(I2C_PORT,addr,data,6,false);

    AccelRawX = (data[0] << 8) | data[1];
    AccelRawY = (data[2] << 8) | data[3];
    AccelRawZ = (data[4] << 8) | data[5];

    AccelX = AccelRawX/accel_lsb;
    AccelY = AccelRawY/accel_lsb;
    AccelZ = AccelRawZ/accel_lsb;
    power_mode();
}

int MPU6050::mpu_init(int sda, int sdl){

    if(sda >= 0 && sda <= 12 && (sda == (sdl-1))){
        if(sda%4 == 0){
            I2C_PORT = i2c0;
        } else{
            I2C_PORT = i2c1;
        }
    }
    else{
        std::cout << "Invalid GPIO Port for I2C" << std::endl;
        return false;
    }

    i2c_init(I2C_PORT, 400000);
    gpio_set_function(sda,GPIO_FUNC_I2C);
    gpio_set_function(sdl,GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(sdl);

    //Check if the device is connected
    sleep_ms(1000);
    uint8_t chipID[1];
    i2c_write_blocking(I2C_PORT,addr,&WHO_AM_I,1,true);
    i2c_read_blocking(I2C_PORT,addr,chipID,1,false);

    if(chipID[0] == 0x68){
        std::cout << "Sucessfully connected to MPU-6050" << std::endl;
        return true;
    }
       std::cout << "False to connected to MPU-6050" << std::endl;
    return false;

}


int MPU6050::mpu_DLPF(uint8_t dlfp_cfg){

    if(dlfp_cfg> 7 || dlfp_cfg < 0){
        std::cout << "Please enter configuration value between 0 and 7" << std::endl;
        return false;
        
    }
    uint8_t data[2];
    data[0] = DLPF;
    data[1] = dlfp_cfg;
    i2c_write_blocking(I2C_PORT,addr,data,2,false);
    std::cout << "Sucessfully Set the low pass filter" << std::endl;
    return true;
}

