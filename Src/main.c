/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "hd44780u.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LED_PATTERN_0            BIT(0)
#define LED_PATTERN_1            BIT(1)
#define LED_PATTERN_2            BIT(2)
#define LED_PATTERN_3            BIT(3)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void StartDefaultTask(void const * argument);
void LED_blink(void *pvParameters);
void update_delay(uint32_t *delay);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void udpate_leds_0()
{
  static int count = 0;
  int pin_order[] = {13, 14, 15, 12};
  int pin_to_set = 0;
  int value = 0;

  switch ( count ) {
  case 0:
    pin_to_set = pin_order[0];
    value = 1;
    count++;
    break;
  case 1:
    pin_to_set = pin_order[0];
    value = 0;
    count++;
    break;
  case 2:
    pin_to_set = pin_order[1];
    value = 1;
    count++;
    break;
  case 3:
    pin_to_set = pin_order[1];
    value = 0;
    count++;
    break;
  case 4:
    pin_to_set = pin_order[2];
    value = 1;
    count++;
    break;
  case 5: 
    pin_to_set = pin_order[2];
    value = 0;
    count++;
    break;
  case 6:
    pin_to_set = pin_order[3];
    value = 1;
    count++;
    break;
  case 7:
    pin_to_set = pin_order[3];
    value = 0;
    count = 0;
    break;
  default:
    count = 0;
  }

  gpio_gp_pin_set(GPIOD_ADDR, pin_to_set, value);
}

void udpate_leds_1()
{
  static int count = 0;

  switch ( count ) {
  case 0:
    gpio_gp_pin_set(GPIOD_ADDR, 12, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 13, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 1);
    count++;
    break;
  case 1:
    count++;
    break;
  case 2:
    count++;
    break;
  case 3:
    count++;
    break;
  case 4:
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    count++;
    break;
  case 5: 
    count++;
    break;
  case 6:
    count++;
    break;
  case 7:
    count = 0;
    break;
  default:
    count = 0;
  }

}

void udpate_leds_2()
{
  static int count = 0;

  switch ( count ) {
  case 0:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 1:
    count++;
    break;
  case 2:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 3:
    count++;
    break;
  case 4:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 5:
    count++;
    break;
  case 6:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 1);
    count++;
    break;
  case 7:
    count = 0;
    break;
  default:
    count = 0;
  }

}

void udpate_leds_3()
{
  static int count = 0;

  switch ( count ) {
  case 0:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 1);
    count++;
    break;
  case 1:
    count++;
    break;
  case 2:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 3:
    count++;
    break;
  case 4:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 5:
    count++;
    break;
  case 6:
    gpio_gp_pin_set(GPIOD_ADDR, 13, 1);
    gpio_gp_pin_set(GPIOD_ADDR, 14, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 15, 0);
    gpio_gp_pin_set(GPIOD_ADDR, 12, 0);
    count++;
    break;
  case 7:
    count = 0;
    break;
  default:
    count = 0;
  }

}

/* Shifts the hd44780 display to the left */
void HD44780u_display_shifter( void * pvParameters )
{
  TickType_t xDelay = 300 / portTICK_PERIOD_MS;

  while ( 1 ) {
    vTaskDelay(xDelay);
    hd44780u_4bit_shift_display_left();
  }
}

/* Waits for button click */
void listener( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreate() below. */
  TickType_t xDelay = 100 / portTICK_PERIOD_MS;
  EventGroupHandle_t xEventGroup = (EventGroupHandle_t) pvParameters;
  int i = 0;

  const char *messages[] = {
    "Rickard <3 är bra!",
    "Han är duktig på detta",
    "Kolla ett nytt meddelande!"
  };

  int prev_led_pattern = -1;
  int led_pattern = LED_PATTERN_0;

  // Await button press
  while ( 1 ) {
    vTaskDelay(xDelay);

    if ( gpio_gp_pin_get(GPIOB_ADDR, 5) ) {
      led_pattern = LED_PATTERN_1;
    } else {
      led_pattern = LED_PATTERN_0;
    }

    if ( prev_led_pattern != led_pattern ) {
      xEventGroupSetBits(xEventGroup, led_pattern);
      prev_led_pattern = led_pattern;
    }

    if ( led_pattern == LED_PATTERN_1 )
      break;
  }

  // Writing a message!
  hd44780u_4bit_write(messages[i++]);
  vTaskDelay(xDelay);

  led_pattern = LED_PATTERN_0;
  prev_led_pattern = led_pattern;
  while ( 1 ) {
    vTaskDelay(xDelay);

    if ( gpio_gp_pin_get(GPIOB_ADDR, 5) ) {
      led_pattern = LED_PATTERN_2;
    } else {
      led_pattern = LED_PATTERN_3;
    }

    if ( prev_led_pattern != led_pattern ) {
      xEventGroupSetBits(xEventGroup, led_pattern);
      prev_led_pattern = led_pattern;

      if ( led_pattern != LED_PATTERN_3 ) {
        ++i;
        i = (i) % ( 3 );
        hd44780u_4bit_write(messages[i]);
      }
    }

    prev_led_pattern = led_pattern;
  }
}

/* Task to be created. */
void LED_blink( void * pvParameters )
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreate() below. */
  uint32_t delay = 80000;
  uint32_t delay_count = 0;

  EventGroupHandle_t xEventGroup = (EventGroupHandle_t) pvParameters;

  int led_pattern = LED_PATTERN_0;

  while (1)
  {
    /* Wait to be notified of an interrupt. */
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 1 / portTICK_PERIOD_MS;

    /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
    the event group.  Clear the bits before exiting. */
    uxBits = xEventGroupWaitBits(
      xEventGroup,   /* The event group being tested. */
      LED_PATTERN_0 | LED_PATTERN_1 | LED_PATTERN_2 | LED_PATTERN_3 , /* The bits within the event group to wait for. */
      pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
      pdFALSE,       /* Don't wait for both bits, either bit will do. */
      xTicksToWait );/* Wait a maximum of 100ms for either bit to be set. */

    if ( uxBits & LED_PATTERN_0 ) {
      led_pattern = LED_PATTERN_0;
    } else if ( uxBits & LED_PATTERN_1 ) {
      led_pattern = LED_PATTERN_1;
    } else if ( uxBits & LED_PATTERN_2 ) {
      led_pattern = LED_PATTERN_2;
    } else if ( uxBits & LED_PATTERN_3 ) {
      led_pattern = LED_PATTERN_3;
    }

    switch( led_pattern ) {
    case LED_PATTERN_0:
    udpate_leds_0();
    update_delay(&delay);
    break;
    case LED_PATTERN_1:
    udpate_leds_1();
    delay = 80000;
    break;
    case LED_PATTERN_2:
    udpate_leds_2();
    delay = 100000;
    break;
    case LED_PATTERN_3:
    udpate_leds_3();
    delay = 100000;
    break;
    default:
    break;
    }

    delay_count = 0;
    while ( delay_count < delay ) {
    ++delay_count;
    }

  /* USER CODE BEGIN 3 */
  }
}

/* USER CODE BEGIN 4 */

void update_delay(uint32_t *delay)
{
  static uint32_t count = 0;
  static uint32_t trend_break = 50;
  static uint32_t magnitude = 1000;

  if ( count < trend_break ) {
    *delay += magnitude;
  } else if ( count > trend_break ){
    *delay -= magnitude;
  }

  if ( count == trend_break*2 ){
    count = 0;
  } else {
    count++;
  }
}

/* Function that creates a task. */
int spawn_GPIO_App( void )
{
  TaskHandle_t xHandle = NULL;
  EventGroupHandle_t xCreatedEventGroup;

  xCreatedEventGroup = xEventGroupCreate();

  if ( xCreatedEventGroup == NULL )
    return 1;

  gpio_setting_t settings[] =
  {
    // For the blinking
    {
      .pin = 12,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP
    },
    {
      .pin = 13,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP
    },
    {
      .pin = 14,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP
    },
    {
      .pin = 15,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP
    },
    // For the button
    {
      .pin = 5,
      .base = GPIOB_ADDR,
      .mode = GPIO_SETTING_MODE_IN,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP
    },
    // for the hd44780u LCD display
    {
      // PD0 -> DB7
      .pin = 0,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP     
    },
    {
      // PD2 -> DB6
      .pin = 2,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP     
    },
    {
      // PD4 -> DB5
      .pin = 4,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP     
    },
    {
      // PD6 -> DB4
      .pin = 6,
      .base = GPIOD_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP     
    },
    {
      // PC9 -> E
      .pin = 9,
      .base = GPIOC_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP    
    },
    {
      // PC7 -> RS
      .pin = 7,
      .base = GPIOC_ADDR,
      .mode = GPIO_SETTING_MODE_GP_OUT,
      .type = GPIO_SETTING_TYPE_PUSH_PULL,
      .speed = GPIO_SETTING_SPEED_LOW,
      .pupdr = GPIO_SETTING_PUPDR_NO_PULL_UP  
    },
    // Terminating entry
    { 
      .pin = -1
    }
  };

  hd44780u_config_t hd447_settings[] =
  {
    {
      .type = HD44780U_DB4,
      .pin = {
        .pin = 6,
        .base = GPIOD_ADDR
      }
    },
    {
      .type = HD44780U_DB5,
      .pin = {
        .pin = 4,
        .base = GPIOD_ADDR
      }
    },
    {
      .type = HD44780U_DB6,
      .pin = {
        .pin = 2,
        .base = GPIOD_ADDR
      }
    },
    {
      .type = HD44780U_DB7,
      .pin = {
        .pin = 0,
        .base = GPIOD_ADDR
      }
    },
    {
      .type = HD44780U_E,
      .pin = {
        .pin = 9,
        .base = GPIOC_ADDR
      }
    },
    {
      .type = HD44780U_RS,
      .pin = {
        .pin = 7,
        .base = GPIOC_ADDR
      }
    },
    // Terminating entry
    {
      .type = -1
    }

  };

  // Init the GPIO pins
  init_gpio_pins(settings);

  // Init the hd44780u interface
  hd44780u_init_4bit_op(hd447_settings);

  /* Create the task, storing the handle. */
  xTaskCreate(
    LED_blink,        /* Function that implements the task. */
    "LED blinker!",   /* Text name for the task. */
    128,              /* Stack size in words, not bytes. */
    xCreatedEventGroup,   /* Parameter passed into the task. */
    tskIDLE_PRIORITY, /* Priority at which the task is created. */
    &xHandle );       /* Used to pass out the created task's handle. */

  xTaskCreate(
      listener,        /* Function that implements the task. */
      "LED blinker alterer!",   /* Text name for the task. */
      128,              /* Stack size in words, not bytes. */
      xCreatedEventGroup,   /* Parameter passed into the task. */
      tskIDLE_PRIORITY, /* Priority at which the task is created. */
      &xHandle );       /* Used to pass out the created task's handle. */

  xTaskCreate(
      HD44780u_display_shifter,        /* Function that implements the task. */
      "hd44780u display shifter!",   /* Text name for the task. */
      128,              /* Stack size in words, not bytes. */
      xCreatedEventGroup,   /* Parameter passed into the task. */
      tskIDLE_PRIORITY, /* Priority at which the task is created. */
      &xHandle );       /* Used to pass out the created task's handle. */

  return 0;
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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  /* osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);*/

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    /* Declare a variable to hold the created event group. */
  EventGroupHandle_t xCreatedEventGroup;

  /* Attempt to create the event group. */
  xCreatedEventGroup = xEventGroupCreate();

  if ( xCreatedEventGroup != NULL ) {

    spawn_GPIO_App();

  }



  /* USER CODE END RTOS_THREADS */

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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
