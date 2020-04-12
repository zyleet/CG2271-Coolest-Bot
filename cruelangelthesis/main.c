#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "string.h"

#define PTB0_Pin 0
#define PTB1_Pin 1
#define MASK(x) (1 << (x))
#define C6 1047
#define CS6 1109
#define D6 1175
#define DS6 1245
#define E6 1319
#define F6 1397
#define FS6 1480
#define G6 1568
#define GS6 1661
#define A6 1760
#define AS6 1865
#define B6 1976
#define C7 2093
#define CS7 2217
#define D7 2349
#define DS7 2489
#define E7 2637
#define F7 2794
#define FS7 2960
#define G7 3136
#define GS7 3322
#define A7 3520
#define AS7 3729
#define B7 3951
#define sp 4186
#define D8 4699
#define SEMIBREVE 1000
#define CRO 500 //CROTCHET
#define QUA 250 //QUAVER

osSemaphoreId_t mySem, mySem2, mySem3, mySem4, mySem5;
                
int MusicRepeat1 = 0;
int MusicRepeat2 = 0;

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

void cruelAngelThesis1Thread(void* argument) {
    for (;;) {
        osSemaphoreAcquire(mySem, osWaitForever);
        int note[] =     {C7,  DS7, F7,  DS7, F7,  AS7, GS7, G7,  F7,  G7,  AS7, sp,  F7,  DS7, AS7, AS7, G7,  AS7, AS7, sp};
        int duration[] = {500, 500, 375, 375, 750, 250, 250, 125, 250, 1125, 500, 400, 300, 375, 250, 250, 250, 250, 375, 1625};
        int total = sizeof(note)/sizeof(int);
        int counter = 0;
        for (;counter < total; counter += 1) {
            TPM1->MOD = freqToMod(note[counter]);
            TPM1_C0V = (int)(0.2 * TPM1->MOD);
            osDelay(duration[counter]);
        }
        osSemaphoreRelease(mySem2);
    }
}

//Bar 10
void cruelAngelThesis2Thread(void* argument) {
    for (;;) {
        osSemaphoreAcquire(mySem2, osWaitForever);
        int note[] =     {0,   DS7, AS6,  0,  DS7, F7,  AS6, AS6, G7,  GS7,  G7,  F7, DS7, F7,  G7,  GS7,  G7,  C7,  C7,  D7};
        int duration[] = {500, 500, 750, 125, 750, 375, 250, 1000, 375, 375, 250, 375, 375, 250, 375, 375, 250, 750, 125, 125};
        int counter = 0;
        int total = sizeof(note)/sizeof(int);
        for (;counter < total; counter += 1) {
            TPM1->MOD = freqToMod(note[counter]);
            TPM1_C0V = (int)(0.2 * TPM1->MOD);
            osDelay(duration[counter]);
        }
        osSemaphoreRelease(mySem3);
    }
}


//Bar 14
void cruelAngelThesis3Thread(void* argument) {
    for (;;) {
        osSemaphoreAcquire(mySem3, osWaitForever);
        int note1, note2, timer1, timer2;
        osSemaphoreId_t releasedSem;
        if (MusicRepeat1 == 0) {
            note1 = C7;
            note2 = D7;
            timer1 = 1000;
            timer2 = 1250;
            MusicRepeat1 = 1;
            releasedSem = mySem2; //repeat once more first, before moving to next thread
        } else {
            note1 = GS7;
            note2 = G7;
            timer1 = 250;
            timer2 = 1875;
            MusicRepeat1 = 0;
            releasedSem = mySem4;
        }
        int note[] =     {DS7,  D7, DS7,  F7,  GS7,  G7,  F7, DS7, G7,   F7,  E7,  F7,  G7, note1, note2};
        int duration[] = {750, 1000, 125, 125, 375, 375, 250, 750, 625, 375, 250, 375, 375, timer1, timer2};
        int counter = 0;
        int total = sizeof(note)/sizeof(int);
        for (;counter < total; counter += 1) {
            TPM1->MOD = freqToMod(note[counter]);
            TPM1_C0V = (int)(0.2 * TPM1->MOD);
            osDelay(duration[counter]);
        }
        osSemaphoreRelease(releasedSem);
    }
}




//Bar 26
void cruelAngelThesis4Thread(void* argument) {
    for (;;) {
        osSemaphoreAcquire(mySem4, osWaitForever);
        int note1, note2, note3, note4, note5, note6;
        int timer1;
        osSemaphoreId_t releasedSem;
        int note[] =     {DS7, D7,  DS7, D7,  F7,  DS7, D7,  C7,  D7, DS7,  D7,   F7,  D7,  C7,    AS6,    G7,    GS7,   AS7};
        int duration[] = {750, 250, 750, 250, 750, 250, 375, 375, 250, 750, 250, 375, 375,  250,    500,   500,   500,   500};
        int counter = 0;
        int total = sizeof(note)/sizeof(int);
        for (;counter < total; counter += 1) {
            TPM1->MOD = freqToMod(note[counter]);
            TPM1_C0V = (int)(0.2 * TPM1->MOD);
            osDelay(duration[counter]);
        }
        osSemaphoreRelease(mySem5);
    }
}

//Bar 30
void cruelAngelThesis5Thread(void* argument) {
    for (;;) {
        osSemaphoreAcquire(mySem5, osWaitForever);
        int note[] =     {DS7, D7,  DS7, D7,  F7,  DS7, D7,  C7,  D7, DS7,  D7,   F7,  G7,  GS7,    G7};
        int duration[] = {750, 250, 750, 250, 750, 250, 375, 375, 250, 750, 250, 375, 375,  250,   1875};
        int counter = 0;
        int total = sizeof(note)/sizeof(int);
        for (;counter < total; counter += 1) {
            TPM1->MOD = freqToMod(note[counter]);
            TPM1_C0V = (int)(0.2 * TPM1->MOD);
            osDelay(duration[counter]);
        }
        osSemaphoreRelease(mySem);
    }
}


//To loop back, final thread releases the semaphore of the first thread
//Hard limit of 21 notes in one thread, safe limit is 20
//Thread 1 is the establish connection ring tone
//Thread 2 and 3 is the running tone
//Thread 4 is the ending tone
//ReleasedSem is used to test how the entire song sounds like, can remove
int main() {
    initPWM();
    osKernelInitialize();
    mySem = osSemaphoreNew(1,1,NULL);
    mySem2 = osSemaphoreNew(1,0,NULL);
    mySem3 = osSemaphoreNew(1,0,NULL);
    mySem4 = osSemaphoreNew(1,0,NULL);
    mySem5 = osSemaphoreNew(1,0,NULL);

    osThreadNew(cruelAngelThesis1Thread, NULL, NULL);
    osThreadNew(cruelAngelThesis2Thread, NULL, NULL);
    osThreadNew(cruelAngelThesis3Thread, NULL, NULL);
    osThreadNew(cruelAngelThesis4Thread, NULL, NULL);
    osThreadNew(cruelAngelThesis5Thread, NULL, NULL);
    osKernelStart();
    while(1){
    }
}