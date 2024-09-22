#ifndef CircularBuffer_H
#define	CircularBuffer_H

#include <xc.h> // include processor files - each processor file is guarded.

#define MAX_SIZE 50 // Maximum size of the circular buffer

typedef struct {
    char buffer[MAX_SIZE];
    int readPointer;
    int writePointer;
} CircularBuffer ;
CircularBuffer CirBufTx;
CircularBuffer CirBufRx;

void circularBufferInit(CircularBuffer *cb) {
    cb->readPointer = 0;
    cb->writePointer = 0;
}

// Writing data in the circular buffer
void circularBufferWrite(CircularBuffer *cb, char value) {
    cb->buffer[cb->writePointer] = value;
    cb->writePointer = (cb->writePointer + 1) % MAX_SIZE;
}

// Reading data from circular buffer
char circularBufferRead(CircularBuffer *cb) {
    char value = cb->buffer[cb->readPointer];
    cb->readPointer = (cb->readPointer + 1) % MAX_SIZE;
    return value;
}

//to checkout if there are data available in the buffer to read 
// becomes greater than zero if data is available and zero if there is no data
int checkFreeBytes(CircularBuffer *cb) {
    int availableBytes;
    if (cb->writePointer >= cb->readPointer)
        availableBytes = cb->writePointer - cb->readPointer;
    else
        availableBytes = MAX_SIZE - (cb->readPointer - cb->writePointer);

    return availableBytes;
}


#endif	/* CircularBuffer_H */