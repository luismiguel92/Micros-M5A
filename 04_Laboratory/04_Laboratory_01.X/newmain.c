/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on September 23, 2020, 5:16 PM
 */


#include <xc.h>
#pragma config FOSC = INTOSCIO_EC
int dec=0;
int sec=0;
int min=0;

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
    T0CONbits.T0PS=0b001;
    T0CONbits.T08BIT=0;
    TMR0H=(15536)>>8;
    TMR0L=15536;
    T0CONbits.TMR0ON=0;
    INTCONbits.TMR0IF=0;
    INTCONbits.TMR0IE=1;
    INTCONbits.GIE=1;
}

void Timer1_Init(void){
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
}

void Port_Init(void){
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001111;
    TRISA=0;
    TRISB=0b00000011;
    TRISC=0b00000001;
    TRISD=0;
    PORTA=0;
    PORTB=0;
    PORTC=0;
    PORTCbits.RC2=1;
    PORTD=0;
}

void main(void) {
    Timer0_Init();
    Timer1_Init();
    ExtInt_Init();
    Port_Init();
    while(1){
        PORTA=(TMR1L%10);
        PORTB=((TMR1L/10)*16);
        PORTC=(dec)<<4;
        PORTD=((sec/10)*16)+(sec%10);  
    }
    return;
}

void __interrupt()ISR(void){
    if(INTCONbits.TMR0IF==1){
        dec++;
        INTCONbits.TMR0IF=0;
        TMR0H=(15536)>>8;
        TMR0L=15536;
        if(dec==10){
            PORTCbits.RC2=0;
            dec=0;
            sec++;
        }
        if(sec==60){
            PORTCbits.RC2=1;
            sec=0;
        }
    }
    if(TMR1L==100){
        TMR1L=0;
    }
    if(INTCONbits.INT0IF==1){
        T0CONbits.TMR0ON=!T0CONbits.TMR0ON;
        INTCONbits.INT0IF=0;
    }
    if(INTCON3bits.INT1IF==1){
        T0CONbits.TMR0ON=0;
        sec=0;
        dec=0;
        TMR1L=0;        
        INTCON3bits.INT1IF=0;
    }
}