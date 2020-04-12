#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "string.h"

extern osSemaphoreId_t mySem2, mySem3;
extern osEventFlagsId_t idleMusicFlag;
extern int MusicRepeat1, MusicRepeat2;

void initBuzzer(void);
void cruelAngelThesis1Thread(void);
void cruelAngelThesis2Thread(void* argument);
void cruelAngelThesis3Thread(void* argument);
void cruelAngelThesis4Thread(void);
void cruelAngelThesis5Thread(void);
