// Adaptive Thermostat
// By Adam Stebbing
// 01/23/2013
#include "mbed.h"
#include "TMP36.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "Speaker.h"
#include "Shiftbrite.h"

// Initialize temperature sensor pins
TMP36 myTMP36(p15);  //Analog in

// Initialize microSD card filesystem
SDFileSystem sd(p5, p6, p7, p8, "sd");

// Initialize the colored LCD screen
uLCD_4DGL uLCD(p28, p27, p29);

// Initialize pushbuttons pins
PinDetect pb1(p23);
PinDetect pb2(p24);
PinDetect pb3(p25);

// Initialize speaker
Speaker mySpeaker(p21);

// Initialize Shiftbrite pins
Shiftbrite myShiftbrite(p9, p10, p11, p12, p13);// ei li di n/c ci

// Initialize Mbed's four on-board LEDs
DigitalOut myLED1(LED1);
DigitalOut myLED2(LED2);
DigitalOut myLED3(LED3);
DigitalOut myLED4(LED4);

// Initialize  output pins
DigitalOut P16(p16);
DigitalOut P17(p17);
DigitalOut P18(p18);
DigitalOut P19(p19);
DigitalOut P20(p20);

// Global variables used in callbacks and main program
// C variables in interrupt routines should use volatile keyword
int volatile heat_setting=78; // heat to temp
int volatile cool_setting=68; // cool to temp
bool volatile mode=false; // heat or cool mode

// Callback routine is interrupt activated by a debounced pb1 hit
void pb1_hit_callback (void)
{
}
// Callback routine is interrupt activated by a debounced pb2 hit
void pb2_hit_callback (void)
{
}
// Callback routine is interrupt activated by a debounced pb3 hit
void pb3_hit_callback (void)
{
}


int main()
{
    float Current_temp=0.0;

    // Use internal pullups for the three pushbuttons
    pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
    // Delay for initial pullup to take effect
    wait(.01);
    // Setup Interrupt callback functions for a pb hit
    pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);
    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();
    // Pushbuttons now setup and running

    Current_temp = myTMP36; //Read temp sensor
    printf("Hello PC World\n\r"); // need terminal application running on PC to see this output
    uLCD.printf("\n\rHello LCD World\n\r"); // LCD
    mySpeaker.PlayNote(500.0, 1.0, 1.0); // Speaker buzz
    myShiftbrite.write( 0, 50 ,0); // Green RGB LED
    // SD card write file example - prints error message on PC when running until SD card hooked up
    // Delete to avoid run time error
    mkdir("/sd/mydir", 0777); // set up directory and permissions
    FILE *fp = fopen("/sd/mydir/sdtest.txt", "w"); //open SD
    if(fp == NULL) {
        error("Could not open file for write\n");
    }
    fprintf(fp, "Hello SD Card World!"); // write SD
    fclose(fp); // close SD card

    // State machine for thermostat
    while (1) {
        {
            enum Statetype { Heat_off = 0, Heat_on };
            Statetype state = Heat_off;
            while(1) {
                switch (state) {
                    case Heat_off:
                        myLED4 = 0;
                        state = Heat_on;
                        break;
                    case Heat_on:
                        myLED4 = 1;
                        state = Heat_off;
                        break;
                }
                wait(0.33);
                // Blinks as long as code is running and not locked up
                myLED1=!myLED1;
            }
        }
    }
}