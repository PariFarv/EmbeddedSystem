/*
 * File:   main.c
 * Author: Parisa Farvardin (S6101141), Vahid Bagherian (S6016232), 
 *         Mobina Alinaghian (S5905100), AmirMahdi Matin (S5884715)
 *
 * Created on June 7, 2024, 2:45 PM
 */

#include "xc.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "TIMER.h"
#include "PWM.h"
#include "Parser.h"
#include "ADC.h"
#include "CircularBuffer.h"
#include "UART.h"
#include "TaskHandler.h"

#define TIMER1 1
#define TIMER2 2

//Initialization of the functions
void tmr_setup_period(int timer , int ms) ;
void tmr_wait_period(int timer) ; 
void tmr_wait_ms(int timer, int ms) ; 
void tmr_wait_ms_long(int timer, int ms) ;
void LED_setup () ; 
void PWM_init() ;
void ADC_setup () ;
void circularBufferInit(CircularBuffer *cb) ; 
void circularBufferWrite(CircularBuffer *cb, char value) ; 
char circularBufferRead(CircularBuffer *cb) ;
int  checkFreeBytes(CircularBuffer *cb) ;
void UART_setup() ;
void scheduler() ; 
void task_scheduler();
void PWM_setup (float RD1 , float RD2 , float RD3 , float RD4) ; 
void task_blink_LED () ; 
void task_Motor_control() ; 
void task_Battery_sensor () ;
void task_Dist_sensor_read () ;
void task_Dist_sensor_send () ;
int parse_byte(parser_state* ps, char byte);
void parse_init() ; 
void Enqueue_Command ();
void State_Control ();

void LED_setup (){    
    // initialization LED outputs
    TRISAbits.TRISA0 = 0; // LED1
    TRISGbits.TRISG9 = 0; // LED2
    TRISBbits.TRISB8 = 0; // Left indicator
    TRISFbits.TRISF1 = 0; // Right indicator   
}

//Timer 2 Interrupt in order to solve bouncing problem
void __attribute__ (( __interrupt__ , __auto_psv__ )) _T2Interrupt() {    
    IFS0bits.T2IF = 0; //Resets timer2 flag
    
    IEC0bits.T2IE = 0; //Disables timer2 interrupt
    T2CONbits.TON = 0; //Disables timer2
    if (Control_state == 0){ // In "Wait_for_start" just toggle the state
        Control_state ^= 1 ;
        
    } else if (Control_state == 1){ // In "Execute" toggle the state and clear the FIFO
        Control_state ^= 1 ;
        commandQueueStart = 0;
        commandQueueEnd = 0;
        commandQueueSize = 0;        
    }  
} 

//External Interrupt1
void __attribute__ (( __interrupt__ , __auto_psv__ )) _INT1Interrupt() {
    IFS1bits.INT1IF = 0; //Resets external interrupt flag
    IEC0bits.T2IE = 1; //Enables timer2 interrupt
    
    tmr_setup_period(TIMER2, 10); //Waits 10ms in order to settle the switch bouncing and makes the flag 1
}

int main(void) {
    
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X000;
    
    TRISEbits.TRISE8 = 1; // Button1
    
    //External interrupt
    RPINR0bits.INT1R = 88; //Remap external interrupt1 to PR88
    INTCON2bits.GIE = 1; //Enables global interrupt (used for external interrupts)
    
    IFS1bits.INT1IF = 0; //Resets external interrupt1 flag
    IEC1bits.INT1IE = 1; //Enables external interrupt1 interrupt
    
    LED_setup ();
    ADC_setup ();
    UART_setup() ; 
    PWM_init() ; 
    parse_init();
    task_scheduler(); 
    
    tmr_setup_period(TIMER1, 1);
    while (1){
        
        scheduler() ;
        tmr_wait_period(TIMER1);  
    }
    return 0;
}
