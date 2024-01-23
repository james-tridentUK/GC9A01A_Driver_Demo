/**
 ******************************************************************************
 * @file           : trident_fpu.c
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
#include <stdint.h>
#include "trident_fpu_f4.h"
#include "trident_spi_f4.h"
#include "trident_gpio_f4.h"
#include "trident_timebase.h"
#include "GC9A01.h"
#include "math.h"

#include "lv_port_disp.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
static void GPIO_Configure(void);
/************************************
 * STATIC FUNCTIONS
 ************************************/

/**
 * @brief Configure GPIO pins
 *
 */
static void GPIO_Configure(void)
{
	/*Start the GPIOA clock*/
	GPIO_Clock_Init(GPIOA_CLK);

	/*Start the GPIOC clock*/
	GPIO_Clock_Init(GPIOC_CLK);

	/*LCD Rst Pin*/
	GPIO_PinConfig(GPIO_MODE_OUTPUT, LCD_RST_Port, LCD_RST_Pin);
	GPIO_SetOutputType(GPIO_OUT_PUSH_PULL, LCD_RST_Port, LCD_RST_Pin);
	GPIO_SetOutputSpeed(GPIO_OUT_SPEED_HIGH, LCD_RST_Port, LCD_RST_Pin);

	/*LCD DC Pin*/
	GPIO_PinConfig(GPIO_MODE_OUTPUT, LCD_DC_Port, LCD_DC_Pin);
	GPIO_SetOutputType(GPIO_OUT_PUSH_PULL, LCD_DC_Port, LCD_DC_Pin);
	GPIO_SetOutputSpeed(GPIO_OUT_SPEED_HIGH, LCD_DC_Port, LCD_DC_Pin);

	/*LCD SPI chip select*/
	GPIO_PinConfig(GPIO_MODE_OUTPUT, SPI1_CS_Port, SPI1_CS_Pin);
	GPIO_SetOutputType(GPIO_OUT_PUSH_PULL, SPI1_CS_Port, SPI1_CS_Pin);
	GPIO_SetOutputSpeed(GPIO_OUT_SPEED_HIGH, SPI1_CS_Port, SPI1_CS_Pin);

	/*User LED Pin*/
	GPIO_PinConfig(GPIO_MODE_OUTPUT, LED_Port, LED_Pin);
	GPIO_SetOutputType(GPIO_OUT_PUSH_PULL, LED_Port, LED_Pin);
	GPIO_SetOutputSpeed(GPIO_OUT_SPEED_HIGH, LED_Port, LED_Pin);
}
/************************************
 * GLOBAL FUNCTIONS
 ************************************/

#if defined ( __GNUC__) && !defined(__clang__)
/* With GCC, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define UART_DISPLAY int __io_putchar(int ch)
#else
#define UART_DISPLAY int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int main(void)
{

	/*Enable the floating point unit*/
	FPU_Enable();

	/*Init the timebase*/
	Time_Timebase_Init();

	HSPI1.Instance = SPI1;

	/*Init SPI*/
	SPI_Init(&HSPI1);

	/*Configure the GPIO*/
	GPIO_Configure();

	/*Init the display*/
	lv_port

	/* Loop forever */
	while (1)
	{



		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
UART_DISPLAY
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	//HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
