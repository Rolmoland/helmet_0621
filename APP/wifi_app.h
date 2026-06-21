#ifndef _ESP01S_H
#define _ESP01S_H

#include "bsp_system.h"

#define WIFI_SSID               "123"
#define WIFI_PWD                "12345670"

#define HUAWEI_MQTT_ADDRESS     "e3d70f6c17.st1.iotda-device.cn-south-1.myhuaweicloud.com"
#define HUAWEI_MQTT_ClientID    "67bec08580c3872914ade8c0_smart_helmet_2_0_0_2025040608"
#define HUAWEI_MQTT_USERNAME    "67bec08580c3872914ade8c0_smart_helmet_2"
#define HUAWEI_MQTT_PASSWORD    "f6b9de259b59bd03c632d40138419d96a4e02a5fe809cf30e3c31db0598592a0"
#define HUAWEI_MQTT_PORT        "1883"

void esp_init(void);
void esp_report1(void);
void esp_report2(void);

#endif
