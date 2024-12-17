#include "MKL25Z4.h"
#include "act3.h"
int i, j, led_r=20, led_v = 21;
int S1=2, S2=3, puls=5, entrada=1;
int display1[7]={6, 7, 8, 9, 10, 11, 12};
int display2[7]={13, 14, 15, 16, 17, 18, 19};


int bcd7[7][10]={
//    a  b  c  d  e  f  g
    { 1, 1, 1, 1, 1, 1, 0},//0
    { 0, 1, 1, 0, 0, 0, 0},//1
    { 1, 1, 0, 1, 1, 0, 1},//2
    { 1, 1, 1, 1, 0, 0, 1},//3
    { 0, 1, 1, 0, 0, 1, 1},//4
    { 1, 0, 1, 1, 0, 1, 1},//5
    { 1, 0, 1, 1, 1, 1, 1},//6
    { 1, 1, 1, 0, 0, 0, 0},//7
    { 1, 1, 1, 1, 1, 1, 1},//8
    { 1, 1, 1, 1, 0, 1, 1}//9
};
void InitDeMux(){
    SIM->SCGC5_PORTC_MCSK;
    PORTC-> PCR[2]|=(1<<8);
    PORTC-> PCR[3]|=(1<<8);
    PTC-> PDDR&=~(1u<<2);
    PTC-> PDDR&=~(1u<<3);
    PTC-> PSOR&=~(1u<<2);
    PTC-> PSOR&=~(1u<<3);

}
void DeMux(){
    if((entrada==1)&&(select==0b01)){
        
        ContAscendente(Dec, Uni);
    }else if ((entrada==1)&&(select==0b10)){
        ContDescendente(Dec, Uni);
    }else if ((entrada==1)&&(select==0b00)){
        ContDescendente(0, 0);
        ContAscendente(0, 0);
    }
}
void InitContAscendente(){
    SIM->SCGC5_PORTC_MCSK;
    for(i=0; i<7; i++){
        PORTC-> PCR[display1[i]]|=(1<<8);
        PTC-> PDDR&=~(1u<<display1[i]);
        PTC-> PSOR&=~(1u<<display1[i]);
    }
}
void ContAscendente(int Dec, int Uni){
    for (i=0; i<7;i++){
        for (j=0; j<10;j++){
            if((i!=9)&&(j!=10)){
                if(PTC->PDIR&(1u<<display1[9])==1){
                    PTC->PTOR(1<<display1[i]);
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                }else if(PTC->PDIR&(1u<<display2[9])==1){
                    PTC->PTOR(1<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }else {
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }
            }else if((i==9)&&(j==10)){
                if(PTC->PDIR&(1u<<display1[9])==1){
                    PTC->PTOR(1<<display1[i]);
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                }else if(PTC->PDIR&(1u<<display2[9])==1){
                    PTC->PTOR(1<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }else {
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }
            }
        }
    }
}
void InitContDescendente(){
    for(i=0; i<7; i++){
        PORTC-> PCR[display2[i]]|=(1<<8);
        PTC-> PDDR&=~(1u<<display2[i]);
        PTC-> PSOR&=~(1u<<display2[i]);
    }
}
void ContDescendente(int Dec, int Uni){
    for (i=7; i>0;i--){
        for (j=10; j>0;j--){
            if((i!=0)&&(j!=0)){
                if(PTC->PDIR&(1u<<display1[9])==1){
                    PTC->PTOR(1<<display1[i]);
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                }else if(PTC->PDIR&(1u<<display2[9])==1){
                    PTC->PTOR(1<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }else {
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }
            }else if((i==0)&&(j==0)){
                if(PTC->PDIR&(1u<<display1[9])==1){
                    PTC->PTOR(1<<display1[i]);
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                }else if(PTC->PDIR&(1u<<display2[9])==1){
                    PTC->PTOR(1<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }else {
                    PTC->PTOR(bcd7[i][j]<<display2[i]);
                    PTC->PTOR(bcd7[i][j]<<display1[i]);
                }
            }
        }
    }
}
void InitLeds(){
    PORTC-> PCR[20]|=(1<<8);
    PTC-> PDDR|=(1u<<20);
    PTC-> PSOR&=~(1u<<20);
    PORTC-> PCR[21]|=(1<<8);
    PTC-> PDDR|=(1u<<21);
    PTC-> PSOR&=~(1u<<21);
}
void leds(){
    if(ContAscendente(9, 9)){
        PTA->PTOR(1u<<21);
    }else if(ContDescendente(9, 9)){
        PTA->PTOR(1u<<20);
        for(int t=1000000; t>0; t--);
        if (select == 0b00){
            if (((PTA->PDIR&(1u<<21))==1)&&((PTA->PDIR&(1u<<20))==0)){
                PTA->PTOR(1u<<21);
            }else if (((PTA->PDIR&(1u<<21))==0)&&((PTA->PDIR&(1u<<20))==1)){
                PTA->PTOR(1u<<20);
            }else if (((PTA->PDIR&(1u<<21))==1)&&((PTA->PDIR&(1u<<20))==1)){
                PTA->PTOR(1u<<20);
                PTA->PTOR(1u<<21);
            }
        }
    }
}