
/* Program 6_4.c Toggle the LED using the SysTick interrupt
 *
 * This program sets up the SysTick to interrupt at 1 Hz.
 * The system clock is running at 16 MHz.
 * 1 sec/1 us = 16,000,000-1 for RELOAD register.
 * In the interrupt handler, the LED is toggled.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32F4xx.h"
void SysTick_Handler(void);
int main(void)
{
    __disable_irq();                /* global disable IRQs */

    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */

    /* Configure SysTick */
    SysTick->LOAD = 16000000-1;     /* reload with number of clocks per second */
    SysTick->VAL = 0;
    SysTick->CTRL = 7;              /* enable SysTick interrupt, use system clock */
    __enable_irq();

    while (1) {
    }
}

void SysTick_Handler(void)
{
    GPIOA->ODR ^= 0x00000020;   /* turn on LED */
}
