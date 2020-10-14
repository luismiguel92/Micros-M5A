/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on October 11, 2020, 6:54 PM
 */

#pragma config FOSC = INTOSCIO_EC
#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdio.h>
#include "LCD.h"

int flag=0;

void ExtInt_Init(void){
    INTCONbits.GIE=1;
    INTCON2bits.INTEDG0=1;
    INTCONbits.INT0IF=0;
    INTCONbits.INT0IE=1;    
}

void ADCon_Init(void){
    ADCON1=0b00001110;
    ADCON0bits.ADON=1;
    ADCON2bits.ACQT=4;
    ADCON2bits.ADCS=1;
    ADCON2bits.ADFM=1;
    ADCON0bits.CHS=0x00; 
}

void Port_Init(void){
    OSCCONbits.IRCF=0b111;
    TRISA=0;
    TRISB=0b00000011;
    PORTA=0;
    PORTB=0;
}

void main(void) {
    int conv;
    int c;
    float f;
    float k;
    char str[3];
    ExtInt_Init();
    ADCon_Init();
    Port_Init();
    lcd_init();
    lcd_clear_display();
    lcd_print("La Temperatura");
    lcd_print_with_position(0,2,"es: ");
    char degree[8]={
        0b00000110,
        0b00001001,
        0b00001001,
        0b00000110,
    };
    lcd_put_caracter(0x00,degree);
    lcd_goto(10,2);
    lcd_dato(0x00);
    while(1){
        ADCON0bits.GO_DONE=1;
        conv = (ADRESH*256)+ADRESL;
        c = (((conv*100)+0.0)/1023)*5;
        f = (((c+0.0)*9)/5)+32;
        k = (c+0.0)+(273.15);
        switch(flag){ 
           case 1:
               sprintf(str,"%6.2f",k);
               lcd_print_with_position(4,2,str);
               lcd_print_with_position(11,2,"K");
               break; 
           case 2:
               sprintf(str,"%6.2f",f);
               lcd_print_with_position(4,2,str);
               lcd_print_with_position(11,2,"F");
               break; 
            default:
               sprintf(str,"%6.1i",c);
               lcd_print_with_position(4,2,str);
               lcd_print_with_position(11,2,"C");
               break;
        } 
    }
    return;
}

void __interrupt()ISR(void){
    if(INTCONbits.INT0IF==1){
        if(flag<2){
            flag++;
        }else{
            flag=0;
        }
        INTCONbits.INT0IF=0;
    }
}
