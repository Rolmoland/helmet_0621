#ifndef __SYS_H
#define __SYS_H
#include "bsp_system.h"

void WFI_SET(void);
void INTX_DISABLE(void);
void INTX_ENABLE(void);
void MSR_MSP(uint32_t addr);

#endif
