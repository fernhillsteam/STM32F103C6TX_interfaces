
/* Program 7_2.c: On-chip temperature sensor
 * This program converts the on-chip temperature sensor output
 * and display the temperature in Celcius on the console.
 * Timer TIM2 is configured to generate 1 Hz output, which is
 * used to trigger the A/D conversion. The internal temperature
 * sensor is connected to channel 18 of ADC1 Master. The sensor
 * is turned on at bit 23 of ADC_CCR register.
 *
 * The conversion result is displayed as Celcius using the
 * formula and parameters in the datasheet on the console.
 * The console UART driver is from Program 4-5.
 *
 * Created on: 11/8/2021
 * Author: SUNIL PAWAR
 */

#include "stm32f4xx.h"
#include <stdio.h>

void USART2_init(void);
int USART2_write(int c);

int main(void)
{
    int data;
    double volt, temp;

    RCC->AHB1ENR |=  1;	            /* enable GPIOA clock */

    /* setup TIM2 */
    RCC->APB1ENR |= 1;              /* enable TIM2 clock */
    TIM2->PSC = 1600 - 1;           /* divided by 1600 */
    TIM2->ARR = 10000 - 1;          /* divided by 10000, sample at 1 Hz */
    TIM2->CNT = 0;
    TIM2->CCMR1 = 0x00006800;       /* pwm1 mode,  preload enable */
    TIM2->CCER = 0x0010;            /* ch2 enable */
    TIM2->CCR2 = 50 - 1;
    TIM2->CR1 = 1;                  /* enable timer2 */

    /* setup ADC1 */
    RCC->APB2ENR |= 0x00000100;     /* enable ADC1 clock */
    /* turn on the temp sensor */
    ADC->CCR |= 0x800000;
    ADC->CCR &= ~0x400000;          /* VBATE must be disabled for temp sensor */
    /* configure A to D converter */
    ADC1->SMPR1 = 0x4000000;        /* sampling time minimum 10 us */
    ADC1->SQR3 = 18;                /* ch18 - internal temp sensor, single channel */
    ADC1->CR2 = 0x13000000;         /* trigger: EXTEN rising edge, EXTSEL 3 = tim2.2 */
    ADC1->CR2 |= 1;                 /* enable ADC1 */

    /* initialize USART2 for output */
    USART2_init();
    printf("ADC internal temperature sensor \r\n");

    while(1)
    {
        while(!(ADC1->SR & 2)) {}
        data = ADC1->DR;
        /* Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25 */
        /* V25 = 0.76V, slope = 2.5 mV/C */
        volt = (double)data / 4095 * 3.3;   /* convert ADC output to voltage */
        temp = (volt - 0.76) / 0.0025 + 25; /* convert voltage to temperature C */
//        printf("%d, %.2f\370C\r\n", data, temp);    /* octal 370 is degree sign in extended ASCII */
    }
}

/* initialize USART2 to transmit at 9600 Baud */
void USART2_init (void)
{
    RCC->AHB1ENR |= 1;          /* Enable GPIOA clock */
    RCC->APB1ENR |= 0x20000;    /* Enable USART2 clock */

    /* Configure PA2 for USART2_TX */
//    GPIOA->AFR[0] &= ~0x0F00;
//    GPIOA->AFR[0] |=  0x0700;   /* alt7 for USART2 */
//    GPIOA->MODER  &= ~0x0030;
//    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    /* Configure PA2 for USART2_TX */
    GPIOA->AFR[0] |= 0x0700;
    GPIOA->AFR[0] |= 0x7000;   /* alt7 for USART2 */
    GPIOA->MODER  |=  0x0080;
    GPIOA->MODER  |=  0x0020;   /* enable alternate function for PA2 */

    USART2->BRR = 0x0683;       /* 9600 baud @ 16 MHz */
    USART2->CR1 = 0x000C;       /* enable Tx, Rx, 8-bit data */
    USART2->CR2 = 0x0000;       /* 1 stop bit */
    USART2->CR3 = 0x0000;       /* no flow control */
    USART2->CR1 |= 0x2000;      /* enable USART2 */
}

/* Write a character to USART2 */
int USART2_write (int ch)
{
    while (!(USART2->SR & 0x0080)) {}   /* wait until Tx buffer empty */
    USART2->DR = ch;
    return ch;
}

/* The code below is the interface to the C standard I/O library.
 * All the I/O are directed to the console, which is UART2.
 */
struct __FILE { int handle; };
FILE __stdout = {1};

/* Called by C library console/file output */
int fputc(int c, FILE *f)
{
    return USART2_write(c);  /* write the character to console */
}
