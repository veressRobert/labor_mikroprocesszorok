/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "OLED.h"
#include <stdlib.h>



typedef enum
{
    OLED_STATE_LABOR = 0x00,        // cimke
    OLED_STATE_SMILEY = 0x01       // smiley
} OLED_Test_state;
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
OLED_Test_state test_state = OLED_STATE_LABOR;

GPIO_PinState previous_button_state[3] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};
GPIO_PinState current_button_state[3] = {GPIO_PIN_SET, GPIO_PIN_SET, GPIO_PIN_SET};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void OLED_TestLabor(void)
{
    OLED_SetCursor(25, 12);
    OLED_WriteString("Labor 4", Font_11x18, WHITE);

    OLED_SetCursor(18, 38);
    OLED_WriteString("Veress Robert", Font_7x10, WHITE);

    return;
}

void OLED_TestSmiley(void)
{
    // Fej
    OLED_DrawCircle(63, 31, 28, WHITE);

    // Bal szem
    OLED_DrawCircle(50, 22, 4, WHITE);

    // Jobb szem
    OLED_DrawCircle(76, 22, 4, WHITE);

    // Mosoly
    OLED_DrawArc(63, 30, 18, 300, 360, WHITE);
    OLED_DrawArc(63, 30, 18, 0, 60, WHITE);

    return;
}

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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	// Button handling - read the button states
	current_button_state[0] = HAL_GPIO_ReadPin(PE2_IN_GPIO_Port, PE2_IN_Pin);
	current_button_state[1] = HAL_GPIO_ReadPin(PE3_IN_GPIO_Port, PE3_IN_Pin);
	current_button_state[2] = HAL_GPIO_ReadPin(PE6_IN_GPIO_Port, PE6_IN_Pin);
	// Bal
	if ((current_button_state[0] != previous_button_state[0]) && (GPIO_PIN_RESET == current_button_state[0]))
	{
		if (OLED_STATE_LABOR == test_state)
		{
			test_state = OLED_STATE_SMILEY;
		}
		else
		{
			test_state = OLED_STATE_LABOR;
		}

		OLED_Fill(BLACK);
		OLED_UpdateScreen();
	}

	// Jobb
	if ((current_button_state[2] != previous_button_state[2]) && (GPIO_PIN_RESET == current_button_state[2]))
	{
		if (OLED_STATE_SMILEY == test_state)
		{
			test_state = OLED_STATE_LABOR;
		}
		else
		{
			test_state = OLED_STATE_SMILEY;
		}

		OLED_Fill(BLACK);
		OLED_UpdateScreen();
	}

	// Invert
	if ((current_button_state[1] != previous_button_state[1]) && (GPIO_PIN_RESET == current_button_state[1]))
	{
		if (OFF == OLED_GetDisplayInverse())
		{
			OLED_SetDisplayInverse(ON);
		}
		else
		{
			OLED_SetDisplayInverse(OFF);
		}
	}

	previous_button_state[0] = current_button_state[0];
	previous_button_state[1] = current_button_state[1];
	previous_button_state[2] = current_button_state[2];

	switch (test_state)
	{
	case OLED_STATE_SMILEY:
		OLED_TestSmiley();
		break;
	case OLED_STATE_LABOR:
	default:
		OLED_TestLabor();
		break;
	}

	OLED_UpdateScreen();
	HAL_Delay(50);
    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pins : PE2_IN_Pin PE3_IN_Pin PE6_IN_Pin */
  GPIO_InitStruct.Pin = PE2_IN_Pin|PE3_IN_Pin|PE6_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
