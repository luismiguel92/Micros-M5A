/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on August 27, 2020, 6:58 PM
 */


#include <xc.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = INTOSCIO_EC

void main(void) {
    int conv;
    int c;
    int f;
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001110;
    TRISA=0x0f;
    TRISEbits.RE0=0;
    TRISEbits.RE1=0;
    PORTA=0x00;
    PORTEbits.RE0=0;
    PORTEbits.RE1=0;
    ADCON0bits.ADON=1;
    ADCON2bits.ACQT=4;
    ADCON2bits.ADCS=1;
    ADCON2bits.ADFM=1;
    
    while (1){
        ADCON0bits.GO_DONE=1;
        ADCON0bits.CHS=0x00;
        conv = (ADRESH*256)+ADRESL;
        c = (((conv*100)+0.0)/1023)*5;
        f = ((c*9)/5)+32;
        
        if(f>176){
            PORTEbits.RE0=1;
        }
        else{
            PORTEbits.RE0=0;
        }
        
        if((f<176)&&(PORTAbits.RA1==0)&&(PORTAbits.RA2==1)&&(PORTAbits.RA3==0)){
            PORTEbits.RE1=1;
        }
        else if ((f<176)&&(PORTAbits.RA1==1)&&(PORTAbits.RA2==0)&&(PORTAbits.RA3==1)){
            PORTEbits.RE1=1;
        }
        else if ((f>176)&&(PORTAbits.RA1==0)&&(PORTAbits.RA2==0)&&(PORTAbits.RA3==1)){
            PORTEbits.RE1=1;
        }
        else if ((f>176)&&(PORTAbits.RA1==1)&&(PORTAbits.RA2==1)&&(PORTAbits.RA3==0)){
            PORTEbits.RE1=1;
        }
        else {
            PORTEbits.RE1=0;
        }
    }
    return;
}
