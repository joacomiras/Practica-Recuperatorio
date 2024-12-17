#include "MKL25Z4.h"
#include "act3.h"
volatile int select, b1, b2;
int puls1=15, puls2=12;
int main (void){
    SIM->SCGC5_PORTD_MASK;
    NVIC_EnableIRQ(PORTD_IRQn);
    NVIC_SetPriority(PORTD_IRQn,0);
    PORTD->PCR[12]=PORT_PCR_MUX(1)|PORTD_PCR_IRQC(0b1101);
    InitContAscendente();
    InitContDescendente();
    InitDeMux();
    InitLeds();
    PORTC-> PCR[5]|=(1<<8);
    PTC-> PDDR|=(1u<<5);

    while(1){
        b1 = PTC->PDIR&(1<<5);
        b2 = PTC->PDIR&(1<<5);
        select=b1|(b2<<1);
        DeMux();
        leds();
    }
    return 0;
}

void PORTD_IRQhander(void){
    if(PORTD->ISFR&(1<<12)){
        PORTD->ISFR|=(1<<12)
        b1=0;
        b2=0;
    }
}