
#include "Alarm.h"

using namespace std;

//bool playing = true; // should be set false when paused
bool stopped = false; // If set to true, program should close
unsigned char buffer[2][BUFFER_SIZE][2];
int buffer_location = 0;
bool bufferReading = 0; //using this to switch between column 0 and 1 - the first column
bool threadReady = false; //using this to finish writing the first column at the start of the song, before the column is played



long lastInterruptTime =0 ; //initializing last interrupt value

void play(void)
{
     stopped=false;
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     { 
        //playing = !playing;
        // Call the setup GPIO function
           if(setup_gpio()==-1)
           {
                return 0;
           }


    //Write your logic here
          pthread_attr_t tattr;
          pthread_t thread_id;
          int newprio = 97;
          sched_param param;

          pthread_attr_init (&tattr); /* initialized with default attributes */
          pthread_attr_getschedparam (&tattr, &param); /* safe to get existing scheduling param */
          param.sched_priority = newprio; /* set the priority; others are unchanged */
          pthread_attr_setschedparam (&tattr, &param); /* setting the new scheduling param */
          pthread_create(&thread_id, &tattr, playThread, (void *)1); /* with new priority specified */



          char ch;
          FILE *filePointer;
          printf("%s\n", FILENAME);
          filePointer = fopen(FILENAME, "r"); // read mode

          if (filePointer == NULL) 
          {
                perror("Error while opening the file.\n");
                exit(EXIT_FAILURE);
          }

          int counter = 0;
          int bufferWriting = 0;

          // Have a loop to read from the file
          while((ch = fgetc(filePointer)) != EOF)
          {
                while(threadReady && bufferWriting==bufferReading && counter==0){
                //waits in here after it has written to a side, and the thread is still reading from the other side
                continue;
          }


          char value = fgetc(filePointer);
       
          char upper = ((value >> 6) & 0xff) | 0b0 << 7 | 0b0 << 6 | 0b1 << 5 | 0b1 << 4;
          char lower = value << 2 & 0b11111100;
          counter++;
          if(counter >= BUFFER_SIZE+1)
          {
              if(!threadReady)
              {
                threadReady = true;
              }

              counter = 0;
              bufferWriting = (bufferWriting+1)%2;
          }
          
    

          // Close the file
          fclose(filePointer);
          printf("Complete reading");
          //Join and exit the playthread
          pthread_join(thread_id, NULL);
          pthread_exit(NULL);
        
     }
     
     lastInterruptTime=interrupt_time;

}



void stop(void)
{
     long interrupt_time = millis();
     if ( interrupt_time - lastInterruptTime > 60 )
     {
        //cout<<"stopped"<<endl;
       stopped = true;
       //exit(0);
     }
     
     lastInterruptTime=interrupt_time;

}



void *playThread(void *threadargs)
{
    // If the thread isn't ready, don't do anything
    while(!threadReady)
    {
        continue;
    }

    //You need to only be playing if the stopped flag is false
    while(!stopped)
    {
        
        wiringPiSPIDataRW (SPI_CHAN, buffer[bufferReading][buffer_location], 2) ;

        //Do some maths to check if you need to toggle buffers
        buffer_location++;
        if(buffer_location >= BUFFER_SIZE) 
        {
            buffer_location = 0;
            bufferReading = !bufferReading; // switches column one it finishes one column
        }
        
    }

    pthread_exit(NULL);
}
    
