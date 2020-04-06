/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "led_control.h"
#include "uart.h"
#include "pwm.h"

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define BAUD_RATE 9600
#define MASK(x) (1 << (x))

//Define thread to handle UART2 interrupts
void UART2_thread(void *argument) {
	for (;;) {
		osSemaphoreAcquire(UARTsem, osWaitForever);				
		osSemaphoreRelease(PWMsem);
	}
}

void pwm_thread(void *argument) {
	for (;;) {
		osSemaphoreAcquire(PWMsem, osWaitForever);
		if (UARTdata & MASK(2)) {
			if (UARTdata & MASK(4)) {
				pwm_forward_left();
			} else if (UARTdata & MASK(5)) {
				pwm_forward_right();
			} else {				
				pwm_forward();
			}
		} else if (UARTdata & MASK(3)) {
			if (UARTdata & MASK(4)) {
				pwm_backward_left();
			} else if (UARTdata & MASK(5)) {
				pwm_backward_right();
			} else {				
				pwm_backward();
			}
        } else if (UARTdata & MASK(4)) {
			pwm_left();
		} else if (UARTdata & MASK(5)) {
			pwm_right();
		} else {
			pwm_stop();
		}
	}
}

void led_red_thread(void *argument) {
    for (;;) {
        osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
        led_control(RED, 1);
        osDelay(1000);
        led_control(RED, 0);
        osDelay(1000);
    }
}

void led_green_thread(void *argument) {
    for (;;) {
        osThreadFlagsWait(0x00000001, osFlagsWaitAny, osWaitForever);
        led_control(GREEN, 1);
        osDelay(1000);
        led_control(GREEN, 0);
        osDelay(1000);
    }
}

int main (void) { 
    // System Initialization
    SystemCoreClockUpdate();
    initPWM();
    initUART2(BAUD_RATE);
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    osThreadNew(UART2_thread, NULL, NULL);
    osThreadNew(pwm_thread, NULL, NULL);
    osKernelStart();                      // Start thread execution
    for (;;) {
    }
}
