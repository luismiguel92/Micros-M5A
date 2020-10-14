/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on September 27, 2020, 12:52 PM
 */


#include <xc.h>
#pragma config FOSC = INTOSCIO_EC

void ExtInt_Init(void){
    INTCON2bits.INTEDG0=1;
    INTCON2bits.INTEDG1=1;
    INTCONbits.INT0IF=0;
    INTCONbits.INT0IE=1;
    INTCON3bits.INT1IF=0;
    INTCON3bits.INT1IE=1;       
}

void Timer1_Init(void){
    INTCONbits.GIE=0;
    INTCONbits.PEIE=1;
    PIR1bits.TMR1IF=0;
    PIE1bits.TMR1IE=1;
    T1CONbits.RD16=0;
    T1CONbits.T1RUN=0;
    T1CONbits.T1CKPS=0;
    T1CONbits.T1OSCEN=0;
    T1CONbits.T1SYNC=0;
    T1CONbits.TMR1CS=1;
    T1CONbits.TMR1ON=1;
    TMR1L=0;
    INTCONbits.GIE=1;
}

void Port_Init(void){
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001111;
    TRISB=0b00000001;
    TRISC=0b00000001;
    TRISD=0;
    TRISEbits.RE0=0;
    PORTCbits.RC2=1;
    PORTB=0;
    PORTC=0;
    PORTD=0;
    PORTEbits.RE0=0;
}

void main(void) {
    Timer1_Init();
    ExtInt_Init();
    Port_Init();
    
    while(1){
        PORTCbits.RC2=0;
        PORTD=((TMR1L/10)*16)+(TMR1L%10);
        if(TMR1L>30){
        PORTEbits.RE0=1;
        }
        else{
        PORTEbits.RE0=0;    
        }
    }
    return;
}

void __interrupt()ISR(void){
    if(INTCONbits.INT0IF==1){
        PORTCbits.RC2=1;
        INTCONbits.INT0IF=0;
    }
    if(INTCON3bits.INT1IF==1){
        if(TMR1L>0){
            TMR1L--;
        }     
        INTCON3bits.INT1IF=0;
    }
}