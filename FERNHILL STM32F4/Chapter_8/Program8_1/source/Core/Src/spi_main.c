/* Program 8_1.c: Using SPI1 to send A to Z characters
 *
 * SPI1 is configured as master with software slave select.
 * Clock rate is set to 1 MHz. Polarity/Phase are 0, 0
 *
 * PA7  MOSI
 * PA5  SCK
 * PA4  SS
 *
 * Created on: 14/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"
#include <stdio.h>
void USART2_init(void);
int USART2_write(int c);
void USART2_sendByte(unsigned char c);
void UART2_SendString (char *string);
void usart2_sendInt(unsigned int i);
void delayMs(int n);
void SPI1_init(void);
void SPI1_write(unsigned char data);
uint8_t SPI1_transfer(uint8_t d);

int main(void)
{
    char c;

    SPI1_init();
    USART2_init();
    UART2_SendString("welcome to SPI\r\n");

    while(1)
    {
        for (c = 'A'; c <= 'Z'; c++)
        {
            SPI1_write(c);      /* write the letter through SPI1 */
            delayMs(100);
        }
        UART2_SendString("\r\n");
    }
}

/* enable SPI1 and associated GPIO pins */
void SPI1_init(void)
{
    RCC->AHB1ENR |= 1;              /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;         /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PORTA4 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x00000300;    /* clear pin mode */
    GPIOA->MODER |=  0x00000100;    /* set pin output mode */

    SPI1->CR1 = 0x31C;		   /* set the Baud rate, 8-bit data frame */
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */
}

/* This function enables slave select, writes one byte to SPI1,
   wait for transmission complete and deassert slave select. */
void SPI1_write(unsigned char data)
{
    GPIOA->BSRR = 0x00100000;       /* assert slave select */
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    SPI1->DR = data;                /* write data */
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00000010;       /* deassert slave select */
    usart2_sendInt(data);
}
uint8_t SPI1_transfer(uint8_t d)
{
	SPI1->DR = d;	/* send the contents of d */
	while((SPI1->SR&(1<<0)) == 0);	/* wait until RXNE is set */
	return SPI1->DR;	/* return the received data */
}
/* initialize USART2 to transmit at 9600 Baud */
void USART2_init (void)
{
    RCC->AHB1ENR |= 1;          /* Enable GPIOA clock */
    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

    /* Configure PA2 for USART2_TX */
    GPIOA->AFR[0] |= 0x0700;
    GPIOA->AFR[0] |= 0x7000;   /* alt7 for USART2 */
    GPIOA->MODER  |=  0x0080;
    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    USART2->CR1 = 0x00;
    USART2->CR1 |= 0x2000;      // UE=1... EN USART
    USART2->BRR = 0x0683;       /* 9600 baud @ 16 MHz */
    USART2->CR1 = 0x000C;       /* enable Tx, Rx, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */

}
/* Write a character to USART2 */
int USART2_write (int ch)
{
    while (!(USART2->SR & 0x0080)) {}   // wait until Tx buffer empty
    USART2->DR = ch;
    return ch;
}
void USART2_sendByte(unsigned char c)
{

	while((USART2->SR & (1<<6)) == 0);  //wait until the TC flag is set
	//USART2->SR &= ~(1<<6);
	USART2->DR = c;
}
void UART2_SendString(char *string)
{
	while(*string) USART2_write(*string++);
}
void usart2_sendInt(unsigned int i)
{
	char str[10];
	sprintf(str,"%c ",i);
	UART2_SendString(str);
}

/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
