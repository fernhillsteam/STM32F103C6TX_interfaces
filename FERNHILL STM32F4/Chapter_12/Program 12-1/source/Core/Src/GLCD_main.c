
/* P12_1.c: PCD8544 (Nokia5110) GLCD via SPI with STM32F446
 *
 * Letters A,B,C are written on the display.
 *
 * The connections
 * GLCD    Arduino          STM32F446
 * 1 RST   D6   reset       PB10
 * 2 CE    D7   chip select PA8
 * 3 DC    D10  reg select  PB6
 * 4 DIN   D11  SPI MOSI    PA7
 * 5 CLK   D13  SPI SCK     PA5
 * 6 VCC
 * 7 LIGHT
 * 8 GND
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"

/* define the pixel size of display */
#define GLCD_WIDTH  84
#define GLCD_HEIGHT 48

void GLCD_setCursor(unsigned char x, unsigned char y);
void GLCD_clear(void);
void GLCD_init(void);
void GLCD_data_write(unsigned char data);
void GLCD_command_write(unsigned char data);
void GLCD_putchar(int c);
void SPI_init(void);
void SPI_write(unsigned char data);

/* sample font table */
const char font_table[][6] = {
    {0x7e, 0x11, 0x11, 0x11, 0x7e, 0},  /* A */
    {0x7f, 0x49, 0x49, 0x49, 0x36, 0},  /* B */
    {0x3e, 0x41, 0x41, 0x41, 0x22, 0},  /* C */
    {0x7f, 0x41, 0x41, 0x22, 0x1c, 0}   /* D */
};

int main(void)
{
    GLCD_init();        /* initialize the GLCD controller */
    GLCD_clear();       /* clear display and home the cursor */

    GLCD_putchar(0);    /* display letter A */
    GLCD_putchar(1);    /* display letter B */
    GLCD_putchar(2);    /* display letter C */
    GLCD_putchar(3);    /* display letter C */

    while(1) {
    }
}

void GLCD_putchar(int c)
{
    int i;
    for (i = 0; i < 6; i++)
        GLCD_data_write(font_table[c][i]);
}

void GLCD_setCursor(unsigned char x, unsigned char y)
{
    GLCD_command_write(0x80 | x);  /* column */
    GLCD_command_write(0x40 | y);  /* bank (8 pixel rows per bank) */
}

/* clears the GLCD by writing zeros to the entire screen */
void GLCD_clear(void)
{
    int32_t index;
    for (index = 0 ; index < (GLCD_WIDTH * GLCD_HEIGHT / 8) ; index++)
        GLCD_data_write(0x00);

    GLCD_setCursor(0, 0); /* return to the home position */
}

/* send the initialization commands to PCD8544 GLCD controller */
void GLCD_init(void)
{
    SPI_init();

    /* PORTB 6, 10 as GPIO output for GLCD DC and RESET */
    RCC->AHB1ENR |= 2;              /* enable GPIOB clock */
    GPIOB->MODER &= ~0x00303000;    /* clear pin mode */
    GPIOB->MODER |=  0x00101000;    /* set pin output mode */

    /* hardware reset of GLCD controller */
    GPIOB->BSRR = 0x04000000;       /* assert RESET */
    GPIOB->BSRR = 0x00000400;       /* deassert RESET */

    GLCD_command_write(0x21);       /* set extended command mode */
    GLCD_command_write(0xB8);       /* set LCD Vop for contrast */
    GLCD_command_write(0x04);       /* set temp coefficient */
    GLCD_command_write(0x14);       /* set LCD bias mode 1:48 */
    GLCD_command_write(0x20);       /* set normal command mode */
    GLCD_command_write(0x0C);       /* set display normal mode */
}

/* write to GLCD controller data register */
void GLCD_data_write(unsigned char data)
{
    GPIOB->BSRR = 0x00000040;       /* select data register */
    SPI_write(data);                /* send data via SPI */
}

/* write to GLCD controller command register */
void GLCD_command_write(unsigned char data)
{
    GPIOB->BSRR = 0x00400000;       /* select command register */
    SPI_write(data);                /* send data via SPI */
}

void SPI_init(void)
{
    RCC->AHB1ENR |= 1;              /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;         /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PORTA 8 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x00030000;    /* clear pin mode */
    GPIOA->MODER |=  0x00010000;    /* set pin output mode */

    SPI1->CR1 = 0x31C;
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */
}

void SPI_write(unsigned char data)
{
    GPIOA->BSRR = 0x01000000;       /* assert slave select */
    while (!(SPI1->SR & 2)) {}      /* wait until transfer buffer Empty */
    SPI1->DR = data;                /* write command and upper 4 bits of data */
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00000100;       /* deassert slave select */
}
