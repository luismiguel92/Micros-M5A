/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on September 27, 2020, 5:50 PM
 */


#include <xc.h>
#pragma config FOSC = INTOSCIO_EC
int sec=0;
int flag=0;

void ExtInt_Init(void){
    INTCON2bits.INTEDG0=1;
    INTCON2bits.INTEDG1=1;
    INTCONbits.INT0IF=0;
    INTCONbits.INT0IE=1;
    INTCON3bits.INT1IF=0;
    INTCON3bits.INT1IE=1;       
}

void Timer0_Init(void){
    INTCONbits.GIE=0;
    T0CONbits.T0CS=0;
    T0CONbits.PSA=0;
    T0CONbits.T0PS=0b100;
    T0CONbits.T08BIT=0;
    TMR0H=(3036)>>8;
    TMR0L=3036;
    T0CONbits.TMR0ON=1;
    INTCONbits.TMR0IF=0;
    INTCONbits.TMR0IE=1;
    INTCONbits.GIE=1;
}

void Port_Init(void){
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001111;
    TRISB=0b00000011;
    TRISD=0;
    PORTB=0;
    PORTD=0;
}

void main(void) {
    Timer0_Init();
    ExtInt_Init();
    Port_Init();
    while(1){
        PORTD=((sec/10)*16)+(sec%10);
        if(flag==0){
            if(sec==0){
                PORTBbits.RB3=0;
                PORTBbits.RB5=0;
                PORTBbits.RB2=1;
                PORTBbits.RB4=1;
            }
            if(sec==1){
                PORTBbits.RB2=0;
                PORTBbits.RB4=0;
                PORTBbits.RB3=1;
                PORTBbits.RB5=1;
            }
            if(sec>=2){
                sec=0;
            }
        }
        else{
            if(sec==0){
                PORTBbits.RB3=0;
                PORTBbits.RB5=0;
                PORTBbits.RB2=1;
                PORTBbits.RB4=1;
            }
            if(sec==15){
                sec=0;
                flag=0;                
            }
        }
    }
    return;
}

void __interrupt()ISR(void){
    if(INTCONbits.TMR0IF==1){
        sec++;
        INTCONbits.TMR0IF=0;
        TMR0H=(3036)>>8;
        TMR0L=3036;
    }   
    if(INTCONbits.INT0IF==1||INTCON3bits.INT1IF==1){
        flag=1;
        INTCONbits.INT0IF=0;
        INTCON3bits.INT1IF=0;
        sec=0;
    }
}
