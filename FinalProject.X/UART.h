#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.

#define BAUD 9600

void UART_setup() {
    
    TRISDbits.TRISD0 = 0; //TX/RP64-output
    TRISDbits.TRISD11 = 1; //RX/RP175-input
    
    //Setting for UART2
    long int REG_Val = (long int)((((double)FOSC/2) / (16.0 * (double) BAUD)) - 1);
    U2BRG = REG_Val; //Setting Baud Rate
    U2MODEbits.UARTEN = 1; //Enable UART2
    U2STAbits.UTXEN = 1; //Enable U2TX (must be after UARTEN)
    
    RPINR19bits.U2RXR = 75; //Input remappable pin RX pin in RPI75 (table 11-1 page 211)
    RPOR0bits.RP64R = 0b000011; //Output remappable pin TX in RP64 (register 11-44 page 261 & table 11-3 page 215)
    
    IFS1bits.U2TXIF = 0; //Reset the flag
    IEC1bits.U2TXIE = 1; //Enable the TX interrupt
    U2STAbits.UTXISEL0 = 0b01; // UART2 interrupt mode (1: every char received, 2: 3/4 char buffer, 3: full)
    
    IFS1bits.U2RXIF = 0; //Reset UART RX flag
    IEC1bits.U2RXIE = 1; //Enable the RX interrupt
}

// Interrupt to send data through UART2
// It checks if the transmit buffer is empty and the transmit circular buffer has 
// data to read 
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U2TXInterrupt() {
    IFS1bits.U2TXIF = 0;
    
    while (!U2STAbits.UTXBF && checkFreeBytes(&CirBufTx) > 0) {
        
        U2TXREG = circularBufferRead(&CirBufTx);
    } 
    
}

// Interrupt to receive data through UART2
void __attribute__ (( __interrupt__ , __auto_psv__ )) _U2RXInterrupt() {
    IFS1bits.U2RXIF = 0; //Reset UART RX flag
    
    //while (U2STAbits.URXDA == 0) ;
    circularBufferWrite(&CirBufRx, U2RXREG);
}


#endif	/* UART_H */