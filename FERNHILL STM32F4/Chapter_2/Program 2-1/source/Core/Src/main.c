// * main.c Toggle Green LED (LD2) on STM32F401RE Nucleo64 board at 1 Hz
// * This program toggles LD2 for 0.5 second ON and 0.5 second OFF
// * The green LED (LD2) is connected to PA5.
// *
// *  Created on: Jul 27, 2021
// *  Author: SUNIL PAWAR



#include "stm32f4xx.h"

void delayMs(int n);

int main(void)
{
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */

    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */

    while(1) {
        GPIOA->ODR |=  0x00000020;  /* turn on LED */
        delayMs(500);
        GPIOA->ODR &= ~0x00000020;  /* turn off LED */
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
