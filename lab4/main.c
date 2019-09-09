#include "msp.h"
#include "ee1910delay.h"
#include "ee1910millis.h"
#include <stdio.h>


/**
 * main.c
 */

int lightCount = -1; //Doesn't exist
int lbPressed = 0;
int rbPressed;
void main(void) {
    //set up P2 & P6 Pins (Lights 1 - 9)
    P2 -> DIR |= 0xff;
    P2 -> OUT &= ~(0xf8);
    P5 -> DIR |= 0x37;
    P5 -> OUT &= ~(0x37);
    P6 -> DIR |= 0x01;
    P6 -> OUT &= ~(0x01);

    //set up P1 Pins (Left and Right Buttons)
    P1 -> DIR &= ~(0x02 | 0x10); // LB = 0x02; RB = 0x10
    P1 -> REN |= (0x02 | 0x10);
    P1 -> OUT |= (0x02 | 0x10); // Pressed is 1; Unpressed is 0


    while (1){


        if((P1 -> IN & 0x02) == 0){
            lbPressed = 1; //Set Left Button boolean
            start_timer(); //Start timer when left button is pressed


        }
        if((P1 -> IN & 0x10) == 0 && lbPressed == 1){
            delay(100);
            lightCount++;
            switch(lightCount){
            case 0:
                P2 -> OUT |= 0x08;
                break;
            case 1:
                P2 -> OUT |= 0x18;
                break;
            case 2:
                P2 -> OUT |= 0x38;
                break;
            case 3:
                P2 -> OUT |= 0x78;
                break;
            case 4:
                P2 -> OUT |= 0xF8;
                break;
            case 5:
                P5 -> OUT |= 0x01;
                break;
            case 6:
                P5 -> OUT |= 0x03;
                break;
            case 7:
                P5 -> OUT |= 0x07;
                break;
            case 8:
                P5 -> OUT |= 0x17;
                break;
            case 9:
                P5 -> OUT |= 0x37;
                break;
            case 10:
                //Turn off all LED Bar Lights
                P2 -> OUT &= ~(0xF8);
                P5 -> OUT &= ~(0x37);
                break;
            }
            if(lightCount > 10){
                lightCount = -1;
            }
            lbPressed = 0; // reset Left Button boolean




        if(millis() < 300){
            printf("Sprint\n");
        }
        else if (millis() > 300 && millis() < 599){
            printf("Run\n");
        }
        else if (millis() > 600 && millis() < 899){
            printf("Jog\n");
        }
        else if (millis() > 900 && millis() < 1199){
            printf("Walk\n");
        }
        else{
            printf("Nothing\n");
        }


        reset_timer();
        }

    }
}













