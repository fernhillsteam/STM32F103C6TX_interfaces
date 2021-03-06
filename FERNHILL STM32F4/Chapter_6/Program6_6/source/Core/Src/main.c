
/* Program 6_6.c Timer TIM2 interrupt
 *
 * Timer TIM2 is configured as an up-counter. By default, the system clock is
 * running at 16 MHz. The prescaler is set to divide by 16,000 that gives a
 * 1 kHz clock to the counter.
 * The timer is set up for output compare interrupt for channel 1. When the
 * counter matches the CCR1 register, the output toggles and an interrupt
 * is triggered. In the interrupt handler, the PERIOR value is added CCR1
 * so that the next CCR1 match occurs after the PERIOR count.
 * When the PERIOD is set to 1000, the output toggles at 1 Hz.
 * The output of TIM2 Channel 1 is PA5 which is connected to the LED.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"

#define PERIOD 1000
void delayMs(int n);

int main(void)
{
    __disable_irq();                /* global disable IRQs */
    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */

    GPIOA->MODER &= ~0x00000C00;
    GPIOA->MODER |=  0x00000800;    /* use alternate function */
    GPIOA->AFR[0] = 0x00100000;     /* PA5 TIM2-CH1 */

    /* setup TIM2 */
    RCC->APB1ENR |= 1;              /* enable TIM2 clock */
    TIM2->PSC = 16000 - 1;          /* divided by 16000 */
    TIM2->ARR = 0xFFFF;             /* max count */
    TIM2->CCR1 = PERIOD;
    TIM2->CNT = 0;
    TIM2->CCMR1 = 0x30;             /* Output compare toggle */
    TIM2->CCER = 1;                 /* CC1 enable */
    TIM2->CR1 = 1;                  /* enable counter */

    TIM2->DIER |= 2;                /* enable CC1IE */
    NVIC_EnableIRQ(TIM2_IRQn);      /* enable interrupt in NVIC */

    __enable_irq();                 /* global enable IRQs */

    while(1) {
    }
}

void TIM2_IRQHandler(void)
{
    TIM2->SR = 0;                   /* clear UIF */
    TIM2->CCR1 = (TIM2->CCR1 + PERIOD) & 0xFFFF;    /* update CCR1 */
}
