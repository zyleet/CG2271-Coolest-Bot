#include "MKL25Z4.h"
#include "pwm.h"
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494  
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define SEMIBREVE 1000
#define CRO 500 //CROTCHET
#define QUA 250 //QUAVER

int counter = 0;
void music() {
    
    while(1){
        TPM1->MOD = freqToMod(494);
        TPM1_C0V = (int)(0.5 * TPM1->MOD);
        for (int i = 0; i < 4800000; i += 1) {
        }
    }
    counter += 1;
}