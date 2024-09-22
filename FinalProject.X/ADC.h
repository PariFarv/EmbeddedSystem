#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.

void ADC_setup () {
    
    // ADC
    TRISBbits.TRISB15 = 1 ; // ADC AN15 input
    TRISAbits.TRISA3 = 0 ; // ADC Enable pin 
    TRISBbits.TRISB11 = 1 ; // ADC AN11 input
    
    // Configure ADC
    AD1CON3bits.ADCS = 7 ; // 8*TCY = TAD
    //AD1CON1bits.ASAM = 0 ; // manual sampling start
    AD1CON1bits.ASAM = 1 ; // Automatic sampling start
    //AD1CON1bits.SSRC = 0 ; // Manual sampling end 
    AD1CON1bits.SSRC = 7 ; // Automatic sampling end 
    
   
    AD1CON2bits.CSCNA = 1 ; // Scans inputs
    AD1CSSLbits.CSS11 = 1 ; //Input scan select AN11
    AD1CSSLbits.CSS15 = 1 ; //Input scan select AN15
    
    
    ANSELBbits.ANSB15 = 1 ; // Analog register map (datasheet page 115 table 4-56)
    ANSELBbits.ANSB11 = 1 ; // Analog register map (datasheet page 115 table 4-56)
    AD1CON1bits.SIMSAM = 0 ; // Sample multiple channels
    AD1CON2bits.SMPI = 1 ; // After 2nd conversion IFx sets to 1
    AD1CON1bits.ADON = 1 ; // Turn ADC on
    LATAbits.LATA3 = 1 ; //En pin
}

#endif	/* ADC_H */