/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#define MASK(x) (1 << (x))
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}
 
void initMotor(void) {
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    PORTA->PCR[2] &= ~PORT_PCR_MUX_MASK; //timer 2
    PORTA->PCR[2] |= PORT_PCR_MUX(3);
    
    PORTA->PCR[1] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTA->PCR[1] |= PORT_PCR_MUX(1);
    
    PORTA->PCR[4] &= ~PORT_PCR_MUX_MASK; //timer 0
    PORTA->PCR[4] |= PORT_PCR_MUX(3);
    
    PORTA->PCR[12] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
	PORTA->PCR[12] |= PORT_PCR_MUX(1);
    
    PTA->PDDR |= (MASK(1) | MASK(12));
    
    SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
    
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
    
    TPM0->SC &= ~((TPM_SC_CMOD_MASK) |TPM_SC_PS_MASK);
    TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
    
    TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    
    TPM2->SC &= ~((TPM_SC_CMOD_MASK) |TPM_SC_PS_MASK);
    TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
    
    TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

static void delay(volatile uint32_t nof) {
    while(nof!=0) {
        __ASM("NOP");
        nof--;
    }
}

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  initMotor();
  TPM0->MOD = 256;

  for (;;) {
    TPM0_C1V = (int)(0.5 * TPM0->MOD);
    PTA->PSOR |= MASK(12);
    delay(5000000);
    TPM0_C1V = 256;
    delay(5000000);
    PTA->PCOR |= MASK(12);
    TPM0_C1V = (int)(0.5 * TPM0->MOD);
    delay(5000000);
    TPM0_C1V = 0;
    delay(5000000);
  }
  
}
