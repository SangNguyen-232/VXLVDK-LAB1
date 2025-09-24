/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (12-LED clock; Cycle1 -> Cycle2 -> Cycle3 realtime)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Cycle 3 step delay in milliseconds (1 second real-time) */
#define CYCLE3_STEP_MS 50

/* If LED is active HIGH set 1, if active LOW set 0 (default = 0) */
#define LED_ACTIVE_HIGH 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* RTC-like internal time for Cycle 3 (starts at 00:00:00) */
static int rtc_hour   = 0;
static int rtc_minute = 0;
static int rtc_second = 0;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Error_Handler(void);

/* Cycle functions */
void setNumberOnClock(int num);
void clearNumberOnClock(int num);
void displayClockRealtime(void);
void signalCycleEnd(int times, uint32_t on_ms, uint32_t off_ms);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* Map LED ports: use Led_x_GPIO_Port if CubeMX generated them, otherwise default to GPIOA */
#ifdef Led_1_GPIO_Port
  #define LED1_PORT Led_1_GPIO_Port
#else
  #define LED1_PORT GPIOA
#endif

#ifdef Led_2_GPIO_Port
  #define LED2_PORT Led_2_GPIO_Port
#else
  #define LED2_PORT GPIOA
#endif

#ifdef Led_3_GPIO_Port
  #define LED3_PORT Led_3_GPIO_Port
#else
  #define LED3_PORT GPIOA
#endif

#ifdef Led_4_GPIO_Port
  #define LED4_PORT Led_4_GPIO_Port
#else
  #define LED4_PORT GPIOA
#endif

#ifdef Led_5_GPIO_Port
  #define LED5_PORT Led_5_GPIO_Port
#else
  #define LED5_PORT GPIOA
#endif

#ifdef Led_6_GPIO_Port
  #define LED6_PORT Led_6_GPIO_Port
#else
  #define LED6_PORT GPIOA
#endif

#ifdef Led_7_GPIO_Port
  #define LED7_PORT Led_7_GPIO_Port
#else
  #define LED7_PORT GPIOA
#endif

#ifdef Led_8_GPIO_Port
  #define LED8_PORT Led_8_GPIO_Port
#else
  #define LED8_PORT GPIOA
#endif

#ifdef Led_9_GPIO_Port
  #define LED9_PORT Led_9_GPIO_Port
#else
  #define LED9_PORT GPIOA
#endif

#ifdef Led_10_GPIO_Port
  #define LED10_PORT Led_10_GPIO_Port
#else
  #define LED10_PORT GPIOA
#endif

#ifdef Led_11_GPIO_Port
  #define LED11_PORT Led_11_GPIO_Port
#else
  #define LED11_PORT GPIOA
#endif

#ifdef Led_12_GPIO_Port
  #define LED12_PORT Led_12_GPIO_Port
#else
  #define LED12_PORT GPIOA
#endif

/* LED on/off macros (auto-invert when active-low) */
#if LED_ACTIVE_HIGH
  #define LED_ON(port,pin)  HAL_GPIO_WritePin((port),(pin),GPIO_PIN_SET)
  #define LED_OFF(port,pin) HAL_GPIO_WritePin((port),(pin),GPIO_PIN_RESET)
#else
  #define LED_ON(port,pin)  HAL_GPIO_WritePin((port),(pin),GPIO_PIN_RESET)
  #define LED_OFF(port,pin) HAL_GPIO_WritePin((port),(pin),GPIO_PIN_SET)
#endif

/* led ports & pins arrays (index 0 -> Led_1, 11 -> Led_12) */
static GPIO_TypeDef* led_ports[12] = {
  LED1_PORT, LED2_PORT, LED3_PORT, LED4_PORT,
  LED5_PORT, LED6_PORT, LED7_PORT, LED8_PORT,
  LED9_PORT, LED10_PORT, LED11_PORT, LED12_PORT
};

static const uint16_t led_pins[12] = {
  Led_1_Pin,  Led_2_Pin,  Led_3_Pin,  Led_4_Pin,
  Led_5_Pin,  Led_6_Pin,  Led_7_Pin,  Led_8_Pin,
  Led_9_Pin,  Led_10_Pin, Led_11_Pin, Led_12_Pin
};

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals (GPIO for LEDs) */
  MX_GPIO_Init();

  /* Cycle 1: run once -> light a single LED (example: input 3 -> Led_3) */
  setNumberOnClock(3);
  HAL_Delay(1000);
  signalCycleEnd(1, 300, 300);

  /* Cycle 2: run once -> clear a single LED (example: input 7 -> Led_7) */
  clearNumberOnClock(7);
  HAL_Delay(1000);
  signalCycleEnd(1, 300, 300);

  /* Cycle 3: realtime clock starting at 00:00:00, runs forever */
  while (1)
  {
    displayClockRealtime();
    /* displayClockRealtime includes the delay (1 second) and time increment */
  }
}

/**
  * @brief System Clock Configuration
  *        Uses HSI, no PLL (matches many CubeMX default for low-end boards).
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
  *        Configures all LED pins as output push-pull.
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable GPIO ports clocks used by leds (common default GPIOA) */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* If some leds use other ports, enable those as well:
     __HAL_RCC_GPIOB_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE(); */

  /* Initialize all LED pins as outputs */
  GPIO_InitStruct.Pin = Led_1_Pin|Led_2_Pin|Led_3_Pin|Led_4_Pin
                      | Led_5_Pin|Led_6_Pin|Led_7_Pin|Led_8_Pin
                      | Led_9_Pin|Led_10_Pin|Led_11_Pin|Led_12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /* If some Led_x_Pin are on ports other than GPIOA, call HAL_GPIO_Init for those ports separately. */

  /* Set initial state: ensure LEDs are OFF according to LED_ACTIVE_HIGH */
  for (int i = 0; i < 12; ++i)
  {
    if (LED_ACTIVE_HIGH) HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_RESET);
    else                 HAL_GPIO_WritePin(led_ports[i], led_pins[i], GPIO_PIN_SET);
  }
}

/* =========================
   Cycle / helper functions
   ========================= */

/**
 * Cycle 1
 * Turn ON only the LED corresponding to num (0..11).
 */
void setNumberOnClock(int num)
{
  if (num < 0 || num > 11) return;
  for (int i = 0; i < 12; ++i)
  {
    if (i == num) LED_ON(led_ports[i], led_pins[i]);
    else          LED_OFF(led_ports[i], led_pins[i]);
  }
}

/**
 * Cycle 2
 * Turn OFF only the LED corresponding to num (0..11); turn ON all others.
 */
void clearNumberOnClock(int num)
{
  if (num < 0 || num > 11) return;
  for (int i = 0; i < 12; ++i)
  {
    if (i == num) LED_OFF(led_ports[i], led_pins[i]);
    else          LED_ON(led_ports[i], led_pins[i]);
  }
}

/**
 * Cycle 3 (realtime)
 * Display rtc_hour:rtc_minute:rtc_second on 12-LED clock and advance time by 1 second.
 * - Hour mapped to 0..11 via rtc_hour % 12
 * - Minute/second mapped by /5 to 0..11
 * This function blocks for CYCLE3_STEP_MS (1 second) per call.
 */
void displayClockRealtime(void)
{
  int h = (rtc_hour % 12 + 12) % 12;
  int m = ((rtc_minute / 5) % 12 + 12) % 12;
  int s = ((rtc_second / 5) % 12 + 12) % 12;

  /* Turn all off first */
  for (int i = 0; i < 12; ++i) LED_OFF(led_ports[i], led_pins[i]);

  /* Turn on hour, minute, second LEDs */
  LED_ON(led_ports[h], led_pins[h]);
  LED_ON(led_ports[m], led_pins[m]);
  LED_ON(led_ports[s], led_pins[s]);

  /* Wait one second (real) */
  HAL_Delay(CYCLE3_STEP_MS);

  /* Increment time */
  rtc_second++;
  if (rtc_second >= 60) { rtc_second = 0; rtc_minute++; }
  if (rtc_minute >= 60) { rtc_minute = 0; rtc_hour++; }
  if (rtc_hour >= 24)   { rtc_hour = 0; }
}

/**
 * Signal end of a cycle by blinking all LEDs (times times).
 */
void signalCycleEnd(int times, uint32_t on_ms, uint32_t off_ms)
{
  for (int t = 0; t < times; ++t)
  {
    for (int i = 0; i < 12; ++i) LED_ON(led_ports[i], led_pins[i]);
    HAL_Delay(on_ms);
    for (int i = 0; i < 12; ++i) LED_OFF(led_ports[i], led_pins[i]);
    HAL_Delay(off_ms);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    /* Blink all to indicate error */
    for (int i = 0; i < 12; ++i) LED_ON(led_ports[i], led_pins[i]);
    HAL_Delay(100);
    for (int i = 0; i < 12; ++i) LED_OFF(led_ports[i], led_pins[i]);
    HAL_Delay(100);
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief Reports the name of the source file and the source line number
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif /* USE_FULL_ASSERT */
