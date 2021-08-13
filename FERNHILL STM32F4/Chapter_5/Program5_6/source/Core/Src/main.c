/* p5_6.c Toggle Green LED using TIM2 Compare mode
 *
 * This program configures TIM2 with prescaler divides by 1600
 * and counter wraps around at 10000. So the 16 MHz system clock
 * is divided by 1600 then divided by 10000 to become 1 Hz.
 * The channel 1 is configured for compare mode to toggle the output
 * pin every time the timer counter matches the CCR1 register.
 * The output pin of TIM2 CH1 is PA5 and the alternate function
 * of PA5 should be set to AF1.
 *
 * Created on: 12/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"

int main(void)
{
    // configure PA5 as output to drive the LED
    RCC->AHB1ENR |=  1;             /* enable GPIOA clock */
    GPIOA->MODER &= ~0x00000C00;    /* clear pin mode */
    GPIOA->MODER |=  0x00000800;    /* set pin to alternate function */
    GPIOA->AFR[0] &= 0x00F00000;    /* clear pin AF bits */
    GPIOA->AFR[0] |= 0x00100000;    /* set pin to AF1 for TIM2 CH1 */

    // configure TIM2 to wrap around at 1 Hz
	// and toggle CH1 output when the counter value is 0
    RCC->APB1ENR |= 1;              /* enable TIM2 clock */
    TIM2->PSC = 1600 - 1;           /* divided by 1600 */
    TIM2->ARR = 10000 - 1;          /* divided by 10000 */
    TIM2->CCMR1 = 0x30;             /* set output to toggle on match */
    TIM2->CCR1 = 0;                 /* set match value */
    TIM2->CCER |= 1;                /* enable CH1 compare mode */
    TIM2->CNT = 0;                  /* clear counter */
    TIM2->CR1 = 1;                  /* enable TIM2 */

    while (1)
    {
    }
}
