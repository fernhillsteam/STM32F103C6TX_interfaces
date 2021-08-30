
/* Program 7_3.c: LM34 Fahrenheit temperature sensor
 *
 * LM34 is a Fahrenheit temperature sensor with the output of
 * 10 mV/degreeF. The program assumes an LM34 connected to
 * pin PC0 (channel 10). The conversion result is displayed one
 * the console as degree Fahrenheit.
 * The console UART driver is from Program 4-5.
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"
#include <stdio.h>

void ADC1_init();
void USART2_init(void);
int USART2_write(int c);
char USART2_read(void);
void UART2_SendString (char *string);
void usart2_sendInt(unsigned int i);
void delayMs(int n);

int main (void)
{
    int result;
    double temp;

    ADC1_init();
    USART2_init();

    UART2_SendString("LM34 Temperature Sensor\r\n");
    delayMs(1000);
    while (1)
    {
        ADC1->CR2 |= 0x40000000;        /* start a conversion */
        while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
        result = ADC1->DR;              /* read conversion result */
        temp = (double)result / 4095 * 330;
        UART2_SendString("ADC value is: ");
        usart2_sendInt(result);
        UART2_SendString("\r\n ");
        UART2_SendString("temp value : ");
        usart2_sendInt(temp);
        UART2_SendString("\r\n ");
        delayMs(1000);
    }
}

void ADC1_init()
{
    /* set up pin PC0 for analog input */
    RCC->AHB1ENR |=  4;	            /* enable GPIOC clock */
    GPIOC->MODER |=  3;             /* PC0 analog */

    /* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    ADC1->CR2 = 0;                  /* SW trigger */
    ADC1->SQR3 = 10;                /* single conversion ch 10 */
    ADC1->SQR1 = 0;                 /* conversion sequence length 1 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */

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
char USART2_read()
{
    while (!(USART2->SR & 0x0020)) {}   // wait until char arrives
    return USART2->DR;
}

void UART2_SendString(char *string)
{
	while(*string) USART2_write(*string++);
}

/* The function sends a number through USART1 */
void usart2_sendInt(unsigned int i)
{
	char str[10];
	sprintf(str,"%d",i);

	UART2_SendString(str);
}

/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}


