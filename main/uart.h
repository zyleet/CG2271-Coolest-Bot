#include "MKL25Z4.h"
#include "cmsis_os2.h"
extern int UARTdata;
extern osSemaphoreId_t UARTsem;
void initUART2(uint32_t baud_rate);
void UART2_IRQHandler();