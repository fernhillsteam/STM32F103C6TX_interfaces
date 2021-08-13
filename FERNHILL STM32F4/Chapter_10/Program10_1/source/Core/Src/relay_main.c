/* Program 10_1.c Relay control
 *
 * This program turns the relay connected to PA5 on and off every second.
 *
 * Created on: 13-08-2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"

void delayMs(int n);

int main(void)
{
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */
    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */

    while(1)
    {
        GPIOA->BSRR = 0x00000020;   /* turn on output */
        delayMs(500);
        GPIOA->BSRR = 0x00200000;   /* turn off output */
        delayMs(500);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
