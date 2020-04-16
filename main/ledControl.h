#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

extern osSemaphoreId_t greenStopSemaphore;
extern osSemaphoreId_t redSemaphore;
extern int running;
extern int redDelay; //500 when moving, 250 when staying
extern int connectFlag;

void led_red_thread(void *argument);
void led_green_running_thread(void *argument);
void led_green_stop_thread(void *argument);
void initLED(void);