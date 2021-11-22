#include <iostream>
#include "include/rplidar.h"
#include "include/rplidar_driver.h"

#include <unistd.h>
static inline void delay(sl_word_size_t ms) {
    while (ms >= 1000) {
        usleep(1000 * 1000);
        ms -= 1000;
    };
    if (ms != 0)
        usleep(ms * 1000);
}

#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))

using namespace sl;

//rp::standalone::rplidar::RPlidarDriver *RPlidarDriver::CreateDriver(_u32 drivertype);

bool ctrl_c_pressed;

int main() {
    std::cout << "Hello World" << std::endl;

    const char* opt_is_channel = NULL;
    const char* opt_channel = NULL;
    const char* opt_channel_param_first = "/dev/ttyUSB0";
    int opt_channel_type = CHANNEL_TYPE_SERIALPORT;
    int boundRate = 115200;
    bool connectSuccess = false;
    bool useArgcBaudrate = false;

    sl_u32 baudrateArray[2] = { 115200, 256000 };
    sl_u32 opt_channel_param_second = 0;
    sl_result op_result;
    IChannel* _channel;
    ILidarDriver* drv = *createLidarDriver();
    sl_lidar_response_device_info_t devinfo;

    _channel = (*createSerialPortChannel("/dev/ttyUSB0", 115200));

    if (SL_IS_OK((drv)->connect(_channel))) {
        op_result = drv->getDeviceInfo(devinfo);

        if (SL_IS_OK(op_result)) {
            connectSuccess = true;
        }
        else {
            delete drv;
            drv = NULL;
        }
    }
    if (!connectSuccess) {
        std::cout << "ERROR WHILE CONNECTION";
    }
    else {
        std::cout << "CONNECTED";
    }


    printf("SLAMTEC LIDAR S/N: ");
    for (int pos = 0; pos < 16; ++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }

    printf("\n"
        "Firmware Ver: %d.%02d\n"
        "Hardware Rev: %d\n",
        devinfo.firmware_version >> 8, devinfo.firmware_version & 0xFF, (int)devinfo.hardware_version);

    drv->setMotorSpeed();
    drv->startScan(0, 1);
}

bool checkSLAMTECLIDARHealth(ILidarDriver* drv) {
    sl_result op_result;
    sl_lidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (SL_IS_OK(op_result)) { // the macro IS_OK is the preperred way to judge whether the operation is succeed.
        printf("SLAMTEC Lidar health status : %d\n", healthinfo.status);
        if (healthinfo.status == SL_LIDAR_STATUS_ERROR) {
            fprintf(stderr, "Error, slamtec lidar internal error detected. Please reboot the device to retry.\n");
            // enable the following code if you want slamtec lidar to be reboot by software
            // drv->reset();
            return false;
        }
        else {
            return true;
        }
    }
    else {
        fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", op_result);
        return false;
    }
}
