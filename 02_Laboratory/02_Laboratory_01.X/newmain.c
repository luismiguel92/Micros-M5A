/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on August 27, 2020, 5:32 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = INTOSCIO_EC

void main(void) {
    int conv;
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001110;
    TRISAbits.RA0=1;
    TRISB=0b00000000;
    TRISD=0b00000000;
    TRISEbits.RE0=0;
    PORTAbits.AN0=0;
    PORTB=0b00000000;
    PORTEbits.RE0=0;
    PORTD=0b00000000;
    ADCON0bits.ADON=1;
    ADCON2bits.ACQT=4;// TAD=8*125ns=1us, Tacq=2.45us, Tacq=2.45us/1us, Tacq=2.45TAD >> 4TAD
    ADCON2bits.ADCS=1;//Tosc=1/8MHz=125ns, TAD=0.8us, TAD/Tosc=0.8us/125ns, 1 TAD=6.4Tosc >> 8Tosc
    ADCON2bits.ADFM=0;
    
    while (1){
        ADCON0bits.CHS=0x00;
        ADCON0bits.GO_DONE=1;
        conv = (ADRESH*50)/255;
        PORTD=ADRESH;
        PORTB = ((conv/10)*16)+(conv%10);
                
        if(conv>25){
           PORTEbits.RE0=1; 
        }
        else{
           PORTEbits.RE0=0; 
        }
    }
    return;
}