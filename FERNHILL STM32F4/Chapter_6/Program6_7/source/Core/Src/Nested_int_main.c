
/* Program 6_7.c Demonstrate Nested Interrupts
 *
 * TIM3 is setup to interrupt at about 1 Hz. In the interrupt handler,
 * PB3 is turned on and a delay function of 500 ms is called.
 * PB3 is turned off at the end of the delay.
 *
 * TIM4 is setup to interrupt at about 10 Hz. In the interrupt handler,
 * PB4 and the LED0 is turned on and a delay function of 25 ms is called.
 * PB4 and the LED0 is turned off at the end of the delay.
 *
 * When TIM3 has the higher priority (the way this code is), the TIM4
 * interrupts are blocked by TIM3 interrupt handler and the LED stops
 * blinking intermittently. You can see their interactions better
 * with an oscilloscope on PB3 and PB4.
 *
 * When TIM4 has the higher priority (you need to switch the priority of
 * the two timers at the #define statements), the TIM3 interrupt handler is
 * preempted by TIM4 interrupts and the LED is blinking all the time.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"

void delayMs(int n);

/* priority of TIM3 and TTIM4 should be between 0 and 15 */
#define PRIO_TIM3 4
#define PRIO_TIM4 3

int main (void)
{
    __disable_irq();                /* global disable IRQs */
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */
    RCC->AHB1ENR |=  2;	            /* enable GPIOB clock */

    GPIOA->MODER &= ~0x00000C00;    /* pin PA5 (LED) output */
    GPIOA->MODER |=  0x00000400;
    GPIOB->MODER &= ~0x000003C0;    /* pins PB3, PB4 output */
    GPIOB->MODER |=  0x00000140;

    /* setup TIM3 */
    RCC->APB1ENR |= 2;              /* enable TIM3 clock */
    TIM3->PSC = 1000 - 1;           /* divided by 1000 */
    TIM3->ARR = 15500 - 1;          /* divided by 15500 */
    TIM3->CR1 = 1;                  /* enable counter */

    TIM3->DIER |= 1;                /* enable UIE */
    NVIC_SetPriority(TIM3_IRQn, PRIO_TIM3);
    NVIC_EnableIRQ(TIM3_IRQn);      /* enable interrupt in NVIC */

    /* setup TIM4 */
    RCC->APB1ENR |= 4;              /* enable TIM4 clock */
    TIM4->PSC = 1000 - 1;           /* divided by 1000 */
    TIM4->ARR = 1562 - 1;           /* divided by 1562 */
    TIM4->CR1 = 1;                  /* enable counter */

    TIM4->DIER |= 1;                /* enable UIE */
    NVIC_SetPriority(TIM4_IRQn, PRIO_TIM4);
    NVIC_EnableIRQ(TIM4_IRQn);      /* enable interrupt in NVIC */

    __enable_irq();                 /* enable interrupt globally */

    while(1)
    {
    }
}

void TIM3_IRQHandler(void)
{
    GPIOB->BSRR = 0x00000008;       /* turn on PB3 */
    delayMs(500);
    GPIOB->BSRR = 0x00080000;       /* turn off PB3 */
    TIM3->SR = 0;                   /* clear UIF */
}

void TIM4_IRQHandler(void)
{
    GPIOA->BSRR = 0x00000020;       /* turn on LED */
    GPIOB->BSRR = 0x00000010;       /* turn on PB4 */
    delayMs(25);
    GPIOA->BSRR = 0x00200000;       /* turn off LED */
    GPIOB->BSRR = 0x00100000;       /* turn off PB4 */
    TIM4->SR = 0;                   /* clear UIF */
}

/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
