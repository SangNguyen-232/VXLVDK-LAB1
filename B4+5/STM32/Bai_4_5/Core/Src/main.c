#include "main.h"

/* Timing constants (seconds) */
#define NS_RED_TIME  9  /* NS red counts from 8 to 0 */
#define EW_GREEN_TIME 5 /* EW green counts from 5 to 0 */
#define EW_YELLOW_TIME 3 /* EW yellow counts from 2 to 0 */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* Helper prototypes */
void display7SEG_NS(int num);
void display7SEG_EW(int num);
void clear7SEG_NS(void);
void clear7SEG_EW(void);
void setLED(GPIO_TypeDef* port, uint16_t pin, uint8_t on);
void setNS(uint8_t g, uint8_t y, uint8_t r);
void setEW(uint8_t g, uint8_t y, uint8_t r);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* Init: turn off traffic LEDs and clear both 7-seg */
  setNS(0,0,0);
  setEW(0,0,0);
  clear7SEG_NS();
  clear7SEG_EW();

  /* Main loop: two-phase traffic controller */
  while (1)
  {
    /* Phase A: North-South RED, East-West GREEN then YELLOW */
    setNS(0,0,1);   /* NS red on */
    setEW(1,0,0);   /* EW green on */
    for (int s = 0; s < EW_GREEN_TIME; s++) {
      display7SEG_NS(NS_RED_TIME - s - 1); /* NS red counts 8->4 */
      display7SEG_EW(EW_GREEN_TIME - s - 1); /* EW green counts 5->1 */
      HAL_Delay(1000);
    }
    setEW(0,1,0);   /* EW yellow on */
    for (int s = 0; s < EW_YELLOW_TIME + 1; s++) {
      display7SEG_NS(NS_RED_TIME - EW_GREEN_TIME - s - 1); /* NS red counts 3->0 */
      display7SEG_EW(EW_YELLOW_TIME - s); /* EW yellow counts 2->0 */
      HAL_Delay(1000);
    }

    /* Phase B: East-West RED, North-South GREEN then YELLOW */
    setNS(1,0,0);   /* NS green on */
    setEW(0,0,1);   /* EW red on */
    for (int s = 0; s < EW_GREEN_TIME; s++) {
      display7SEG_EW(NS_RED_TIME - s - 1); /* EW red counts 8->4 */
      display7SEG_NS(EW_GREEN_TIME - s - 1); /* NS green counts 5->1 */
      HAL_Delay(1000);
    }
    setNS(0,1,0);   /* NS yellow on */
    for (int s = 0; s < EW_YELLOW_TIME + 1; s++) {
      display7SEG_EW(NS_RED_TIME - EW_GREEN_TIME - s - 1); /* EW red counts 3->0 */
      display7SEG_NS(EW_YELLOW_TIME - s); /* NS yellow counts 2->0 */
      HAL_Delay(1000);
    }
  }
}

/* System Clock Configuration (standard HSI) */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                   |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* GPIO init: configure PA (L1..L6) and PB (a..g, A..G) as outputs */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Traffic LEDs (PA) - set pins OFF initially (SET = off for active-LOW) */
  HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(L3_GPIO_Port, L3_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(L4_GPIO_Port, L4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(L5_GPIO_Port, L5_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(L6_GPIO_Port, L6_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = L1_Pin|L2_Pin|L3_Pin|L4_Pin|L5_Pin|L6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(L1_GPIO_Port, &GPIO_InitStruct); /* all Lx on same port (GPIOA) */

  /* 7-seg NS (a..g) and EW (A..G) on PB - set OFF initially (SET = off for common-anode) */
  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(F_GPIO_Port, F_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(G_GPIO_Port, G_Pin, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin
                      | A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(a_GPIO_Port, &GPIO_InitStruct); /* all segment pins on GPIOB */
}

/* USER CODE BEGIN 4 */

/* Clear (turn off) NS 7-seg (a..g) */
void clear7SEG_NS(void) {
  HAL_GPIO_WritePin(a_GPIO_Port, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
}

/* Clear (turn off) EW 7-seg (A..G) */
void clear7SEG_EW(void) {
  HAL_GPIO_WritePin(A_GPIO_Port, A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin, GPIO_PIN_SET);
}

/* Display digit 0..9 on NS 7-seg (a..g). If out of range, remains cleared. */
void display7SEG_NS(int num) {
  clear7SEG_NS();
  const uint16_t digits[10] = {
    a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin,    /* 0 */
    b_Pin|c_Pin,                            /* 1 */
    a_Pin|b_Pin|d_Pin|e_Pin|g_Pin,          /* 2 */
    a_Pin|b_Pin|c_Pin|d_Pin|g_Pin,          /* 3 */
    b_Pin|c_Pin|f_Pin|g_Pin,                /* 4 */
    a_Pin|c_Pin|d_Pin|f_Pin|g_Pin,          /* 5 */
    a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin,    /* 6 */
    a_Pin|b_Pin|c_Pin,                      /* 7 */
    a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin,/* 8 */
    a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin     /* 9 */
  };
  if (num >= 0 && num <= 9) {
    HAL_GPIO_WritePin(a_GPIO_Port, digits[num], GPIO_PIN_RESET); /* LOW = on */
  }
}

/* Display digit 0..9 on EW 7-seg (A..G). If out of range, remains cleared. */
void display7SEG_EW(int num) {
  clear7SEG_EW();
  const uint16_t digits[10] = {
    A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin,    /* 0 */
    B_Pin|C_Pin,                            /* 1 */
    A_Pin|B_Pin|D_Pin|E_Pin|G_Pin,          /* 2 */
    A_Pin|B_Pin|C_Pin|D_Pin|G_Pin,          /* 3 */
    B_Pin|C_Pin|F_Pin|G_Pin,                /* 4 */
    A_Pin|C_Pin|D_Pin|F_Pin|G_Pin,          /* 5 */
    A_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin,    /* 6 */
    A_Pin|B_Pin|C_Pin,                      /* 7 */
    A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin,/* 8 */
    A_Pin|B_Pin|C_Pin|D_Pin|F_Pin|G_Pin     /* 9 */
  };
  if (num >= 0 && num <= 9) {
    HAL_GPIO_WritePin(A_GPIO_Port, digits[num], GPIO_PIN_RESET); /* LOW = on */
  }
}

/* setLED helper (active-LOW): on=1 -> LOW (turn on), on=0 -> HIGH (turn off) */
void setLED(GPIO_TypeDef* port, uint16_t pin, uint8_t on) {
  HAL_GPIO_WritePin(port, pin, on ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/* Set North-South lights (g,y,r) each 0/1 */
void setNS(uint8_t g, uint8_t y, uint8_t r) {
  setLED(L1_GPIO_Port, L1_Pin, g); /* Green */
  setLED(L2_GPIO_Port, L2_Pin, y); /* Yellow */
  setLED(L3_GPIO_Port, L3_Pin, r); /* Red */
}

/* Set East-West lights (g,y,r) each 0/1 */
void setEW(uint8_t g, uint8_t y, uint8_t r) {
  setLED(L4_GPIO_Port, L4_Pin, g); /* Green */
  setLED(L5_GPIO_Port, L5_Pin, y); /* Yellow */
  setLED(L6_GPIO_Port, L6_Pin, r); /* Red */
}

/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1) { }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Optional: report file/line */
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
