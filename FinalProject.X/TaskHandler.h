#ifndef TaskHandler_H
#define	TaskHandler_H

#include <xc.h> // include processor files - each processor file is guarded.

#define MAX_TASKS 3
#define MAX_COMMANDS 10

#define Wait_for_start 0
#define Execute 1

int Control_state = 0 ;
int distance ;
int elapsed = 0;

typedef struct {
    int n;
    int N;
} heartbeat;
heartbeat schedInfo[MAX_TASKS];

typedef struct {
    int action;
    int interval;
} Command;

Command commandQueue[MAX_COMMANDS];
int commandQueueStart = 0;
int commandQueueEnd = 0;
int commandQueueSize = 0;

//Read the battery value and send it to UART
void task_Battery_sensor (){
    
    int Battery_value = ADC1BUF0 ; // Read the value of the Battery sensor
    double Battery_volt = (Battery_value/1023.0)*3.3 ;  //
    double V_bat = Battery_volt*300.0/100.0 ;
    
    char buff[50];
    sprintf(buff, "$MBATT,%.2f*", V_bat); // Sending the battery info in desired format
    for (int i = 0; i < strlen(buff); i++)
    {
        circularBufferWrite(&CirBufTx, buff[i]); // put the data in the circular buffer
    }
    IFS1bits.U2TXIF = 1; // Activate the UART interrupt manually
}

//Read the IR sensor value and calculate the distance
void task_Dist_sensor_read (){
    
    int IR_value = ADC1BUF1 ; //IR value in bits
    float IR_volt = (IR_value/1023.0)*3.3 ;  //IR value in volts
    distance = (int) ((2.34 - (4.74*pow(IR_volt, 1)) + (4.06*pow(IR_volt, 2)) - (1.6*pow(IR_volt, 3)) + (0.24*pow(IR_volt, 4)))*100);  
}

//Send the distance value to UART
void task_Dist_sensor_send (){
    
    char buff1[50];
    sprintf(buff1, "$MDIST,%d*", distance); // Sending the distance in desired format
    for (int i = 0; i < strlen(buff1); i++)
    {
        circularBufferWrite(&CirBufTx, buff1[i]); // put the data in the circular buffer
    }
    IFS1bits.U2TXIF = 1; // Activate the UART interrupt manually
}

//Handle LEDs
void task_blink_LED (){
    
    if (Control_state == Wait_for_start){     
        LATAbits.LATA0 ^= 1 ; //LED A0 is blinking
        LATBbits.LATB8 ^= 1 ; //Left indicator is blinking
        LATFbits.LATF1 ^= 1 ; //Right indicator is blinking
        
    }else if (Control_state == Execute){
        
        LATAbits.LATA0 ^= 1 ; //LED A0 is blinking
        LATBbits.LATB8 = 0 ; //Left indicator stops blinking
        LATFbits.LATF1 = 0 ; //Right indicator stops blinking
    }
}

// Process new commands from the circular buffer
void Enqueue_Command (){
    
    while (checkFreeBytes(&CirBufRx)) { //Check if there are commands in circular buffer
        int ret = parse_byte(&pstate, circularBufferRead(&CirBufRx)); // parse the byte

        if (ret == NEW_MESSAGE) { //Check the new message is received
            if (strcmp(pstate.msg_type, "PCCMD") == 0) { // check the type of the message
                if (commandQueueSize < MAX_COMMANDS) {
                    // Enqueue the command
                    sscanf(pstate.msg_payload, "%d,%d", &commandQueue[commandQueueEnd].action, &commandQueue[commandQueueEnd].interval);
                    commandQueueEnd = (commandQueueEnd + 1) % MAX_COMMANDS;
                    commandQueueSize++;

                    // Send acknowledgment
                    char ack[50];
                    sprintf(ack, "$MACK,1*");
                    for (int i = 0; i < strlen(ack); i++) {
                        circularBufferWrite(&CirBufTx, ack[i]);
                    }
                }else if (commandQueueSize == MAX_COMMANDS){
                    // Queue is full, send error acknowledgment
                    char eack[50];
                    sprintf(eack, "$MACK,0*");
                    for (int i = 0; i < strlen(eack); i++) {
                        circularBufferWrite(&CirBufTx, eack[i]);
                    }
                    // Activate the UART interrupt manually
                    IFS1bits.U2TXIF = 1;
                }
            }
        }
    }
}

//Dequeue and execute the commands
void State_Control (){
    
    if (Control_state == Wait_for_start) {
        PWM_setup(0, 0, 0, 0);
    } else if (Control_state == Execute) {
        if (commandQueueSize > 0) { // Execute the command if available
            int action = commandQueue[commandQueueStart].action;
            int interval = commandQueue[commandQueueStart].interval;
            
            switch (action) {
                case 1:
                    PWM_setup(0, 0.7, 0, 0.7); //Moving Forward
                    break;
                case 2:
                    PWM_setup(0.7, 0, 0, 0.7); //Moving Counterclockwise
                    break;
                case 3:
                    PWM_setup(0, 0.7, 0.7, 0); //Moving Clockwise
                    break;
                case 4:
                    PWM_setup(0.7, 0, 0.7, 0); //Moving Backward
                    break;
                default:
                    PWM_setup(0, 0, 0, 0); // Default action to stop motors
                    break;
            }
            if (elapsed < interval) {
                if (distance < 20 && action == 1){ //In "Moving Forward" state, check the distance
                    PWM_setup(0, 0, 0, 0); // Stop the motors
                }
                elapsed++;
            } else if (elapsed >= interval) {
                PWM_setup(0, 0, 0, 0); // Stop the motors
                elapsed = 0; // Reset elapsed counter
                // Move the start index of the circular queue
                commandQueueStart = (commandQueueStart + 1) % MAX_COMMANDS;
                commandQueueSize--;
            }     
        }
    }
}

//Control Motors
void task_Motor_control() {
    
    Enqueue_Command ();
    State_Control ();  
}

void task_scheduler(){
    schedInfo[0].N = 1; // 1 KHz
    schedInfo[1].N = 100; // 10 Hz
    schedInfo[2].N = 1000; // 1 Hz
}

void scheduler() {
    for (int i = 0; i < MAX_TASKS; i++) {
        schedInfo[i].n++;
        if(schedInfo[i].n >= schedInfo[i].N){
            switch(i){
                case 0:
                    task_Motor_control() ;
                    task_Dist_sensor_read () ;
                    break;    
                case 1:
                    task_Dist_sensor_send () ;
                    break;
                case 2:                       
                    task_blink_LED ();
                    task_Battery_sensor () ;
                    break;  
            }
            schedInfo[i].n = 0;
        }
    }
}

#endif	/* TaskHandler_H */