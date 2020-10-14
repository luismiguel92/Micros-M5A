/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on September 23, 2020, 5:27 PM
 */


#include <xc.h>
#pragma config FOSC = INTOSCIO_EC
int time=0;

void Timer0_Init(void){
    INTCONbits.GIE=0;
    T0CONbits.T0CS=0;
    T0CONbits.PSA=1;
    T0CONbits.T08BIT=0;
    TMR0H=(45536)>>8;
    TMR0L=45536;
    T0CONbits.TMR0ON=1;
    INTCONbits.TMR0IF=0;
    INTCONbits.TMR0IE=1;
    INTCONbits.GIE=1;
}

void main(void) {
    OSCCONbits.IRCF=0b111;
    TRISDbits.RD0=0;
    TRISDbits.RD1=0;
    PORTDbits.RD0=0;
    PORTDbits.RD1=0;
    Timer0_Init();
    
    while(1){
        if(time==0){
            PORTDbits.RD0=1;
            PORTDbits.RD1=1;
        }
        if(time==5){
            PORTDbits.RD1=0;
        }
        if(time==8){
            PORTDbits.RD0=0;
        }
        if(time>=10){
            time=0;
        }   
    }
    return;
}

void __interrupt()ISR(void){
    if(INTCONbits.TMR0IF==1){
        time++;
        INTCONbits.TMR0IF=0;
        TMR0H=(45536)>>8;
        TMR0L=45536;
    }
}