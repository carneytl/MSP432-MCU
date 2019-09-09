#include "msp.h"
#include "ee1910delay.h"
#include "ee1910analog.h"
#include "stdio.h"


/**
 * main.c
 */


int calibrateLight(){
    printf("Please place sensor to brightest available area"
                " then press the left button.\n");
    while((P1 -> IN & 0x02) != 0){
        //Wating for button input
    }
    delay(500);//Avoiding reading if button is held
    int calValueLight = analogRead();
    int max = calValueLight;
    printf("%d\n", max);
    return max;
}

int calibrateDark(){
    printf("Please place sensor to darkest available area"
                " then press the left button.\n");
        while((P1 -> IN & 0x02) != 0){
            //Wating for button input
        }
    delay(500);
    int calValueDark = analogRead();
    int min = calValueDark;
    printf("%d\n", min);
    return min;
}

int calibrate(){
    P1 -> DIR &= ~(0x02); // LB = 0x02; RB = 0x10
    P1 -> REN |= (0x02);
    P1 -> OUT |= (0x02);
    int calibrateCalc = (calibrateLight() + calibrateDark());
    int calTotal = calibrateCalc/10;
    return calTotal;
}

void sevenSeg(int i){
    P9 -> DIR |= 0xFF;
       P9 -> OUT &= ~(0xFF);
    switch(i){
    case 0:
                    P9 -> OUT |= 0xC0;
                    break;
                case 1:
                    P9 -> OUT |= 0xF9;
                    break;
                case 2:
                    P9 -> OUT |= 0xA4;
                    break;
                case 3:
                    P9 -> OUT |= 0xB0;
                    break;
                case 4:
                    P9 -> OUT |= 0x99;
                    break;
                case 5:
                    P9 -> OUT |= 0x92;
                    break;
                case 6:
                    P9 -> OUT |= 0x82;
                    break;
                case 7:
                    P9 -> OUT |= 0xF8;
                    break;
                case 8:
                    P9 -> OUT |= 0x80;
                    break;
                case 9:
                    P9 -> OUT |= 0x98;
                    break;


    }
    delay(100);
}

int photoSensor(int calTotal){
    int sensorRead = analogRead();
    int range = sensorRead/calTotal;
    return range;
}

void main(void){
    analogSetup();
   int calTotal = calibrate();
	while(1){
	    //Seven-Segment Test
	        sevenSeg(photoSensor(calTotal));


	}
}
