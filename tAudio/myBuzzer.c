#include "MKL25Z4.h"
#define PTB0_Pin 0
#define PTB1_Pin 1
#define MASK(x) (1 << (x))
#define A4 440
#define AS4 466
#define B4 494  
#define C5 523
#define CS5 554
#define D5 587
#define DS5 622
#define E5 659
#define F5 698
#define FS5 740
#define G5 784
#define GS5 831
#define A5 880
#define AS5 932
#define B5 988
#define C6 1047
#define CS6 1109
#define D6 1175
#define DS6 1245
#define E6 1319
#define F6 1397
#define FS6 1480
#define G6 1568
#define GS6 1661
#define SEMIBREVE 1000
#define CRO 500 //CROTCHET
#define QUA 250 //QUAVER

int freqToMod(int freq) {
    float x = 48000000/(128*(float)freq);
    return (int)x;
}

void initPWM(void) {
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
    
    PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
    
    PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
    
    SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
    
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
    
    TPM1->SC &= ~((TPM_SC_CMOD_MASK) |TPM_SC_PS_MASK);
    TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM1->SC &= ~(TPM_SC_CPWMS_MASK);
    
    TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

static void delay(volatile uint32_t nof) {
    while(nof!=0) {
        __ASM("NOP");
        nof--;
    }
}

void quatreMainsThread(void* argument) {
    int note[] = {C6, D5, 0, D5, 0, D5, 0, CS5, D5, 0, D5, 0, D5, 0};
    int duration[] = {SEMIBREVE, QUA, 83, QUA, 83, QUA, 83, SEMIBREVE, QUA, 83, QUA, 83, QUA, 83};
    int counter = 0;
    for (;;) {
        TPM1->MOD = freqToMod(note[counter]);
        TPM1_C0V = (int)(0.5 * TPM1->MOD);
        osDelay(duration[counter]);
        counter = (counter + 1)%14;
    }
}
int main() {
    initPWM();
    osKernelInitialize();
    osThreadNew(quatreMainsThread, NULL, NULL);
    osKernelStart
    while(1){
    }
}