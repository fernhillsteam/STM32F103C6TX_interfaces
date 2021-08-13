// * main.c UART on USART2 Receive at 115200 Baud
// * By default, the clock is running at 16 MHz.
// * The UART2 is configured for 115200 Baud.
// * PA2 - USART2 RX (AF7)
// * PA3 - USART2 TX (AF7)
// *  Created on: 11/8/2021
// *  Author: SUNIL PAWAR

#include "stm32F4xx.h"

void USART2_init(void);
void USART2_write(int c);
char USART2_read(void);
void delayMs(int);

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void)
{
	char c;
    USART2_init();          /* initialize USART2 */
    while(1)
    {
        c = USART2_read();          /* wait for a character received */
        USART2_write(c);
    }
}

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void USART2_init (void)
{
    RCC->AHB1ENR |= 1;          /* Enable GPIOA clock */
    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

    /* Configure PA2 for USART2_TX */

    GPIOA->AFR[0] |= 0x0700;
    GPIOA->AFR[0] |= 0x7000;   /* alt7 for USART2 */
    GPIOA->MODER  |=  0x0020;
    GPIOA->MODER  |=  0x0080;   /* enable alternate function for PA2 */

    USART2->CR1 = 0x00;
    USART2->CR1 |= 0x2000;      // UE=1... EN USART
    USART2->BRR = 0x008B;       /* 115200 baud @ 16 MHz */
    USART2->CR1 = 0x0008;       /* enable TE, 8-bit data */
    USART2->CR1 = 0x0004;       /* enable RE, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
}

/* Write a character to USART2 */
void USART2_write (int ch)
{
    while (!(USART2->SR & 0x0080)) {}   // wait until Tx buffer empty
    USART2->DR = (ch & 0xFF);
}
/* Read a character from USART2 */
char USART2_read(void)
{
    while (!(USART2->SR & 0x0020)) {}   // wait until char arrives
    return USART2->DR;
}

void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 2000; i++) ;
}
