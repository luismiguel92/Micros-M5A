/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on August 15, 2020, 3:51 PM
 */

#include <xc.h>
#include <math.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = INTOSCIO_EC

void main(void) {
    OSCCONbits.IRCF=0b111;
    int mode=0;
    ADCON1=0b00001111;
    TRISA=0b00000100;
    TRISB=0b00000000;
    TRISD=0b00000000;
    PORTA=0b00000000;
    PORTB=0b00000000;
    PORTD=0b00000000;

    while(1){
    if (PORTAbits.RA2==1){
        mode++;
    } 

    switch (mode) {
        case 1:
            for (int i=7;i>=0;i--){
                PORTD=pow(2,i);
                __delay_ms(100);
            }
            for (int i=0;i<=7;i++){
                PORTD=pow(2,i);
                __delay_ms(100);
            }
        break;
        case 2 : 
            PORTD=0x00;
            for (int i=0;i<=7;i++){
                PORTB+=pow(2,i);
                __delay_ms(2000);
            }
            for (int i=7;i>=0;i--){
                PORTB-=pow(2,i);
                __delay_ms(2000);
            }
        break;
        case 3:
            for (int i=0;i<=128;i+=8){
                PORTA=i;
                __delay_ms(1000);
            }
        break;
        case 4:
            PORTAbits.RA1=1;
            __delay_ms(500);
            PORTAbits.RA1=0;
            __delay_ms(500);                
        break;
        default : 
            mode=0;
            __delay_ms(200);
        break;
    }
    }
    return;
}
