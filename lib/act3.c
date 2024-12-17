
/*
4. Se solicita una solución programable para que la placa KL25Z funcione de la siguiente
manera. Se deberá realizar un demultiplexor de 1 a 2 donde su entrada selectora se maneja
mediante un pulsador y su entrada estarán a un ‘1’ lógico enviado por el microcontrolador. Si se
selecciona la primera entrada se activará un contador descendente el cuál será mostrado a
través de dos contadores BCD a sus correspondientes displays 7 segmentos, indicando también
que estos displays están encendidos mediante un led verde, que, cuando llegue a su número
máximo se resetee y arranque nuevamente.
Si por el contrario, se selecciona la segunda entrada se deberá habilitar un contador conectado a
otros dos BCD con sus respectivos displays, donde se hará una cuenta ascendente desde el
número “99” y, también, se encenderá un led rojo y se reiniciará. Cómo último en el pin 12 del
puerto D habrá un pulsador que reiniciará a los dos pulsadores, poniéndolos a los dos en “00”,
para luego esperar a que se elija en el demultiplexor.
Para realizar llevar a cabo este código deberán crear una librería que contenga las siguientes
funciones
• Función del demultiplexor, que contendrá la inicialización, funcionalidad.
• Una función para cada contador (ascendente y descendente) con su inicialización
correspondiente.
• Una función para el control de leds (estilo DigitalWrite)
Por último, recuerden que se deberá volcar lo necesario para que funcione dentro de un código
fuente y que, salvo lo que se especifique anteriormente, todo sucede dentro del puerto C.

*/

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
