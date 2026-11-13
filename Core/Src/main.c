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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
  int16_t rocker_l;     //左水平
  int16_t rocker_l1;    //左竖直
  int16_t rocker_r;     //右水平
  int16_t rocker_r1;    //右竖直

  int16_t sw1;          //开关1
  int16_t sw2;          //2
  int16_t sw3;          //3
  int16_t sw4;          //4

  int16_t vr1;          //旋钮1
  int16_t vr2;          //2

  int16_t b_lose;       //帧丢失标志位
  int16_t er_protect;   //故障保护激活位
}rc_data;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
static rc_data ch_data; //通道值。static让变量只能在当前文件调用。
uint8_t raw_data[256];  //原始数据
int16_t ch[16];         //解析值减去1024
uint8_t start_byte;
uint8_t flat_byte;      //标志位
uint8_t stop_byte;      //停止位

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
  if(huart==&huart3){
    start_byte=raw_data[0];

    ch[0]=( ( (int16_t)raw_data[0]>>0 | (int16_t)raw_data[1]<<8 ) & 0x07FF ) -1024;
    ch[1]=( ( (int16_t)raw_data[1]>>3 | (int16_t)raw_data[2]<<5 ) & 0x07FF ) -1024;
    ch[2]=( ( (int16_t)raw_data[2]>>6 | (int16_t)raw_data[3]<<2 | (int16_t)raw_data[4]<<10 ) & 0x07FF ) -1024;
    ch[3]=( ( (int16_t)raw_data[4]>>1 | (int16_t)raw_data[5]<<7 ) & 0x07FF ) -1024;
    ch[4]=( ( (int16_t)raw_data[5]>>4 | (int16_t)raw_data[6]<<4 ) & 0x07FF ) -1024;
    ch[5]=( ( (int16_t)raw_data[6]>>7 | (int16_t)raw_data[7]<<1 | (int16_t)raw_data[8]<<9 ) & 0x07FF ) -1024;
    ch[6]=( ( (int16_t)raw_data[8]>>2 | (int16_t)raw_data[9]<<6 ) & 0x07FF ) -1024;
    ch[7]=( ( (int16_t)raw_data[9]>>5 | (int16_t)raw_data[10]<<3 ) & 0x07FF ) -1024;
    ch[8]=( ( (int16_t)raw_data[10]>>8 | (int16_t)raw_data[11]<<0 | (int16_t)raw_data[12]<<8 ) & 0x07FF ) -1024;
    ch[9]=( ( (int16_t)raw_data[12]>>3 | (int16_t)raw_data[13]<<5 ) & 0x07FF ) -1024;
    flat_byte=raw_data[23];
    stop_byte=raw_data[24];


    ch_data.rocker_l=ch[0];
    ch_data.rocker_l1=ch[1];
    ch_data.rocker_r=ch[2];
    ch_data.rocker_r1=ch[3];

    ch_data.sw1=ch[4];
    ch_data.sw2=ch[5];
    ch_data.sw3=ch[6];
    ch_data.sw4=ch[7];

    ch_data.vr1=ch[8];
    ch_data.vr2=ch[9];

    ch_data.b_lose=(flat_byte>>2)&0x01;
    ch_data.er_protect=(flat_byte>>3)&0x01;
    

    HAL_UARTEx_ReceiveToIdle_DMA(&huart3,raw_data,25);
    __HAL_DMA_DISABLE_IT(huart3.hdmarx,DMA_IT_HT);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
  if(huart==&huart3){
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3,raw_data,25);
    __HAL_DMA_DISABLE_IT(huart3.hdmarx,DMA_IT_HT);
  }
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3,raw_data,25);
    __HAL_DMA_DISABLE_IT(huart3.hdmarx,DMA_IT_HT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

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
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
