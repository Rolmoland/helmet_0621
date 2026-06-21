#include "wifi_app.h"

void esp_init(void)
{
    my_printf(&huart3, "AT+RST\r\n");
    HAL_Delay(1000);
    my_printf(&huart3, "AT+CWMODE=1\r\n");
    HAL_Delay(1000);
    my_printf(&huart3, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PWD);
    HAL_Delay(4000);
    my_printf(&huart3, "AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n", HUAWEI_MQTT_USERNAME, HUAWEI_MQTT_PASSWORD);
    HAL_Delay(1000);
    my_printf(&huart3, "AT+MQTTCLIENTID=0,\"%s\"\r\n", HUAWEI_MQTT_ClientID);
    HAL_Delay(1000);
    my_printf(&huart3, "AT+MQTTCONN=0,\"%s\",1883,1\r\n", HUAWEI_MQTT_ADDRESS);
    HAL_Delay(1000);
}

void esp_report1(void)
{
    my_printf(&huart3, "AT+MQTTPUB=0,\"$oc/devices/%s/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"BasicData\\\"\,\\\"properties\\\":{\\\"spO2\\\":%d\,\\\"density\\\":%.2f\,\\\"heart_rate\\\":%d\,\\\"fall_flag\\\":%d\,\\\"collision_flag\\\":%d}}]}\",0,0\r\n",
              HUAWEI_MQTT_USERNAME, dis_spo2, ppm, dis_hr, fall_flag, collision_flag);
}

void esp_report2(void)
{
    my_printf(&huart3, "AT+MQTTPUB=0,\"$oc/devices/%s/sys/properties/report\",\"{\\\"services\\\":[{\\\"service_id\\\":\\\"BasicData\\\"\,\\\"properties\\\":{\\\"longitude\\\":%.2f\,\\\"latitude\\\":%.2f\,\\\"temperature\\\":%d\,\\\"humidity\\\":%d}}]}\",0,0\r\n",
              HUAWEI_MQTT_USERNAME, longitude, latitude, temp, humi);
}
