#ifndef _ATGM336_H
#define _ATGM336_H

#include "bsp_system.h"

#define USART_REC_LEN      200
#define EN_USART1_RX       1

#define GPS_Buffer_Length  80
#define UTCTime_Length     11
#define latitude_Length    11
#define N_S_Length         2
#define longitude_Length   12
#define E_W_Length         2

typedef struct SaveData
{
    char GPS_Buffer[GPS_Buffer_Length];
    char isGetData;
    char isParseData;
    char UTCTime[UTCTime_Length];
    char latitude[latitude_Length];
    char N_S[N_S_Length];
    char longitude[longitude_Length];
    char E_W[E_W_Length];
    char isUsefull;
} _SaveData;

typedef struct _LatitudeAndLongitude_s
{
    float latitude;
    float longitude;
    char N_S;
    char E_W;
} LatitudeAndLongitude_s;

extern char USART_RX_BUF[USART_REC_LEN];
extern uint16_t point1;
extern _SaveData Save_Data;
extern LatitudeAndLongitude_s g_LatAndLongData;

void atgm336h_init(void);
void clrStruct(void);
void parseGpsBuffer(void);
void printGpsBuffer(void);
void atgm336h_task(void);

#endif
