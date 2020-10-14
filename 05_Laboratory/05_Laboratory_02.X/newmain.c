/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on October 12, 2020, 4:06 PM
 */

#pragma config FOSC = INTOSCIO_EC
#define _XTAL_FREQ 8000000
#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include <stdlib.h>
//#define RAND_MAX 255

int flag=0;

void ExtInt_Init(void){
    INTCONbits.GIE=0;
    INTCON2bits.INTEDG0=1;
    INTCON2bits.INTEDG1=1;
    INTCON2bits.INTEDG2=1;
    INTCONbits.INT0IF=0;
    INTCON3bits.INT1IF=0;
    INTCON3bits.INT2IF=0;
    INTCONbits.INT0IE=1;
    INTCON3bits.INT1IE=1;
    INTCON3bits.INT2IE=1;
    INTCONbits.GIE=0;
}

void Port_Init(void){
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001111;
    TRISB=0b00000111;
    PORTB=0;
}

void main(void) {
    int rnum[4]={10,10,10,10};
    int unum[4]={6,0,8,7};
    char str[4];
    ExtInt_Init();
    Port_Init();
    lcd_init();
    lcd_clear_display();
    lcd_print("Aleatorio");
    lcd_clear_display();
    for(int i=5;i<9;i++){
            sprintf(str,"%i",unum[i-5]);
            lcd_print_with_position(i,1,str);
            //__delay_ms(1000);
    }
    lcd_print_with_position(0,2,"es: ");
   
    while(1){
        int picas=0;
        int fijas=0;
        srand(rand());
        for(int i=0;i<4;i++){
            int x=rand()%10;
            while((x==rnum[0])||(x==rnum[1])||(x==rnum[2])||(x==rnum[3])){
                x=rand()%10;
            }
            rnum[i]=x;
        }
        for(int i=5;i<9;i++){
            sprintf(str,"%i",rnum[i-5]);
            lcd_print_with_position(i,2,str);
            __delay_ms(1000);
        }
        
        
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if((i==j)&&(rnum[i]==unum[j])){
                    fijas++;
                }
                if((i!=j)&&(rnum[i]==unum[j])){
                    picas++;
                }
            }
        }
        sprintf(str,"%i",fijas);
        lcd_print_with_position(0,1,str);
        sprintf(str,"%i",picas);
        lcd_print_with_position(3,1,str);
        __delay_ms(2000);
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