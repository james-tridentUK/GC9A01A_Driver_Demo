/**
 ******************************************************************************
 * @file           : trident_spi_f4.c
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
#include "trident_spi_f4.h"
#include "trident_gpio_f4.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define GPIOAEN		(1U<<0)
#define SPI1EN		(1U<<12)
#define CR1_SSM			(1U<<9)
#define CR1_SSI			(1U<<8)
#define CR1_MSTR		(1U<<2)
#define CR1_CPOL		(1U<<1)
#define CR1_PHA			(1U<<0)
#define CR2_TXDMAEN		(1U<<1)
#define CR2_RXDMAEN		(1U<<0)
#define CR1_SPE			(1U<<6)
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/
SPI_HandleTypeDef HSPI1 =
{ .Instance = SPI1 };
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void spi1_gpio_init(void)
{
	/*Start the GPIO Clock*/
	GPIO_Clock_Init(GPIOA_CLK);

	/*Set PA5,PA6,PA7 mode to alternate function*/

	/*PA5*/
	GPIOA->MODER &= ~(1U << 10);
	GPIOA->MODER |= (1U << 11);

	/*PA6*/
	GPIOA->MODER &= ~(1U << 12);
	GPIOA->MODER |= (1U << 13);

	/*PA7*/
	GPIOA->MODER &= ~(1U << 14);
	GPIOA->MODER |= (1U << 15);

	/*Set PA5,PA6,PA7 alternate function type to SPI1*/
	/*PA5*/
	GPIOA->AFR[0] |= (1U << 20);
	GPIOA->AFR[0] &= ~(1U << 21);
	GPIOA->AFR[0] |= (1U << 22);
	GPIOA->AFR[0] &= ~(1U << 23);

	/*PA6*/
	GPIOA->AFR[0] |= (1U << 24);
	GPIOA->AFR[0] &= ~(1U << 25);
	GPIOA->AFR[0] |= (1U << 26);
	GPIOA->AFR[0] &= ~(1U << 27);

	/*PA7*/
	GPIOA->AFR[0] |= (1U << 28);
	GPIOA->AFR[0] &= ~(1U << 29);
	GPIOA->AFR[0] |= (1U << 30);
	GPIOA->AFR[0] &= ~(1U << 31);

}

StatusTypeDef SPI_Init(SPI_HandleTypeDef *hspi)
{

	spi1_gpio_init(); // Init the GPIO

	RCC->APB2ENR |= (1 << 12);  // Enable SPI1 CLock

	hspi->Instance->CR1 |= (1 << 0) | (1 << 1);   // CPOL=1, CPHA=1

	hspi->Instance->CR1 |= (1 << 2);  // Master Mode

	hspi->Instance->CR1 |=  (1 << 3); // BR[2:0] = 011: fPCLK/2, PCLK2 = 80MHz, SPI clk = 5MHz
	hspi->Instance->CR1 &= ~(1 << 4);
	hspi->Instance->CR1 &= ~(1 << 5);

	hspi->Instance->CR1 &= ~(1 << 7);  // LSBFIRST = 0, MSB first

	hspi->Instance->CR1 |= (1 << 8) | (1 << 9); // SSM=1, SSi=1 -> Software Slave Management

	hspi->Instance->CR1 &= ~(1 << 10);  // RXONLY = 0, full-duplex

	hspi->Instance->CR1 &= ~(1 << 11);  // DFF=0, 8 bit data

	hspi->Instance->CR2 = 0;

	SPI_Enable(&HSPI1);

	return DEV_OK;
}

StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *data,
		uint16_t size, uint32_t timeout)
{

	SPI_TypeDef *SPI = hspi->Instance;

	int i = 0;
	while (i < size)
	{
		while (!((SPI->SR) & (1 << 1)))
		{
		}; // wait for TXE bit to set -> This will indicate that the buffer is empty
		SPI->DR = data[i];  // load the data into the Data Register
		i++;
	}

	while (!((SPI->SR) & (1 << 1)))
	{
	}; // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI->SR) & (1 << 7)))
	{
	}; // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication

	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = SPI->DR;
	temp = SPI1->SR;
	temp = 0;

	return DEV_OK;
}

StatusTypeDef SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size,
		uint32_t timeout)
{

	SPI_TypeDef *SPI = hspi->Instance;

	while (size)
	{
		while (((SPI->SR) & (1 << 7)))
		{
		}; // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI->DR = 0;  // send dummy data
		while (!((SPI->SR) & (1 << 0)))
		{
		}; // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
		*data++ = (SPI->DR);
		size--;
	}

	return DEV_OK;
}

void SPI_Enable (SPI_HandleTypeDef *hspi)
{
	SPI_TypeDef *SPI = hspi->Instance;

	SPI->CR1 |= (1<<6);   // SPE=1, Peripheral enabled
}

void SPI_Disable (SPI_HandleTypeDef *hspi)
{
	SPI_TypeDef *SPI = hspi->Instance;

	SPI->CR1 &= ~(1<<6);   // SPE=0, Peripheral Disabled
}

#ifdef ADV_SPI
StatusTypeDef SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *p_data,
		uint16_t size, uint32_t timeout)
{
	uint32_t tickstart;
	StatusTypeDef error_code = DEV_OK;
	uint16_t tx_xfer_cnt;

	tx_xfer_cnt = size;
	tickstart = Time_Get_Tick();

	if (hspi->State != SPI_STATE_READY)
	{
		error_code = DEV_BUSY;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	if ((p_data == NULL) || (size == 0))
	{
		error_code = DEV_ERROR;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	/*Set the transaction information*/
	hspi->State = SPI_STATE_BUSY_TX;
	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->pTxBuffPtr = (uint8_t*) p_data;
	hspi->TxXferSize = size;
	hspi->TxXferCount = size;

	hspi->pRxBuffPtr = (uint8_t*) NULL;
	hspi->RxXferSize = 0;
	hspi->RxXferCount = 0;

	/*Configure communication direction*/
	if (hspi->Init.Direction == SPI_DIRECTION_1LINE)
	{
		CLEAR_BIT(hspi->Instance->CR1, SPI_CR1_SPE);

		/*Set 1 line TX*/
		SET_BIT(hspi->Instance->CR1, SPI_CR1_BIDIOE);

	}

	if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
	{
		SET_BIT(hspi->Instance->CR1, SPI_CR1_SPE);
	}

	/*Transmit data in 16 bit mode*/
	if (hspi->Init.DataSize == SPI_DATASIZE_16BIT)
	{
		if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (tx_xfer_cnt == 1))
		{
			hspi->Instance->DR = *((uint16_t*) hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint16_t);
			hspi->TxXferCount--;
		}

		while (hspi->TxXferCount > 0)
		{
			/*Check if  TXE flag to be set and then send data*/
			if (hspi->Instance->SR & (SPI_FLAG_TXE))
			{
				hspi->Instance->DR = *((uint16_t*) hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint16_t);
				hspi->TxXferCount--;
			}
			else
			{
				if ((((Time_Get_Tick() - tickstart) >= timeout)
						&& (timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;

				}
			}
		}

	}
	/*Transmit data in 8 bit mode*/
	else
	{
		if ((hspi->Init.Mode == SPI_MODE_SLAVE) || (tx_xfer_cnt == 1))
		{
			*((__IO uint8_t*) &hspi->Instance->DR) = (*hspi->pTxBuffPtr);
			hspi->pTxBuffPtr += sizeof(uint8_t);
			hspi->TxXferCount--;
		}
		while (hspi->TxXferCount > 0)
		{
			/*Check if  TXE flag to be set and then send data*/
			if (hspi->Instance->SR & (SPI_FLAG_TXE))
			{
				*((__IO uint8_t*) &hspi->Instance->DR) = (*hspi->pTxBuffPtr);
				hspi->pTxBuffPtr += sizeof(uint8_t);
				hspi->TxXferCount--;
			}
			else
			{
				if ((((Time_Get_Tick() - tickstart) >= timeout)
						&& (timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;

				}
			}
		}

	}

	if (hspi->ErrorCode != SPI_ERROR_NONE)
	{
		error_code = DEV_ERROR;
	}

	hspi->State = SPI_STATE_READY;
	return error_code;

}

StatusTypeDef SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *p_data,
		uint16_t size, uint32_t timeout)
{
	uint32_t tickstart;
	StatusTypeDef error_code = DEV_OK;

	tickstart = Time_Get_Tick();

	if (hspi->State != SPI_STATE_READY)
	{
		error_code = DEV_BUSY;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	if ((p_data == NULL) || (size == 0))
	{
		error_code = DEV_ERROR;
		hspi->State = SPI_STATE_READY;
		return error_code;
	}

	/*Set the transaction information*/
	hspi->State = SPI_STATE_BUSY_RX;
	hspi->ErrorCode = SPI_ERROR_NONE;
	hspi->pRxBuffPtr = (uint8_t*) p_data;
	hspi->RxXferSize = size;
	hspi->RxXferCount = size;

	hspi->pTxBuffPtr = (uint8_t*) NULL;
	hspi->TxXferSize = 0;
	hspi->TxXferCount = 0;

	if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
	{
		SET_BIT(hspi->Instance->CR1, SPI_CR1_SPE);
	}

	/*Receive data in 8bit mode*/
	if (hspi->Init.DataSize == SPI_DATASIZE_8BIT)
	{
		while (hspi->RxXferCount > 0)
		{
			if (hspi->Instance->SR & (SPI_FLAG_RXNE))
			{
				*((uint8_t*) hspi->pRxBuffPtr) =
						*(__IO uint8_t*) &hspi->Instance->DR;
				hspi->pRxBuffPtr += sizeof(uint8_t);
				hspi->RxXferCount--;
			}
			else
			{
				if ((((Time_Get_Tick() - tickstart) >= timeout)
						&& (timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}
	else
	{
		while (hspi->RxXferCount > 0)
		{
			if (hspi->Instance->SR & (SPI_FLAG_RXNE))
			{
				*((uint16_t*) hspi->pRxBuffPtr) = (uint16_t) hspi->Instance->DR;
				hspi->pRxBuffPtr += sizeof(uint16_t);
				hspi->RxXferCount--;
			}
			else
			{
				if ((((Time_Get_Tick() - tickstart) >= timeout)
						&& (timeout != MAX_DELAY)) || (timeout == 0))
				{
					error_code = DEV_TIMEOUT;
					hspi->State = SPI_STATE_READY;
					return error_code;
				}
			}
		}
	}
	if (hspi->ErrorCode != SPI_ERROR_NONE)
	{
		error_code = DEV_ERROR;
	}

	hspi->State = SPI_STATE_READY;
	return error_code;
}


#endif
