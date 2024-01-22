/**
 ******************************************************************************
 * @file           : trident_timebase_f4.c
 * @author         : User
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Trident Software Systems Ltd UK
 * All rights reserved. 
 *
 ******************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "trident_timebase.h"
#include "stm32f4xx.h"
#include "stm32f401xc.h"
#include "trident_gpio_f4.h"
#include "system_stm32f4xx.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define CTRL_ENABLE		(1U<<0)
#define CTRL_TICKINT	(1U<<1)
#define CTRL_CLCKSRC	(1U<<2)
#define CTRL_COUNTFLAG	(1U<<16)

#define ONE_SEC_LOAD	 84000000

#define TICK_FREQ		 1
#define 	SET_BIT_FIELD(REG, MASK, VALUE, OFFSET)   REG = ((REG) &~(MASK)) | (((VALUE) << (OFFSET)) & (MASK));

#if !defined  (HSE_VALUE)
#define HSE_VALUE    ((uint32_t)25000000) /*!< Default value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSI_VALUE)
#define HSI_VALUE    ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/
volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;
uint32_t SystemCoreClock = 16000000;
const uint8_t AHBPrescTable[16] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9 };
const uint8_t APBPrescTable[8] =
{ 0, 0, 0, 0, 1, 2, 3, 4 };

uint8_t pll_m = 25;
uint8_t pll_n = 168; //192
uint8_t pll_p = 0x00;
uint8_t pll_q = 4;
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void tick_increment(void);
/************************************
 * STATIC FUNCTIONS
 ************************************/
static void tick_increment(void)
{
	g_curr_tick += TICK_FREQ;
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
 * @brief Adds a blocking delay
 *
 * @param delay in ms
 */
void Time_Delay_Ms(uint32_t delay)
{
	uint32_t tickstart = Time_Get_Tick();
	uint32_t wait = delay;

	if (wait < MAX_DELAY)
	{
		wait += (uint32_t) TICK_FREQ;
	}

	while ((Time_Get_Tick() - tickstart) < wait)
	{
	}

}

/**
 * @brief Return the current tick count
 *
 * @return uint32_t current ticks
 */
uint32_t Time_Get_Tick(void)
{
	__disable_irq();
	g_curr_tick_p = g_curr_tick;
	__enable_irq();

	return g_curr_tick_p;
}

/**
 * @brief Init the timebase and set core clock to 84MHZ
 *
 */
void Time_Timebase_Init(void)
{

	/*Disable global interrupts*/
	__disable_irq();

	RCC->PLLCFGR = 0x00000000;

	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
	{
	}

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_VOS;
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN | FLASH_ACR_DCEN |
	FLASH_ACR_LATENCY_3WS;

	RCC->PLLCFGR |= (pll_q << 24) | (pll_p << 16) | (pll_n << 6) | (pll_m << 0);

	RCC->PLLCFGR |= (1U << 22U);

	//	RCC -> APB2ENR |= 0x4000;
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
	{
	}


	RCC->CFGR &=~(1U << 0U);
	RCC->CFGR |= (1U << 1U);

	while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL)
		;

	SystemCoreClockUpdate();

	/*Load the timer with number of clock cycles per second*/
	SysTick->LOAD = ONE_SEC_LOAD - 1;

	/*Clear systick current value register*/
	SysTick->VAL = 0;

	/*Select internal clock source*/
	SysTick->CTRL = CTRL_CLCKSRC;

	/*Enable interrupt*/
	SysTick->CTRL |= CTRL_TICKINT;

	/*Enable systick*/
	SysTick->CTRL |= CTRL_ENABLE;

	/*Enable global interrupts*/
	__enable_irq();
}

/**
 * @brief Routine handler for systick increment
 *
 */
void SysTick_Handler(void)
{
	tick_increment();
}

void SystemCoreClockUpdate(void)
{
  uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;

  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock source */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock source */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock source */

      /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */
      pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
      pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

      if (pllsource != 0)
      {
        /* HSE used as PLL clock source */
        pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }
      else
      {
        /* HSI used as PLL clock source */
        pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
      }

      pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
      SystemCoreClock = pllvco/pllp;
      break;
    default:
      SystemCoreClock = HSI_VALUE;
      break;
  }
  /* Compute HCLK frequency --------------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK frequency */
  SystemCoreClock >>= tmp;
}
