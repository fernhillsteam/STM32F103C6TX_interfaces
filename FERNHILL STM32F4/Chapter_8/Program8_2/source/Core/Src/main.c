
/* Program 8_2.c: Using SPI1 to generate a sawtooth waveform on LTC1661
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

void SPI1_init(void);
void DAC_write(short data);

int main(void)
{
    short i;
    SPI1_init();
    while(1)
    {
        for (i = 0; i < 1024; i++)
        {
            DAC_write(i);      /* write the letter through SPI1 */
        }
    }
}

/* enable SPI1 and associated GPIO pins */
void SPI1_init(void)
{
    RCC->AHB1ENR |= 1;          /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;     /* enable SPI1 clock */

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
   wait for transmit complete and deassert slave select. */
void DAC_write(short data)
{
    data &= 0x03FF;                 /* make sure data is 10-bit */
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    GPIOA->BSRR = 0x00100000;       /* deassert slave select */
    SPI1->DR = (1<<12) | (data >> 6);  /* write command and upper 4 bits of data */
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    SPI1->DR = (data << 2);
  //  SPI1->DR = (data << 2) & 0xFF;
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00000010;       /* assert slave select */
}
