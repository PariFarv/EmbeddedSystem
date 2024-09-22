
#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded.

#define FOSC 144000000
#define PreScaler 256

void tmr_setup_period(int timer , int ms){

    if (timer == 1) {
        
        TMR1 = 0 ; //Reset timer1
        T1CONbits.TON = 0 ; //Disable timer1
        T1CONbits.TCKPS = 0b11 ; //Set prescaler to 256
        T1CONbits.TCS = 0b00 ; //Set clock source to external
        PR1 = ((FOSC /2/ PreScaler) / 1000) * ms - 1 ; //Set the register
        T1CONbits.TON = 1 ; //Starts timer1
    }
    
    
    else if (timer == 2) {
        
        TMR2 = 0 ; //Reset timer2
        T2CONbits.TON = 0 ; //Disable timer2
        T2CONbits.TCKPS = 0b11 ; //Set prescaler to 256
        T2CONbits.TCS = 0b00 ; //Set clock source to external
        PR2 = ((FOSC /2/ PreScaler) / 1000) * ms - 1; //Set the register   
        T2CONbits.TON = 1 ; //Starts timer2        
    }
}


void tmr_wait_period(int timer){
    if (timer == 1){
        while(!IFS0bits.T1IF); //Waits until timer1 flag becomes 1
        IFS0bits.T1IF = 0; //Manually set the flag to 0 again
    }
    
    else if (timer == 2){
        while(!IFS0bits.T2IF); //Waits until timer2 flag becomes 1
        IFS0bits.T2IF = 0; //Manually set the flag to 0 again
    }
}


void tmr_wait_ms(int timer, int ms){
    tmr_setup_period(timer, ms);
    tmr_wait_period(timer);
}


#endif	/* TIMER_H */