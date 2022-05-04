/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
typedef StaticEventGroup_t osStaticEventGroupDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define MODULE_NAME __FILE__
#define LOG(fmt, ...); printf("[LOG]["MODULE_NAME"](%s|%d)"fmt, \
		__func__, __LINE__, ##__VA_ARGS__ );
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for userTask */
osThreadId_t userTaskHandle;
uint32_t myTask02Buffer[ 128 ];
osStaticThreadDef_t myTask02ControlBlock;
const osThreadAttr_t userTask_attributes = {
  .name = "userTask",
  //.cb_mem = &myTask02ControlBlock,
  //.cb_size = sizeof(myTask02ControlBlock),
  //.stack_mem = myTask02Buffer,
  //.stack_size = 128*4,
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for myQueueD */
osMessageQueueId_t myQueueDHandle;
uint8_t myQueue01Buffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t myQueue01ControlBlock;
const osMessageQueueAttr_t myQueueD_attributes = {
  .name = "myQueueD",
  .cb_mem = &myQueue01ControlBlock,
  .cb_size = sizeof(myQueue01ControlBlock),
  .mq_mem = &myQueue01Buffer,
  .mq_size = sizeof(myQueue01Buffer)
};
/* Definitions for myQueueS */
osMessageQueueId_t myQueueSHandle;
uint8_t myQueue02Buffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t myQueue02ControlBlock;
const osMessageQueueAttr_t myQueueS_attributes = {
  .name = "myQueueS",
  .cb_mem = &myQueue02ControlBlock,
  .cb_size = sizeof(myQueue02ControlBlock),
  .mq_mem = &myQueue02Buffer,
  .mq_size = sizeof(myQueue02Buffer)
};
/* Definitions for myTimerD */
osTimerId_t myTimerDHandle;
const osTimerAttr_t myTimerD_attributes = {
  .name = "myTimerD"
};
/* Definitions for myTimerS */
osTimerId_t myTimerSHandle;
osStaticTimerDef_t myTimerSControlBlock;
const osTimerAttr_t myTimerS_attributes = {
  .name = "myTimerS",
  .cb_mem = &myTimerSControlBlock,
  .cb_size = sizeof(myTimerSControlBlock),
};
/* Definitions for myBinarySemD */
osSemaphoreId_t myBinarySemDHandle;
const osSemaphoreAttr_t myBinarySemD_attributes = {
  .name = "myBinarySemD"
};
/* Definitions for myBinarySemS */
osSemaphoreId_t myBinarySemSHandle;
osStaticSemaphoreDef_t myBinarySemSControlBlock;
const osSemaphoreAttr_t myBinarySemS_attributes = {
  .name = "myBinarySemS",
  .cb_mem = &myBinarySemSControlBlock,
  .cb_size = sizeof(myBinarySemSControlBlock),
};
/* Definitions for myCountingSemD */
osSemaphoreId_t myCountingSemDHandle;
const osSemaphoreAttr_t myCountingSemD_attributes = {
  .name = "myCountingSemD"
};
/* Definitions for myCountingSemS */
osSemaphoreId_t myCountingSemSHandle;
osStaticSemaphoreDef_t myCountingSemSControlBlock;
const osSemaphoreAttr_t myCountingSemS_attributes = {
  .name = "myCountingSemS",
  .cb_mem = &myCountingSemSControlBlock,
  .cb_size = sizeof(myCountingSemSControlBlock),
};
/* Definitions for myEvent01 */
osEventFlagsId_t myEvent01Handle;
const osEventFlagsAttr_t myEvent01_attributes = {
  .name = "myEvent01"
};
/* Definitions for myEvent02 */
osEventFlagsId_t myEvent02Handle;
osStaticEventGroupDef_t myEvent02ControlBlock;
const osEventFlagsAttr_t myEvent02_attributes = {
  .name = "myEvent02",
  .cb_mem = &myEvent02ControlBlock,
  .cb_size = sizeof(myEvent02ControlBlock),
};
/* USER CODE BEGIN PV */
__IO int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void *argument);
void StartUserTask(void *argument);
void Callback01(void *argument);
void CallbackS(void *argument);

/* USER CODE BEGIN PFP */
extern void initialise_monitor_handles(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*int _write(int file , char *ptr, int len)
{
    for(int i = 0; i<len; i++)
        ITM_SendChar((*ptr++));
    return len;
}

int _read(int file , char *ptr, int len)
{
	//int32_t ch = -1;
	int nLen = 0;

	return nLen;
}*/
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  initialise_monitor_handles();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myBinarySemD */
  myBinarySemDHandle = osSemaphoreNew(1, 1, &myBinarySemD_attributes);

  /* creation of myBinarySemS */
  myBinarySemSHandle = osSemaphoreNew(1, 1, &myBinarySemS_attributes);

  /* creation of myCountingSemD */
  myCountingSemDHandle = osSemaphoreNew(2, 2, &myCountingSemD_attributes);

  /* creation of myCountingSemS */
  myCountingSemSHandle = osSemaphoreNew(2, 2, &myCountingSemS_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimerD */
  myTimerDHandle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimerD_attributes);

  /* creation of myTimerS */
  myTimerSHandle = osTimerNew(CallbackS, osTimerPeriodic, NULL, &myTimerS_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueueD */
  myQueueDHandle = osMessageQueueNew (16, sizeof(uint16_t), &myQueueD_attributes);

  /* creation of myQueueS */
  myQueueSHandle = osMessageQueueNew (16, sizeof(uint16_t), &myQueueS_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of userTask */
  userTaskHandle = osThreadNew(StartUserTask, NULL, &userTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of myEvent01 */
  myEvent01Handle = osEventFlagsNew(&myEvent01_attributes);

  /* creation of myEvent02 */
  myEvent02Handle = osEventFlagsNew(&myEvent02_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  int nCnt = 0;
  for(;;)
  {
	  char u[30]={0};
	  fgets(u, 25, stdin);
	  LOG("idle routine %d HCLKFreq = %dMHz input = %s\n", nCnt,
			  (int)HAL_RCC_GetHCLKFreq() / 1000000,
			  u);
	  nCnt++;
	  osDelay(1000);

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartUserTask */
/**
* @brief Function implementing the userTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUserTask */
void StartUserTask(void *argument)
{
  /* USER CODE BEGIN StartUserTask */
  /* Infinite loop */
  for(;;)
  {
	char strRev[30]="";
	char str[100] = "";
	HAL_UART_Receive(&huart1, (uint8_t *)strRev, 30, 1000);
	sprintf(str, "PC upload %s\n", strRev);
	HAL_UART_Transmit(&huart1, (uint8_t *)str, (uint16_t)strlen(str), 1000);
    osDelay(1000);
  }
  /* USER CODE END StartUserTask */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */

  /* USER CODE END Callback01 */
}

/* CallbackS function */
void CallbackS(void *argument)
{
  /* USER CODE BEGIN CallbackS */

  /* USER CODE END CallbackS */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
