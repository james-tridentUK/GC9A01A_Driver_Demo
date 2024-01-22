/**
 ******************************************************************************
 * @file           : trident_gpio_f4.c
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
#include "trident_gpio_f4.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define GPIOAEN		(1U<<0)
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

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

/**
 * @brief Activate a GPIO AHB1 Clock
 *
 * @param gpio_port_clk
 */
void GPIO_Clock_Init(GPIO_CLK_t gpio_port_clk)
{
	/*Activate the appropriate gpio bus clock*/
	if (gpio_port_clk == GPIOA_CLK)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	}
	if (gpio_port_clk == GPIOB_CLK)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}
	if (gpio_port_clk == GPIOC_CLK)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	}
	if (gpio_port_clk == GPIOD_CLK)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	}
	if (gpio_port_clk == GPIOE_CLK)
	{
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	}
}

/**
 * @brief DeActivate a GPIO AHB1 Clock
 *
 * @param gpio_port_clk
 */
void GPIO_Clock_DeInit(GPIO_CLK_t gpio_port_clk)
{
	/*Activate the appropriate gpio bus clock*/
	if (gpio_port_clk == GPIOA_CLK)
	{
		RCC->AHB1ENR &= ~ RCC_AHB1ENR_GPIOAEN;
	}
	if (gpio_port_clk == GPIOB_CLK)
	{
		RCC->AHB1ENR &= ~ RCC_AHB1ENR_GPIOBEN;
	}
	if (gpio_port_clk == GPIOC_CLK)
	{
		RCC->AHB1ENR &= ~ RCC_AHB1ENR_GPIOCEN;
	}
	if (gpio_port_clk == GPIOD_CLK)
	{
		RCC->AHB1ENR &= ~ RCC_AHB1ENR_GPIODEN;
	}
	if (gpio_port_clk == GPIOE_CLK)
	{
		RCC->AHB1ENR &= ~ RCC_AHB1ENR_GPIOEEN;
	}
}

void GPIO_PinConfig(GPIO_Mode_t mode, GPIO_TypeDef *port,
		uint16_t pin)
{
	/*GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG*/

	/*
	 00: Input
	 01: Output
	 10: Alternate function
	 11: Analog
	 */

	port->MODER &= ~(1U << (pin * 2));      // Clear existing setting
	port->MODER &= ~(1U << (pin * 2 + 1));   // Clear existing setting

	switch (mode)
	{
	case GPIO_MODE_INPUT:

		port->MODER &= ~(1U << (pin * 2));      // Set as input
		port->MODER &= ~(1U << (pin * 2 + 1));   // Set as input

		break;
	case GPIO_MODE_OUTPUT:

		port->MODER |= (1U << (pin * 2));   // Set as Output
		port->MODER &= ~(1U << (pin * 2 + 1));

		break;

	case GPIO_MODE_AF:

		port->MODER |= (1U << (pin * 2 + 1));   // Set as AF
		port->MODER &= ~(1U << (pin * 2));

		break;

	case GPIO_MODE_ANALOG:

		port->MODER |= (1U << (pin * 2));   // Set as Analog
		port->MODER |= ((pin * 2 + 1));

		break;

	default:

		break;

	}

}

/*Set Output type*/
void GPIO_SetOutputType(GPIO_OutputType_t oType, GPIO_TypeDef *port,
		uint16_t pin)
{
	switch (oType)
	{
	case GPIO_OUT_PUSH_PULL:

		port->OTYPER &= ~(1U << pin);

		break;

	case GPIO_OUT_OPEN_DRAIN:

		port->OTYPER |= (1U << pin);

		break;
	}

}

/*Set Output speed*/
void GPIO_SetOutputSpeed(GPIO_OutputSpeed_t speed, GPIO_TypeDef *port,
		uint16_t pin)
{

	switch (speed)
	{
	case GPIO_OUT_SPEED_VLOW:

		port->OSPEEDR &= ~(1U << (pin * 2));      // Set as very low speed
		port->OSPEEDR &= ~(1U << (pin * 2 + 1));

		break;

	case GPIO_OUT_SPEED_LOW:

		port->OSPEEDR |= (1U << (pin * 2));      // Set as low speed
		port->OSPEEDR &= ~(1U << (pin * 2 + 1));

		break;

	case GPIO_OUT_SPEED_HIGH:

		port->OSPEEDR &= ~(1U << (pin * 2));      // Set as high speed
		port->OSPEEDR |= (1U << (pin * 2 + 1));

		break;

	case GPIO_OUT_SPEED_VHIGH:

		port->OSPEEDR |= (1U << (pin * 2));      // Set as very high speed
		port->OSPEEDR |= (1U << (pin * 2 + 1));

		break;
	}

}

/*Set pull up or pull down resistors in GPIO*/
void GPIO_SetPullUpDown(GPIO_PUPD_t state, GPIO_TypeDef *port,
		uint16_t pin)
{

	switch (state)
		{
		case GPIO_PUPD_NONE:

			port->OSPEEDR &= ~(1U << (pin * 2));      // Set as very low speed
			port->OSPEEDR &= ~(1U << (pin * 2 + 1));

			break;

		case GPIO_PUPD_UP:

			port->OSPEEDR |= (1U << (pin * 2));      // Set as low speed
			port->OSPEEDR &= ~(1U << (pin * 2 + 1));

			break;

		case GPIO_PUPD_DOWN:

			port->OSPEEDR &= ~(1U << (pin * 2));      // Set as high speed
			port->OSPEEDR |= (1U << (pin * 2 + 1));

			break;
		}

}

/*Set the gpio output*/
void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t val)
{
	if (val > 0)
	{
		port->ODR |= (1U << pin);
	}
	else
	{
		port->ODR &= ~(1U << pin);
	}
}

/*Toggle the gpio output*/
void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pin)
{
	port->ODR ^= (1U << pin);
}


