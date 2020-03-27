typedef enum led {RED, BLUE, GREEN} led;
void led_control(led color, int on);
void initLED();
void initLEDGPIO(void);