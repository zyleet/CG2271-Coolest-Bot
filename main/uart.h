#include "MKL25Z4.h"                   
void initUART2(uint32_t baud_rate);
void UART2_Transmit_Poll(uint8_t data);
uint8_t UART2_Receive_Poll(void);
