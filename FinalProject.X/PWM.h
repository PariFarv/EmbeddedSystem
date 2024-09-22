#ifndef PWM_H
#define	PWM_H

#include <xc.h> // include processor files - each processor file is guarded.

#define FOSC 144000000
#define PWM_freq 10000


void PWM_init(){
    
    TRISDbits.TRISD1 = 0; //PWM/RP65-output
    TRISDbits.TRISD2 = 0; //PWM/RP66-output
    TRISDbits.TRISD3 = 0; //PWM/RP67-output
    TRISDbits.TRISD4 = 0; //PWM/RP68-output
        
    //PWM Setting
    RPOR0bits.RP65R = 0b010000; //Page261 register11-44 / Page215 table11-3 / OC1
    RPOR1bits.RP66R = 0b010001; //Page262 register11-45 / Page215 table11-3 / OC2
    RPOR1bits.RP67R = 0b010010; //Page262 register11-45 / Page215 table11-3 / OC3
    RPOR2bits.RP68R = 0b010011; //Page262 register11-46 / Page215 table11-3 / OC4
    
    OC1CON1bits.OCTSEL = 0b111; //Peripheral Clock
    OC1CON1bits.OCM = 0b110; //Edge-aligned PWM
    OC1CON2bits.SYNCSEL = 0b11111; //No sync source
    
    OC2CON1bits.OCTSEL = 0b111; //Peripheral Clock
    OC2CON1bits.OCM = 0b110; //Edge-aligned PWM
    OC2CON2bits.SYNCSEL = 0b11111; //No sync source
    
    OC3CON1bits.OCTSEL = 0b111; //Peripheral Clock
    OC3CON1bits.OCM = 0b110; //Edge-aligned PWM
    OC3CON2bits.SYNCSEL = 0b11111; //No sync source
    
    OC4CON1bits.OCTSEL = 0b111; //Peripheral Clock
    OC4CON1bits.OCM = 0b110; //Edge-aligned PWM
    OC4CON2bits.SYNCSEL = 0b11111; //No sync source
    
    OC1RS = (FOSC/2) / PWM_freq; //Sets the PWM frequency
    OC2RS = (FOSC/2) / PWM_freq; //Sets the PWM frequency
    OC3RS = (FOSC/2) / PWM_freq; //Sets the PWM frequency
    OC4RS = (FOSC/2) / PWM_freq; //Sets the PWM frequency
    
    OC1R = 0;
    OC2R = 0;
    OC3R = 0;
    OC4R = 0;
}

void PWM_setup (float RD1 , float RD2 , float RD3 , float RD4) {
    
    OC1R = (int) (OC1RS * (RD1));
    OC2R = (int) (OC2RS * (RD2));
    OC3R = (int) (OC3RS * (RD3));
    OC4R = (int) (OC4RS * (RD4));
}

#endif	/* PWM_H */