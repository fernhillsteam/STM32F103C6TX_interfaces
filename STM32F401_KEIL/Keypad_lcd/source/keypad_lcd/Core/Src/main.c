/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd_character.h"
#include "stdio.h"
#include "math.h"
#include "stdbool.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define R1_PORT GPIOA
#define	R1_PIN GPIO_PIN_11

#define R2_PORT GPIOA
#define	R2_PIN GPIO_PIN_10

#define R3_PORT GPIOA
#define	R3_PIN GPIO_PIN_9

#define R4_PORT GPIOA
#define	R4_PIN GPIO_PIN_8

#define C1_PORT GPIOC
#define	C1_PIN GPIO_PIN_15

#define C2_PORT GPIOC
#define	C2_PIN GPIO_PIN_14

#define C3_PORT GPIOC
#define	C3_PIN GPIO_PIN_13

#define C4_PORT GPIOC
#define	C4_PIN GPIO_PIN_12

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x % 10) + '0';

        x = x / 10;
    }
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
    reverse(str, i);
    str[i] = '\0';
    return i;
}
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);//0
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007

        fpart = fpart * pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char buff[16];
char read_keypad(void)
{
		HAL_GPIO_WritePin(R1_PORT,R1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R2_PORT,R2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_PORT,R3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_PORT,R4_PIN, GPIO_PIN_SET);

		if(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
			return '7';
		}
		
		if(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
			return '8';
		}		
		
		if(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
			return '9';
		}		
		
		if(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
			return '/';
		}		
		
		HAL_GPIO_WritePin(R1_PORT,R1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_PORT,R2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R3_PORT,R3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_PORT,R4_PIN, GPIO_PIN_SET);
		
		if(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
			return '4';
		}
		
		if(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
			return '5';
		}		
		
		if(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
			return '6';
		}		
		
		if(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
			return 'x';
		}			

		HAL_GPIO_WritePin(R1_PORT,R1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_PORT,R2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_PORT,R3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R4_PORT,R4_PIN, GPIO_PIN_SET);
		
		if(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
			return '1';
		}
		
		if(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
			return '2';
		}		
		
		if(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
			return '3';
		}		
		
		if(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
			return '-';
		}		
		HAL_GPIO_WritePin(R1_PORT,R1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_PORT,R2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_PORT,R3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_PORT,R4_PIN, GPIO_PIN_RESET);
		
		if(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C1_PORT,C1_PIN)==GPIO_PIN_RESET)
			return '*';
		}
		
		if(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C2_PORT,C2_PIN)==GPIO_PIN_RESET)
			return '0';
		}		
		
		if(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C3_PORT,C3_PIN)==GPIO_PIN_RESET)
			return '=';
		}		

		if(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
		{
			while(HAL_GPIO_ReadPin(C4_PORT,C4_PIN)==GPIO_PIN_RESET)
			return '+';
		}		
		
}

uint8_t key;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
   lcd_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		key = read_keypad();
		lcd_gotoxy(0,0);
		lcd_puts("Enter the key");
		
		if(key!=0x01)
		{
				sprintf(buff,"key = %c",key);
				lcd_gotoxy(0,1);
				lcd_puts(buff);
		}
		HAL_Delay(3);
  }
  
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
