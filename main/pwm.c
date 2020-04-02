#include "MKL25Z4.h"
#define PTB0_Pin 0
#define PTB1_Pin 1
#define MASK(x) (1 << (x))

int freqToMod(int freq) {
    float x = 48000000/(128*(float)freq);
    return (int)x;
}

void initPWM(void) {
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
    
    PORTA->PCR[1] &= ~PORT_PCR_MUX_MASK; //timer 2
    PORTA->PCR[1] |= PORT_PCR_MUX(3);
		PORTA->PCR[2] &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
		PORTA->PCR[2] |= PORT_PCR_MUX(1);
		PORTA->PCR[4] &= ~PORT_PCR_MUX_MASK; //timer 0
    PORTA->PCR[4] |= PORT_PCR_MUX(3);
		PORTA->PCR[5] &= ~PORT_PCR_MUX_MASK; //timer 0
    PORTA->PCR[5] |= PORT_PCR_MUX(3);
    
		PTA->PDDR |= (MASK(2));
	
    SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;
    
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
    
    TPM0->SC &= ~((TPM_SC_CMOD_MASK) |TPM_SC_PS_MASK);
    TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
    
    TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
		TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    
    TPM2->SC &= ~((TPM_SC_CMOD_MASK) |TPM_SC_PS_MASK);
    TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
    TPM2->SC &= ~(TPM_SC_CPWMS_MASK);
		
    TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
    TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
		
		TPM0->MOD = 256;
		TPM2->MOD = 256;
}

void pwm_stop(void) {
	TPM0_C0V = 0;
	TPM0_C2V = 0;
	PTA->PCOR |= MASK(2);
	TPM2_C0V = 0;
}

void pwm_forward(void) {
	TPM0_C0V = 0;
	TPM0_C2V = (int)(0.5 * 256);
	PTA->PSOR |= MASK(2);
	TPM2_C0V = (int)(0.5 * 256);
}

void pwm_backward(void) {
	TPM0_C2V = 0;
	TPM0_C0V = (int)(0.5 * TPM0->MOD);
	PTA->PCOR |= MASK(2);
	TPM2_C0V = (int)(0.5 * TPM2->MOD);
}

void pwm_left() {
}

void pwm_right() {
}

void pwm_forward_left() {
}

void pwm_forward_right() {
}

void pwm_backward_left() {
}

void pwm_backward_right() {
}
