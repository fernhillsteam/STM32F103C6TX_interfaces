
/* p5_3.c Toggle Green LED (LD2) on STM32F401RE using SysTick
 *
 * This program uses SysTick to generate multiples of millisecond delay.
 * System clock is running at 16 MHz. SysTick is configure
 * to count down from 16000 to zero to give a 1 ms delay.
 * A for loop counts how many millisecond the delay should be.
 * When 1000 is used for loop count, the delay is 1000 ms or 1 second.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */
#include "stm32f4xx.h"

void delayMs(int n);

int main(void)
{
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */

    while (1)
    {
        delayMs(1000);              /* delay 1000 ms */
        GPIOA->ODR ^= 0x00000020;   /* toggle red LED */
    }
}

void delayMs(int n)
{
    int i;

    /* Configure SysTick */
    SysTick->LOAD = 16000;  /* reload with number of clocks per millisecond */
    SysTick->VAL = 0;       /* clear current value register */
    SysTick->CTRL = 0x5;    /* Enable the timer */

    for(i = 0; i < n; i++)
    {
        while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNTFLAG is set */
            { }
    }
    SysTick->CTRL = 0;      /* Stop the timer (Enable = 0) */
}
