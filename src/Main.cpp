
/*
 * 
 * MANENO MGWAMI (MGWMAN006) AND KEVIN PIAO (PXXZHE001)
 */

#include "Main.h"

using namespace std;

//Global variables
int humidity=0;
int temperature=0;
int light=0;
int dacOut=0;
int tempC=0;
bool threadReady = false; 
long lastInterruptTime =0;

void start_stop (void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
     
     //to do 
     }
     lastInterruptTime=interrupt_time;

}

void change_interval(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
        //to do
     }
    lastInterruptTime=interrupt_time;

}

void dismiss_alarm(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
        //to do
     }
    lastInterruptTime=interrupt_time;

}

void reset_time(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
        //to do
     }
    lastInterruptTime=interrupt_time;

}

int setup_gpio(void)
{
    //Set up wiring Pi
    wiringPiSetup();
    
    //setting up the SPI interface
    wiringPiSPISetup(SPI_CHAN, SPI_SPEED);
    mcp3004Setup(BASE,SPI_CHAN);

    //setting up the buttons
    pinMode(CHANGE_INTERVAL_BUTTON, INPUT);
    pinMode(RESET_TIME_BUTTON, INPUT);
    pinMode(DISMISS_ALARM_BUTTON, INPUT);
    pinMode(STOP_START_BUTTON, INPUT);
    
    //Pull up
    pullUpDnControl(CHANGE_INTERVAL_BUTTON, PUD_UP);
    pullUpDnControl(RESET_TIME_BUTTON, PUD_UP);
    pullUpDnControl(DISMISS_ALARM_BUTTON, PUD_UP);
    pullUpDnControl(STOP_START_BUTTON, PUD_UP);
    
    
    //Main Thread
    wiringPiISR(CHANGE_INTERVAL_BUTTON,INT_EDGE_FALLING,change_interval);
    wiringPiISR(RESET_TIME_BUTTON,INT_EDGE_FALLING,reset_time);
    wiringPiISR(DISMISS_ALARM_BUTTON,INT_EDGE_FALLING,dismiss_alarm);
    wiringPiISR(STOP_START_BUTTON,INT_EDGE_FALLING,start_stop);


  
    return 0;
}

/*
 * Thread that handles writing to SPI
 *
 * You must pause writing to SPI if not playing is true (the player is paused)
  * When calling the function to write to SPI, take note of the last argument.
 * You don't need to use the returned value from the wiring pi SPI function
 * You need to use the buffer_location variable to check when you need to switch buffers
 */
void *adcThread(void *threadargs)
{
    // If the thread isn't ready, don't do anything
    while(!threadReady)
    {
        continue;
    }

    //Read adcValu
    humidity=analogRead(BASE+0);
    temperature=analogRead(BASE+1);
    light=analogRead(BASE+2);
    
    pthread_exit(NULL);
    
}


int main()
{
    // Call the setup GPIO function
    if(setup_gpio()==-1)
    {
        return 0;
    }

    /* Initialize thread with parameters
     * Set the play thread to have a 99 priority
     * Read https://docs.oracle.com/cd/E19455-01/806-5257/attrib-16/index.html
          */

    //Write your logic here
    pthread_attr_t tattr;
    pthread_t thread_id;
    int newprio = 99;
    sched_param param;

    pthread_attr_init (&tattr); /* initialized with default attributes */
    pthread_attr_getschedparam (&tattr, &param); /* safe to get existing scheduling param */
    param.sched_priority = newprio; /* set the priority; others are unchanged */
    pthread_attr_setschedparam (&tattr, &param); /* setting the new scheduling param */
    pthread_create(&thread_id, &tattr, adcThread, (void *)1); /* with new priority specified */
    
    while(1)
    {
       dacOut=(light*humidity)/(1023);
       tempC=(temperature-0.5)/0.01;
       
       if(dacOut < 0.65 || dacOut > 2.65)
       {
       
         //PLay alarm here
         
       }
       
       printf("%d",tempC);
       printf("%d",humidity);
       printf("%d",light);
       printf("%d",dacOut);
       
       //delay TO DO 
    }
    
    
    //Join and exit the playthread
    pthread_join(thread_id, NULL);
    pthread_exit(NULL);

    return 0;
}