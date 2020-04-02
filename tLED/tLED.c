#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#define COL_1 5 // PortC Pin 5
#define COL_2 6 // PortC Pin 6
#define ROW_1 0 // PortC Pin 0
#define ROW_2 3 // PortC Pin 3
#define ROW_3 4 // PortC Pin 4
#define ROW_4 7 // PortC Pin 7
#define RED 1 // PortA Pin 1
#define SWITCH 6 // PortD Pin 6
#define MASK(x) (1 << (x))


osSemaphoreId_t greenSemaphore;
osSemaphoreId_t redSemaphore;

int greenDelay = 0; //500 when moving, 1 when staying
int redDelay = 0; //500 when moving, 250 when staying

void InitGPIO(void)
{
	// Enable Clock to PORTC
	SIM->SCGC5 |= ((SIM_SCGC5_PORTC_MASK) | (SIM_SCGC5_PORTA_MASK));
	
	// Configure MUX settings to make all 7 pins GPIO
	PORTC->PCR[COL_1] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[COL_1] |= PORT_PCR_MUX(1);
	
	PORTC->PCR[COL_2] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[COL_2] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[ROW_1] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[ROW_1] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[ROW_2] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[ROW_2] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[ROW_3] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[ROW_3] |= PORT_PCR_MUX(1);
    
    PORTC->PCR[ROW_4] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTC->PCR[ROW_4] |= PORT_PCR_MUX(1);
    
    PORTA->PCR[RED] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTA->PCR[RED] |= PORT_PCR_MUX(1);
	
	// Set Data Direction Registers for PortC
	PTC->PDDR |= (MASK(COL_1) | MASK(COL_2));
    PTC->PDDR |= (MASK(ROW_1) | MASK(ROW_2) | MASK(ROW_3) | MASK(ROW_4));
    PTA->PDDR |= (MASK(RED));
    
}

void initSwitch(void){
    /* enable clock for port D */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    
    /* Select GPIO and enable pull-up resistors and
    interrupts on falling edges for pin connected to switch */
    PORTD->PCR[SWITCH] |= PORT_PCR_MUX(1) |
    PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);
    
    /* Set port D switch bit to inputs */
    PTD->PDDR &= ~MASK(SWITCH);
    
    /* Enable Interrupts */
    NVIC_SetPriority(PORTD_IRQn, 0);
    NVIC_ClearPendingIRQ(PORTD_IRQn);
    NVIC_EnableIRQ(PORTD_IRQn);
}

static void delay(volatile uint32_t nof) {
    while(nof!=0) {
        __asm("NOP");
        nof--;
     }
}

void PORTD_IRQHandler(void) {
    // clear pending interrupts
    NVIC_ClearPendingIRQ(PORTD_IRQn);
    
    delay(0x80000);
    //osSemaphoreRelease(greenSemaphore);
    //osSemaphoreRelease(redSemaphore);
    
    if (greenDelay == 1) {
        greenDelay = 500;
    } else {
        greenDelay = 1;
    }
    
    if (redDelay == 250) {
        redDelay = 500;
    } else {
        redDelay = 250;
    }
    
    // clear status flags
    PORTD->ISFR = 0xffffffff;
}

//Green LED
//Light up one at a time when moving 
//All 8 remain lit when stationary 

void green_led(void *argument) {
    //osSemaphoreAcquire(greenSemaphore, osWaitForever);
    
    int vert = 0;
    int hor = 0;
    int hor_array[4] = {MASK(ROW_1),MASK(ROW_2), MASK(ROW_3), MASK(ROW_4)};
    int vert_array[2] = {MASK(COL_1), MASK(COL_2)};
    
   
    while(1) {
        PTC->PDOR = 0;
        PTC->PDOR |= vert_array[vert];
        PTC->PDOR |= hor_array[hor];
        osDelay(greenDelay);
        hor += 1;
        if (hor == 4) {
            hor = 0;
            vert = 1 - vert;
        }
    }
}


//Red LED
//All 8 flashes at 250ms intervals when stationary 
//All 8 flashes at 500ms intervals when moving

void red_led(void *argument) {
    //osSemaphoreAcquire(redSemaphore, osWaitForever);
    
    while(1) {
        PTA->PSOR = MASK(RED);
        osDelay(redDelay);
        PTA->PCOR = MASK(RED);
        osDelay(redDelay);
    }
}


int main(void) {
	SystemCoreClockUpdate();
    initSwitch();
	InitGPIO();
    greenDelay = 1;
    redDelay = 250;
    __enable_irq();
    
	osKernelInitialize();
    //mySem = osSemaphoreNew(2, 2, NULL);  
    //redSemaphore = osSemaphoreNew(1, 1, NULL);
    //greenSemaphore = osSemaphoreNew(1, 1,NULL);
    osThreadNew(red_led, NULL, NULL);
    osThreadNew(green_led, NULL, NULL);
    
    osKernelStart();
}