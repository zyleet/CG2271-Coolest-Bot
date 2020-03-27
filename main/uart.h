#include "MKL25Z4.h"
#include "cmsis_os2.h"
int UARTdata;
osSemaphoreId_t UARTSem;
void initUART2(uint32_t baud_rate);
void UART2_IRQHandler();