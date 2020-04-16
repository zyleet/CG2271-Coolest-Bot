/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_os2.h"
#include "ledControl.h"
#include "uart.h"
#include "pwm.h"
#include "buzzer.h"

#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define BAUD_RATE 9600
#define MASK(x) (1 << (x))

//Define thread to handle UART2 interrupts
void UART2_thread(void *argument) {
    for (;;) {
        osSemaphoreAcquire(UARTsem, osWaitForever);	
        if (UARTdata & 2) {
            if (UARTdata & 1) {
                osSemaphoreRelease(PWMsem);
            } else {
                osSemaphoreRelease(mySem2);
            }
        } else {
            if (UARTdata & 1) {
                osSemaphoreRelease(mySem4);
            } else {
                osSemaphoreRelease(mySem);
                connectFlag = 1;
            }
        }
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
            redDelay = 500;
            running = 0;
            osSemaphoreRelease(greenStopSemaphore);
            continue;
        }
        redDelay = 250;
        running = 1;
    }
}


int main (void) { 
    // System Initialization
    SystemCoreClockUpdate();
    initPWM();
    initUART2(BAUD_RATE);
    initBuzzer();
    initLED();
    osKernelInitialize();                 // Initialize CMSIS-RTOS
    osThreadNew(UART2_thread, NULL, NULL);
    osThreadNew(cruelAngelThesis4Thread, NULL, NULL);
    osThreadSetPriority(cruelAngelThesis4Thread, osPriorityLow7);
    osThreadNew(cruelAngelThesis5Thread, NULL, NULL);
    osThreadSetPriority(cruelAngelThesis5Thread, osPriorityLow7);
    osThreadNew(cruelAngelThesis1Thread, NULL, NULL);  
    osThreadNew(cruelAngelThesis2Thread, NULL, NULL);
    osThreadNew(cruelAngelThesis3Thread, NULL, NULL);
    osThreadNew(led_red_thread, NULL, NULL);
    osThreadNew(led_green_running_thread, NULL, NULL);
    osThreadNew(led_green_stop_thread, NULL, NULL);
    osThreadNew(pwm_thread, NULL, NULL);
    osKernelStart();                      // Start thread execution
    for (;;) {
    }
}
