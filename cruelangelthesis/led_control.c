#include "MKL25Z4.h"
#define RED_LED 18 // PortB Pin 18
#define GREEN_LED 19 // PortB Pin 19
#define BLUE_LED 1 // PortD Pin 1
#define MASK(x) (1 << (x))
typedef enum led {RED, BLUE, GREEN} led;
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
 