/**
 ******************************************************************************
 * @file           : trident_gpio_f4.h
 * @author         : User
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 #client_name.
 * All rights reserved. 
 *
 ******************************************************************************
 */

#ifndef TRIDENT_GPIO_F4_H_
#define TRIDENT_GPIO_F4_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "stdio.h"
#include "stddef.h"
#include "stm32f4xx.h"
/************************************
 * MACROS AND DEFINES
 ************************************/
#define GPIO_LOW  0x00
#define GPIO_HIGH 0x01

#define GPIO_PIN_0                 0U  /* Pin 0 selected    */
#define GPIO_PIN_1                 1U  /* Pin 1 selected    */
#define GPIO_PIN_2                 2U  /* Pin 2 selected    */
#define GPIO_PIN_3                 3U  /* Pin 3 selected    */
#define GPIO_PIN_4                 4U  /* Pin 4 selected    */
#define GPIO_PIN_5                 5U  /* Pin 5 selected    */
#define GPIO_PIN_6                 6U  /* Pin 6 selected    */
#define GPIO_PIN_7                 7U  /* Pin 7 selected    */
#define GPIO_PIN_8                 8U  /* Pin 8 selected    */
#define GPIO_PIN_9                 9U  /* Pin 9 selected    */
#define GPIO_PIN_10                10U  /* Pin 10 selected   */
#define GPIO_PIN_11                11U  /* Pin 11 selected   */
#define GPIO_PIN_12                12U  /* Pin 12 selected   */
#define GPIO_PIN_13                13U  /* Pin 13 selected   */
#define GPIO_PIN_14                14U  /* Pin 14 selected   */
#define GPIO_PIN_15                15U  /* Pin 15 selected   */
#define GPIO_PIN_All               16U  /* All pins selected */

#define LCD_RST_Pin                GPIO_PIN_12
#define LCD_DC_Pin				   GPIO_PIN_1
#define SPI1_CS_Pin                GPIO_PIN_4
#define SPI1_SCK_Pin               GPIO_PIN_5
#define SPI1_MISO_Pin			   GPIO_PIN_6
#define SPI1_MOSI_Pin              GPIO_PIN_7
#define USART1_RX_Pin			   GPIO_PIN_10
#define USART1_TX_Pin              GPIO_PIN_9
#define LED_Pin                    GPIO_PIN_13
#define MCO1_Pin                   GPIO_PIN_8

#define LCD_RST_Port               GPIOA
#define LCD_DC_Port                GPIOA
#define SPI1_CS_Port               GPIOA
#define SPI1_SCK_Port              GPIOA
#define SPI1_MISO_Port			   GPIOA
#define SPI1_MOSI_Port             GPIOA
#define USART1_RX_Port			   GPIOA
#define USART1_TX_Port             GPIOA
#define LED_Port                   GPIOC
#define MCO1_Port				   GPIOA

/************************************
 * TYPEDEFS
 ************************************/
typedef enum
{

  GPIOA_CLK = 0,
  GPIOB_CLK,
  GPIOC_CLK,
  GPIOD_CLK,
  GPIOE_CLK,
  GPIOF_CLK,
  GPIOG_CLK,

}GPIO_CLK_t;

typedef enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG }GPIO_Mode_t;
typedef enum {GPIO_OUT_PUSH_PULL, GPIO_OUT_OPEN_DRAIN}GPIO_OutputType_t;
typedef enum {GPIO_OUT_SPEED_VLOW, GPIO_OUT_SPEED_LOW, GPIO_OUT_SPEED_HIGH, GPIO_OUT_SPEED_VHIGH}GPIO_OutputSpeed_t;
typedef enum {GPIO_PUPD_NONE, GPIO_PUPD_UP, GPIO_PUPD_DOWN}GPIO_PUPD_t;
/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void GPIO_Clock_Init(GPIO_CLK_t gpio_port_clk);
void GPIO_Clock_DeInit(GPIO_CLK_t gpio_port_clk);

void GPIO_PinConfig(GPIO_Mode_t mode,GPIO_TypeDef *port, uint16_t pin);
void GPIO_SetOutputType(GPIO_OutputType_t oType,GPIO_TypeDef *port, uint16_t pin);
void GPIO_SetOutputSpeed(GPIO_OutputSpeed_t speed,GPIO_TypeDef *port, uint16_t pin);
void GPIO_SetPullUpDown(GPIO_PUPD_t state,GPIO_TypeDef *port, uint16_t pin);

void GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, uint8_t val);
void GPIO_TogglePin(GPIO_TypeDef *port, uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif 
