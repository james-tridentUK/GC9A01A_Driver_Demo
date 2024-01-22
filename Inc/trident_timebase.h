/**
 ******************************************************************************
 * @file           : trident_timebase_f4.h
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

#ifndef TRIDENT_TIMEBASE_F4_H_
#define TRIDENT_TIMEBASE_F4_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include <stdint.h>
/************************************
 * MACROS AND DEFINES
 ************************************/
#define MAX_DELAY		 0xFFFFFFFF
/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
void Time_Delay_Ms(uint32_t delay);
uint32_t Time_Get_Tick(void);
void Time_Timebase_Init(void);

void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif 
