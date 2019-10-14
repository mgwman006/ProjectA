/*
 * BinClock.cpp
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019
 * 
 * <PXXZHE001> <MGWMAN006>
 * Date
*/

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h> //For printf functions
#include <stdlib.h> // For system functions
#include "BinClock.h"
//for interrupt
#include <sys/time.h>
//for exit loop
#include <stdbool.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>


//Global variables
int hours, mins, secs;
int dhours, dmins, dsecs;
long lastInterruptTime = 0;
long debounceTime = 300000; //Used for button debounce
struct timeval last_change;



int getSecs( void )
{
	return dsecs;
}


/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours)
{
	/*formats to 12h*/
	if (hours >= 24)
	{
		hours = 0;
	}
	else if (hours > 12)
	{
		hours -= 12;
	}
	return (int)hours;
}



int mFormat(int mins)
{
	/*formats to 60s*/
	if (mins >= 60)
	{
		mins = 0;
	}
	else if (hours > 60)
	{
		mins -= 60;
	}
	return (int)mins;
}



/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units)
{
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units)
{
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
void loopTime(void)
{
	for (;;)
	{
		
		//Fetch the time from the RTC
		hours = wiringPiI2CReadReg8(RTC, HOUR);
		mins = wiringPiI2CReadReg8(RTC, MIN);
		secs = wiringPiI2CReadReg8(RTC, SEC);
		dhours = hexCompensation(hours);
		dmins = hexCompensation(mins);
		secs = secs - 0x80;
		dsecs = hexCompensation(secs);

		printf("The current time is: %d:%d:%d\n", dhours, dmins, dsecs);	
			
	}
	
}


