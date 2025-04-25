// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file hall_sensor.h
 *
 * @brief This is the manual driver header file for the Hall sensor driver
 * 
 * Definitions in this file are for dsPIC33CK256MP508
 * 
 * Component: HALL SENSOR 
 * 
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">

/*******************************************************************************
* SOFTWARE LICENSE AGREEMENT
* 
* © [2024] Microchip Technology Inc. and its subsidiaries
* 
* Subject to your compliance with these terms, you may use this Microchip 
* software and any derivatives exclusively with Microchip products. 
* You are responsible for complying with third party license terms applicable to
* your use of third party software (including open source software) that may 
* accompany this Microchip software.
* 
* Redistribution of this Microchip software in source or binary form is allowed 
* and must include the above terms of use and the following disclaimer with the
* distribution and accompanying materials.
* 
* SOFTWARE IS "AS IS." NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL 
* MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR 
* CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO
* THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE 
* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY
* LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL
* NOT EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR THIS
* SOFTWARE
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
// </editor-fold>

#ifndef HALL_SENSOR_H
#define	HALL_SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
    
#include <xc.h>

#include <stdint.h>
#include <stdbool.h>
#include "port_config.h"
#include "sccp1.h"
#include "hall_sensor_types.h"
#include "general.h"
// </editor-fold> 
// <editor-fold defaultstate="expanded" desc="DEFINITIONS/CONSTANTS ">
    
#define SPEED_FILTER_COEFF                  Q15(0.001)

/* Enter the pins to read hall value, value = Hall_3 , Hall_2, Hall_1 */
/* Reads the value of LSB */
#define HALL_1_GetValue()         M1_HALL_A
/* Reads the value */
#define HALL_2_GetValue()         M1_HALL_B
/* Reads the value of MSB */
#define HALL_3_GetValue()         M1_HALL_C

#define HallStateChangeTimerDataSet           SCCP1_TimerDataSet
#define HallStateChangeTimerDataRead          SCCP1_TimerDataRead    
#define HallStateChangeTimerStart             SCCP1_Timer_Start  
     
// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
void MCAPP_HallSensorInit(MCAPP_HALL_SENSOR_T *);
uint16_t MCAPP_HallSensorRead(MCAPP_HALL_INPUT_T *);
void MCAPP_HallSensorValue(MCAPP_HALL_SENSOR_T *);
void MCAPP_MeasureSpeed(MCAPP_HALL_SENSOR_T *);
void HallSensorHandler(MCAPP_HALL_SENSOR_T *);
    
// </editor-fold>
#ifdef	__cplusplus
}
#endif

#endif	/* HALL_SENSOR_H */

