/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
	* Created: 06/08/2021
	* Author: sunil pawar
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
#include "NOKIA5110.h"
#include "FONT_IMAGE.h"


#define DELAY   2000

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
 
	N5110_Init();
 
  while (1)
  {
    N5110_Clear();
    N5110_SetFont(SmallFont);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*6), N5110_MAX_HEIGHT/2 - 4, "SUNILPAWAR", true);
    N5110_Update();
    HAL_Delay(DELAY);
		
    N5110_Clear();
    N5110_SetFont(TinyFont);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*4), N5110_MAX_HEIGHT/2 - 3, "NOKIA5110", true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    N5110_SetFont(MediumNumbers);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(2.5*12), N5110_MAX_HEIGHT/2 - 8, "99-99", true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    N5110_SetFont(BigNumbers);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(2.5*14), N5110_MAX_HEIGHT/2 - 12, "99-99", true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    N5110_SetFont(SmallFont);
    N5110_PrintStr(N5110_MAX_WIDTH/2-(4.5*6), N5110_MAX_HEIGHT/2 - 4, "NOKIA5110", true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_InvertDisplay(true);
    HAL_Delay(DELAY);
    N5110_InvertDisplay(false);
    HAL_Delay(DELAY);

    N5110_DrawHLine(0, N5110_MAX_HEIGHT/2 - 8, N5110_MAX_WIDTH, true);
    N5110_DrawHLine(0, N5110_MAX_HEIGHT/2 + 8, N5110_MAX_WIDTH, true);
    N5110_DrawVLine(0, N5110_MAX_HEIGHT/2 - 8, 16, true);
    N5110_DrawVLine(N5110_MAX_WIDTH-1, N5110_MAX_HEIGHT/2 - 8, 16, true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_DrawRect(0, N5110_MAX_HEIGHT/2 - 8, N5110_MAX_WIDTH-1, N5110_MAX_HEIGHT/2 + 8, false, false);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    N5110_Update();

    for(int i = 0; i < N5110_MAX_WIDTH; i=i+5)
    {
      N5110_DrawLine(0, 0, i, N5110_MAX_HEIGHT-1, true);
    }
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    for(int i = 0; i < N5110_MAX_WIDTH; i=i+5)
    {
      N5110_DrawLine(0, N5110_MAX_HEIGHT-1, i, 0, true);
    }
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    for(int i = 0; i < N5110_MAX_WIDTH; i=i+5)
    {
      N5110_DrawLine(N5110_MAX_WIDTH-1, 0, i, N5110_MAX_HEIGHT-1, true);
    }
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    for(int i = 0; i < N5110_MAX_WIDTH; i=i+5)
    {
      N5110_DrawLine(N5110_MAX_WIDTH-1, N5110_MAX_HEIGHT-1, i, 0, true);
    }
    N5110_Update();
    HAL_Delay(DELAY);


    N5110_Clear();
    for(int i = 0; i < N5110_MAX_HEIGHT/2; i=i+2)
    {
      N5110_DrawRect(N5110_MAX_WIDTH/2-1.7*i,N5110_MAX_HEIGHT/2-1-i,N5110_MAX_WIDTH/2+1.7*i,N5110_MAX_HEIGHT/2+i, true, false);
      N5110_Update();
    }
    HAL_Delay(DELAY);

    N5110_DrawRect(0, 0, N5110_MAX_WIDTH-1, N5110_MAX_HEIGHT-1, true, true);
    N5110_Update();
    HAL_Delay(DELAY);


    N5110_Clear();
    for(int i = 0; i < N5110_MAX_HEIGHT/2; i=i+2)
    {
      N5110_DrawCircle(N5110_MAX_WIDTH/2-1,N5110_MAX_HEIGHT/2-1, i, true, false);
      N5110_Update();
    }
    HAL_Delay(DELAY);


    N5110_DrawCircle(N5110_MAX_WIDTH/2-1,N5110_MAX_HEIGHT/2-1, N5110_MAX_HEIGHT/2-1, true, true);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_PrintStr(15, N5110_MAX_HEIGHT/2 - 4, "NOKIA5110", false);
    N5110_Update();
    HAL_Delay(DELAY);

    N5110_Clear();
    N5110_DrawBitmap(0, 0, (uint8_t*)Bitmap, N5110_MAX_WIDTH, N5110_MAX_HEIGHT);
    N5110_Update();
    HAL_Delay(DELAY);


  }
  /* USER CODE END 3 */
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

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
  HAL_GPIO_WritePin(GPIOA, RST_Pin|CE_Pin|DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : RST_Pin CE_Pin DC_Pin */
  GPIO_InitStruct.Pin = RST_Pin|CE_Pin|DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
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
