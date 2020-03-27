#include "MKL25Z4.h"
#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))
typedef enum led {RED, BLUE, GREEN} led;

void initLEDGPIO(void){
// Enable Clock to PORTB and PORTD
    SIM->SCGC5 |= ((SIM_SCGC5_PORTB_MASK) | (SIM_SCGC5_PORTD_MASK));
// Configure MUX settings to make all 3 pins GPIO
    PORTB->PCR[RED_LED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[RED_LED] |= PORT_PCR_MUX(1);
    PORTB->PCR[GREEN_LED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[GREEN_LED] |= PORT_PCR_MUX(1);
    PORTD->PCR[BLUE_LED] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[BLUE_LED] |= PORT_PCR_MUX(1);
// Set Data Direction Registers for PortB and PortD
    PTB->PDDR |= (MASK(RED_LED) | MASK(GREEN_LED));
    PTD->PDDR |= MASK(BLUE_LED);
}
void led_control(led color, int on) {
    if (on == 1) {
        if (color == RED) {
            PTB -> PDOR &= ~MASK(RED_LED);
        } else if (color == GREEN) {
            PTB -> PDOR &= ~MASK(GREEN_LED);
        } else if (color == BLUE) {
            PTD -> PDOR &= ~MASK(BLUE_LED);
        }
    } else {
        if (color == RED) {
            PTB -> PDOR |= MASK(RED_LED);
        } else if (color == GREEN) {
            PTB -> PDOR |= MASK(GREEN_LED);
        } else if (color == BLUE) {
            PTD -> PDOR |= MASK(BLUE_LED);
        }
    }
}

void initLED() {
	PTB -> PDOR |= MASK(RED_LED);
	PTB -> PDOR |= MASK(GREEN_LED);
	PTD -> PDOR |= MASK(BLUE_LED);
}