/*  Program 6_5.c Timer TIM2 interrupt
 *
 * Timer TIM2 is configured as an up-counter. By default, the system clock is
 * running at 16 MHz. The prescaler is set to divide by 16,000 that gives a
 * 1 kHz clock to the counter. The counter auto-reload is set to 999. When
 * the counter counts to 999, it updates the counter to zero and sets the
 * update interrupt flag (UIF). The UIE bit of TIM2->DIER is set so that
 * the UIF triggers a timer interrupt. The interrupt frequency is 1 Hz.
 * In the timer interrupt handler, the green LED (PA5) is toggled and
 * the UIF is cleared.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */
#include "stm32f4xx.h"

void delayMs(int n);

int main(void)
{
    __disable_irq();                /* global disable IRQs */
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */

    GPIOA->MODER &= ~0x00000C00;
    GPIOA->MODER |=  0x00000400;

    /* setup TIM2 */
    RCC->APB1ENR |= 1;              /* enable TIM2 clock */
    TIM2->PSC = 16000 - 1;          /* divided by 16000 */
    TIM2->ARR = 1000 - 1;           /* divided by 1000 */
    TIM2->CR1 = 1;                  /* enable counter */

    TIM2->DIER |= 1;                /* enable UIE */
    NVIC_EnableIRQ(TIM2_IRQn);      /* enable interrupt in NVIC */

    __enable_irq();                 /* global enable IRQs */

    while(1) {
    }
}

void TIM2_IRQHandler(void)
{
    TIM2->SR = 0;                   /* clear UIF */
    GPIOA->ODR ^= 0x20;				/* toggle LED */
}
