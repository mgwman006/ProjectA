
#ifndef ALARM_H
#define ALARM_H

//Includes
#include <wiringPi.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <iostream>

//Define buttons
#define PLAY_BUTTON 4 // Write your value here
#define STOP_BUTTON 5 // Write your value here
#define BUFFER_SIZE 1000

//SPI Settings
#define SPI_CHAN 0// Write your value here
#define SPI_SPEED 220000// Write your value here

//Filename
#define FILENAME "src/sound_16k_8bit.raw"

//Function definitions
void play_audio(void);
void stop_audio(void);
int setup_gpio(void);
int main(void);


#endif