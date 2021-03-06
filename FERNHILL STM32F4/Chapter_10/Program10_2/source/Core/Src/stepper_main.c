//* Program 10_2.c Stepper motor control
// *
// * This program controls a unipolar stepper motor using PTA7, 6, 5, 4.
// *
// * This program was tested with Keil uVision v5.23 with DFP v2.11.0.
// *
// *  Created on: 13-08-2021
// *  Author: SUNIL PAWAR

#include "stm32f4xx.h"

void delayMs(int n);
int main(void)
{
    const char steps[ ] = {0x90, 0x30, 0x60, 0xC0};
    int i;

    /* PTA7, 6, 5, 4 for motor control */
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */
    GPIOA->MODER &= ~0x0000FF00;    /* clear pin mode */
    GPIOA->MODER |=  0x00005500;    /* set pins to output mode */

    for (;;) {
        GPIOA->ODR = steps[i++ & 3];   /* set the output pattern */
        delayMs(100);
    }
}
/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
