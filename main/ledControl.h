#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

extern osEventFlagsId_t greenEventFlag;
extern int redDelay; //500 when moving, 250 when staying
void led_red_thread(void *argument);
void led_green_running_thread(void *argument);
void led_green_stop_thread(void *argument);
void initLED(void);
