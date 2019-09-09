#include "msp.h"
#include "ee1910delay.h"
#include "ee1910analog.h"
#include "stdio.h"
#include "sinus.h"


/**
 * Lab 06:
 * Created By: Terry Carney
 * EE1910
 *
 * =========== Hardware Connections =================================
 *
 * PORT P1_1 = "Flatline" Action; P1_4 = Atrial Fibrillation Action.
 * PORT P2_3 = AnalogWrite() function.
 * PORT P9_0 - P9_7 = LED Bar 1 - 8; PORT P6_0 - P6_1 = LED Bar 9 -10.
 * PORT P4_0 - PORT P4_2 = Dip switches 1 - 3
 *
 * =========== Hardware Connections =================================
 */

//Creating connections for left and right buttons
void buttonSetup(){
    P1 -> DIR &= ~(0x02 | 0x10); // LB = 0x02; RB = 0x10
    P1 -> REN |= (0x02 | 0x10);
    P1 -> OUT |= (0x02 | 0x10);
}

//Creating connection to LED Bar
void ledBarSetup(){
    P6 -> DIR |= 0x03;
    P6 -> OUT &= ~(0x03);
    P9 -> DIR |= 0xFF;
    P9 -> OUT &= ~(0xFF);
}

//Creating connections to Dip switch
void dipSetup(){
    P4 -> DIR &= ~(0x07);
    P4 -> REN |= (0x07);
    P4 -> OUT |= (0x07);
}

//Function for Dip switches. Will return "int T".
//The Switch-Case describes the integer (Inital speed) of delay function that will
//be produced.
int dip(int T){
    //LOW = 1; HIGH = 0
    switch(P4 -> IN & 0b00000111){
    case 0b00000111:
        T = 0;
        break;
    case 0b00000110:
        T = 1;
        break;
    case 0b00000101:
        T = 2;
        break;
    case 0b00000100:
        T = 3;
        break;
    case 0b00000011:
        T = 4;
        break;
    case 0b00000010:
        T = 5;
        break;
    case 0b00000001:
        T = 6;
        break;
    case 0b00000000:
        T = 7;
        break;
    }
    return T;
}

//Function for LED Bar. Will return "int n".
//The Switch-Case describes the number of lights that will be turned on.
void ledBar(int n){
    switch(n){
    case 0:
        P9 -> OUT |= 0x01;
        break;
    case 1:
        P9 -> OUT |= 0x03;
        break;
    case 2:
        P9 -> OUT |= 0x07;
        break;
    case 3:
        P9 -> OUT |= 0x0F;
        break;
    case 4:
        P9 -> OUT |= 0x1F;
        break;
    case 5:
        P9 -> OUT |= 0x3F;
        break;
    case 6:
        P9 -> OUT |= 0x7F;
        break;
    case 7:
        P9 -> OUT |= 0xFF;
        break;
    case 8:
        P9 -> OUT |= 0xFF;
        P6 -> OUT |= 0x01;
        break;
    case 9:
        P9 -> OUT |= 0xFF;
        P6 -> OUT |= 0x03;
        break;
    }
}


void main(void)
{
    //All functions to activate specific ports.
    analogSetup();
    buttonSetup();
    ledBarSetup();
    dipSetup();

    int i = 0;

    //Used for dip() function
    int T = 0;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //runs program indefinitely
    while(1){

        //function used to write array to circuit.
        analogWrite(3,beat[i]);

        //Used to modify number of lights on according to index of array.
        int n = beat[i]/11;
        T = dip(T);
        ledBar(n);
        delay(T + 1);
        i++;

        //When the program reaches the end of the array, reset to beginning.
        if(i > 2000){
            i = 0;
        }

        //If Left button (P1_1) pressed, produced "flatline" waveform and LED action.
        while((P1 -> IN & 0x02) == 0){
            analogWrite(3, 0);
        }

        //If Right button (P1_4) pressed, produced Atrial Fibrillation waveform, and
        //LED action.
        while((P1 -> IN & 0x10) == 0){
            analogWrite(3, fib[i]);
            n = fib[i]/11;
            ledBar(n);
            delay(4);
            i++;
            if(i > 2000){
                i = 0;
            }
            //Turn off all LED Bar Lights while in "fib" array.
            P9 -> OUT &= ~(0xFF);
            P6 -> OUT &= ~(0x03);
        }
        //Turn off all LED Bar Lights while in "beat" array or flatline.
        P9 -> OUT &= ~(0xFF);
        P6 -> OUT &= ~(0x03);
    }
}
