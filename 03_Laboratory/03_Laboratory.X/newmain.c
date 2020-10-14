/*
 * File:   newmain.c
 * Author: luism
 *
 * Created on September 6, 2020, 6:47 PM
 */


#include <xc.h>
#include <pic18f4550.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = INTOSCIO_EC
int dec=0;
int uni=0;
int cont=0;
int flag=0;
int time=0;
int conv=0;
int c=0;
int num=0;

void main(void) {
    OSCCONbits.IRCF=0b111;
    ADCON1=0b00001110;
    ADCON0bits.ADON=1;
    ADCON2bits.ACQT=4;//TAD=8*125ns=1us, Tacq=2.45us, Tacq=2.45us/1us, Tacq=2.45TAD >> 4TAD
    ADCON2bits.ADCS=1;//Tosc=1/8MHz=125ns, TAD=0.8us, TAD/Tosc=0.8us/125ns, 1 TAD=6.4Tosc >> 8Tosc
    ADCON2bits.ADFM=1;
    ADCON0bits.CHS=0x00;
    INTCON2bits.NOT_RBPU=0;
    TRISAbits.RA0=1;
    TRISB=0b11110001;
    TRISD=0b00000000;
    TRISEbits.RE0=0;
    TRISEbits.RE1=0;    
    PORTAbits.AN0=0;
    PORTB=0b00001110;
    PORTD=0b00000000;
    PORTEbits.RE0=0;
    PORTEbits.RE1=0;
    INTCONbits.GIE=1;
    INTCONbits.INT0IE=1;
    INTCON2bits.INTEDG0=1;
    INTCONbits.INT0IF=0;
    INTCONbits.RBIE=1;
    INTCONbits.RBIF=0;
    
    while(1){
        ADCON0bits.GO_DONE=1;
        PORTB=0b11111100;
        PORTB=0b11111010;
        PORTB=0b11110110;
        conv = (ADRESH*256)+ADRESL;
        c = (((conv*100)+0.0)/1023)*5;
        if(c>=60){
            PORTEbits.RE1=1;
        }
        else{
            PORTEbits.RE1=0;
        }
        if(flag==1){
            PORTD=((time/10)*16)+(time%10);
            PORTEbits.RE0=1;
            __delay_ms(1000);
            time--;
            if(time==0){
                flag=0;
            }
        }
        else if(flag==2){
            PORTD=((time/10)*16)+(time%10);
            PORTEbits.RE0=0;
        }
        else if(flag==3){
            time=0;
            PORTE=0b00000000;
            for(int i=0;i<=9;i++){
                PORTD=(i*16)+(i);
                __delay_ms(1000);
            }
            for(int i=0;i<3;i++){
                PORTE=0b00000011;
                __delay_ms(500);
                PORTE=0b00000000;
                __delay_ms(500);
            }
            flag=0;
        }
        else{
            PORTD=((time/10)*16)+(time%10);
            PORTEbits.RE0=0;    
        }
    }
    return;
}

void __interrupt() teclado(void){
    if(INTCONbits.RBIF==1){
        if(PORTBbits.RB1==0){
            if(PORTBbits.RB4==0){num=1;cont++;}
            if(PORTBbits.RB5==0){num=4;cont++;}
            if(PORTBbits.RB6==0){num=7;cont++;}
            if(PORTBbits.RB7==0){cont=0;flag=1;} //*
        }
        if(PORTBbits.RB2==0){
            if(PORTBbits.RB4==0){num=2;cont++;}
            if(PORTBbits.RB5==0){num=5;cont++;}
            if(PORTBbits.RB6==0){num=8;cont++;}
            if(PORTBbits.RB7==0){num=0;cont++;}
        }
        if(PORTBbits.RB3==0){
            if(PORTBbits.RB4==0){num=3;cont++;}
            if(PORTBbits.RB5==0){num=6;cont++;}
            if(PORTBbits.RB6==0){num=9;cont++;}
            if(PORTBbits.RB7==0){cont=0;flag=2;time++;} //#
        }
        while(PORTBbits.RB4==0||PORTBbits.RB5==0||PORTBbits.RB6==0||PORTBbits.RB7==0);
        INTCONbits.RBIF=0;
    }   
    if(cont==1){
        uni=num;
        time=(dec*10)+(uni);
    }
    if(cont==2){
        dec=uni;
        uni=num;
        time=(dec*10)+(uni);
        cont=0;
        dec=0;
        uni=0;
    }
    if(INTCONbits.INT0IF==1){
        flag=3;
        INTCONbits.INT0IF=0;
    }
}