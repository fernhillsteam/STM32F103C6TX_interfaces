/* p5_5.c Toggle Green LED (LD2) on STM32F446RE Nucleo64 using TIM3
 *
 * This program configures TIM3 with prescaler divides by 1600
 * and counter wraps around at 10000. So the 16 MHz system clock
 * is divided by 1600 then divided by 10000 to become 1 Hz.
 * Every time the counter wraps around, it sets UIF flag
 * (bit 0 of TIM3_SR register). The program waits for the
 * UIF to set then toggles the LED (PA5).
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
    GPIOA->MODER |=  0x00000400;    /* set pin to output mode */

    // configure TIM3 to wrap around at 1 Hz
    RCC->APB1ENR |= 2;              /* enable TIM3 clock */
    TIM3->PSC = 1600 - 1;           /* divided by 1600 */
    TIM3->ARR = 10000 - 1;          /* divided by 10000 */
    TIM3->CNT = 0;                  /* clear timer counter */
    TIM3->CR1 = 1;                  /* enable TIM3 */

    while (1)
    {
        while(!(TIM3->SR & 1)) {}   /* wait until UIF set */
        TIM3->SR &= ~1;             /* clear UIF */
        GPIOA->ODR ^= 0x00000020;   /* toggle green LED */
    }
}
