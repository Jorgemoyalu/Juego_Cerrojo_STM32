/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "inputs_Jorge.h"
#include "audio_Jorge.h"
#include "display_Tudor.h"
#include "ranking_Tudor.h"
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

/* USER CODE BEGIN PV */
EntradasUsuario misPotenciometros;
volatile int tiempo_restante = 60;
//volatile int contador_tim4 = 0; //Variable de prueba TIM4
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void MX_I2C1_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  Inputs_Init();
  Display_Init();
  Ranking_Init();
  Audio_Init(&htim3);

  //Pantalla de carga al comienzo
  Display_LCD_Limpiar();
  Display_LCD_Escribir(0, 0, "Cargando...");
    for(int i = 0; i <= 100; i += 2) {
        Display_BarraProgreso(1, i);
        HAL_Delay(20);
    }

    HAL_Delay(500);
    Display_LCD_Limpiar();
//Bienvenida: Parpadeo de luces
    for(int k=0; k<3; k++) {
          Actualizar_Semaforo(LED_VICTORIA);
          HAL_Delay(150); // Encendido rápido
          Actualizar_Semaforo(LED_APAGADO);
          HAL_Delay(150); // Apagado rápido
      }

  HAL_TIM_Base_Start_IT(&htim2); // Timer del Juego (1 Hz / 1 seg)
  HAL_TIM_Base_Start_IT(&htim4); // Timer de Botones (100 Hz / 10 ms)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Limpiamos la pantalla una vez antes de entrar
    Display_LCD_Limpiar();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	Leer_Potenciometros(&hadc1, &misPotenciometros);
	//PINTAR EN LCD
	    char buffer[32];
	    char bufferTiempo[24];
	    // Pintamos los 4 valores: Ej "Codigo: 1 5 9 0"
	    sprintf(buffer, "Code: %d %d %d %d",
	            misPotenciometros.digito[0],
	            misPotenciometros.digito[1],
	            misPotenciometros.digito[2],
	            misPotenciometros.digito[3]);
	    //Prueba TIM4
	    //sprintf(buffer, "TEST TIM4: %d", contador_tim4);
	    //Display_LCD_Escribir(1, 0, buffer);

	    // Lógica del Tiempo para la Fila 0
	        if (tiempo_restante > 0) {
	            sprintf(bufferTiempo, "TIEMPO: %02d s    ", tiempo_restante);
	        } else {
	            sprintf(bufferTiempo, "!! TIME OUT !!  ");
	        }
	        Display_LCD_Escribir(0, 0, bufferTiempo);
	    Display_LCD_Escribir(1, 0, buffer);         // Fila 1
	    //Lógica simple para probar el semáforo:
	    //Miramos el valor del Primer Potenciómetro (Ruleta 1)
	    if (misPotenciometros.digito[0] < 3) {
	        // Si está bajo (0-2) -> Rojo (FRÍO)
	        Actualizar_Semaforo(LED_FRIO_ROJO);
	    }
	    else if (misPotenciometros.digito[0] < 7) {
	        // Si está medio (3-6) -> Amarillo (TEMPLADO)
	        Actualizar_Semaforo(LED_TEMPLADO_AMARILLO);
	    }
	    else {
	        // Si está alto (7-9) -> Verde (CALIENTE)
	        Actualizar_Semaforo(LED_CALIENTE_VERDE);
	    }
	    HAL_Delay(50);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // CASO 1: Timer del JUEGO (1 segundo)
    if (htim->Instance == TIM2) {
        if (tiempo_restante > 0) {
            tiempo_restante--;
        }
    }

    // CASO 2: Timer de BOTONES (10 milisegundos)
    if (htim->Instance == TIM4) {
    	//contador_tim4++; //Para prueba de contador tim 4
        // Inputs_Leer_Botones();
    }
}


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
