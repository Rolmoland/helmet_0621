#include "gps_app.h"

uint16_t point1 = 0;
float longitude;
float latitude;

_SaveData Save_Data;
LatitudeAndLongitude_s g_LatAndLongData =
{
    .E_W = 0,
    .N_S = 0,
    .latitude = 0.0,
    .longitude = 0.0
};

char USART_RX_BUF[USART_REC_LEN];
uint8_t uart_A_RX_Buff;

void atgm336h_init(void)
{
    clrStruct();
    HAL_UART_Receive_IT(&huart2, &uart_A_RX_Buff, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        if (uart_A_RX_Buff == '$') point1 = 0;
        USART_RX_BUF[point1++] = uart_A_RX_Buff;

        if (USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')
        {
            if (uart_A_RX_Buff == '\n')
            {
                memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);
                memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1);
                Save_Data.isGetData = true;
                point1 = 0;
                memset(USART_RX_BUF, 0, USART_REC_LEN);
            }
        }
        if (point1 >= USART_REC_LEN) point1 = USART_REC_LEN;
        HAL_UART_Receive_IT(&huart2, &uart_A_RX_Buff, 1);
    }
}

void clrStruct(void)
{
    Save_Data.isGetData = false;
    Save_Data.isParseData = false;
    Save_Data.isUsefull = false;
    memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);
    memset(Save_Data.UTCTime, 0, UTCTime_Length);
    memset(Save_Data.latitude, 0, latitude_Length);
    memset(Save_Data.N_S, 0, N_S_Length);
    memset(Save_Data.longitude, 0, longitude_Length);
    memset(Save_Data.E_W, 0, E_W_Length);
}

void parseGpsBuffer(void)
{
    char *subString;
    char *subStringNext;
    char i = 0;
    uint16_t Number = 0, Integer = 0, Decimal = 0;

    if (Save_Data.isGetData)
    {
        Save_Data.isGetData = false;
        for (i = 0; i <= 6; i++)
        {
            if (i == 0)
            {
                if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
                    return;
            }
            else
            {
                subString++;
                if ((subStringNext = strstr(subString, ",")) != NULL)
                {
                    char usefullBuffer[2];
                    switch (i)
                    {
                        case 1: memcpy(Save_Data.UTCTime, subString, subStringNext - subString); break;
                        case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break;
                        case 3: memcpy(Save_Data.latitude, subString, subStringNext - subString); break;
                        case 4: memcpy(Save_Data.N_S, subString, subStringNext - subString); break;
                        case 5: memcpy(Save_Data.longitude, subString, subStringNext - subString); break;
                        case 6: memcpy(Save_Data.E_W, subString, subStringNext - subString); break;
                        default: break;
                    }
                    subString = subStringNext;
                    Save_Data.isParseData = true;
                    if (usefullBuffer[0] == 'A') Save_Data.isUsefull = true;
                    else if (usefullBuffer[0] == 'V') Save_Data.isUsefull = false;
                }
            }
        }

        if (Save_Data.isParseData && Save_Data.isUsefull)
        {
            g_LatAndLongData.N_S = Save_Data.N_S[0];
            g_LatAndLongData.E_W = Save_Data.E_W[0];

            Number = Integer = Decimal = 0;
            for (uint8_t j = 0; j < 9; j++)
            {
                if (j < 2) { Number *= 10; Number += Save_Data.latitude[j] - '0'; }
                else if (j < 4) { Integer *= 10; Integer += Save_Data.latitude[j] - '0'; }
                else if (j == 4);
                else if (j < 9) { Decimal *= 10; Decimal += Save_Data.latitude[j] - '0'; }
            }
            g_LatAndLongData.latitude = 1.0 * Number + (1.0 * Integer + 1.0 * Decimal / 10000) / 60;

            Number = Integer = Decimal = 0;
            for (uint8_t j = 0; j < 10; j++)
            {
                if (j < 3) { Number *= 10; Number += Save_Data.longitude[j] - '0'; }
                else if (j < 5) { Integer *= 10; Integer += Save_Data.longitude[j] - '0'; }
                else if (j == 5);
                else if (j < 10) { Decimal *= 10; Decimal += Save_Data.longitude[j] - '0'; }
            }
            g_LatAndLongData.longitude = 1.0 * Number + (1.0 * Integer + 1.0 * Decimal / 10000) / 60;

            longitude = g_LatAndLongData.longitude;
            latitude = g_LatAndLongData.latitude;

            if (g_LatAndLongData.E_W == 'W') longitude = -longitude;
            if (g_LatAndLongData.N_S == 'S') latitude = -latitude;
        }
    }
}

void printGpsBuffer(void)
{
    if (Save_Data.isParseData)
    {
        Save_Data.isParseData = false;
        my_printf(&huart1, "Save_Data.UTCTime = %s\r\n", Save_Data.UTCTime);
        if (Save_Data.isUsefull)
        {
            Save_Data.isUsefull = false;
            my_printf(&huart1, "Save_Data.latitude = %s\r\n", Save_Data.latitude);
            my_printf(&huart1, "Save_Data.longitude = %s\r\n", Save_Data.longitude);
            my_printf(&huart1, "latitude: %c,%.4f\r\n", g_LatAndLongData.N_S, g_LatAndLongData.latitude);
            my_printf(&huart1, "longitude: %c,%.4f\r\n", g_LatAndLongData.E_W, g_LatAndLongData.longitude);
        }
    }
}

void atgm336h_task(void)
{
    parseGpsBuffer();
}
