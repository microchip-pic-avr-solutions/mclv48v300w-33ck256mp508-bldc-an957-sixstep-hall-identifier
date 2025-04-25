// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * hall_sensor.c
 *
 * This file includes subroutine to configure and read Hall value.
 * 
 * Definitions in this file are for dsPIC33CK256MP508.
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

// <editor-fold defaultstate="collapsed" desc="Header Files ">

#include <xc.h>
#include <stdint.h>
#include <math.h>
#include "hall_sensor.h"
#include "mc1_user_params.h"
#include "filter.h"
// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

/**
* <B> Function: MCAPP_HallSensorInit(&pHallsensor) </B>
*
* @brief Function to initialize Hall sensor 
*        
* @param Pointer to the data structure containing hall sensor parameters.
* @return none.
* 
* @example
* <CODE> MCAPP_HallSensorInit(&pHallsensor); </CODE>
*
*/
void MCAPP_HallSensorInit(MCAPP_HALL_SENSOR_T *pHallsensor)
{
    pHallsensor->calculateSpeed.speed       = 0;
    pHallsensor->calculateSpeed.period      = 0;
    pHallsensor->calculateSpeed.timerValue  = 0;
    pHallsensor->hallChangeDetected         = 0;
    pHallsensor->value                      = 0;
    pHallsensor->presentValue               = 0;
    pHallsensor->previousValue              = 0;
}

/**
* <B> Function: MCAPP_HallSensorRead(&pHallInput) </B>
*
* @brief Function to read hall sensor value.
*        
* @param Pointer to the data structure containing hall input parameters.
* @return hall sector value.
* 
* @example
* <CODE> MCAPP_HallSensorRead(&pHallInput); </CODE>
*
*/
uint16_t MCAPP_HallSensorRead(MCAPP_HALL_INPUT_T *pHallInput)
{
    /* Read Hall sensor inputs */
    pHallInput->hall_1 = HALL_1_GetValue();
    pHallInput->hall_2 = HALL_2_GetValue();
    pHallInput->hall_3 = HALL_3_GetValue();
    
    pHallInput->hallValue = (pHallInput->hall_3 << 2)|
            (pHallInput->hall_2 << 1)|(pHallInput->hall_1);
    
    return pHallInput->hallValue;
}

/**
* <B> Function: MCAPP_HallSensorValue(&pHallSensor) </B>
*
* @brief Function to read hall sensor value.
*        
* @param none.
* @return none.
* 
* @example
* <CODE> MCAPP_HallSensorValue(&pHallSensor); </CODE>
*
*/
void MCAPP_HallSensorValue(MCAPP_HALL_SENSOR_T *pHallSensor)
{
    pHallSensor->presentValue = MCAPP_HallSensorRead(&pHallSensor->hallInput);
    
    pHallSensor->value = pHallSensor->presentValue;
    if(pHallSensor->presentValue != pHallSensor->previousValue)
    {
        pHallSensor->value = pHallSensor->presentValue;
        pHallSensor->previousValue = pHallSensor->value;
        
        if(pHallSensor->hallChangeDetected == 0)
        {
            pHallSensor->hallChangeDetected = 1;
        }
        else
        {
            pHallSensor->hallChangeDetected = 0;
        }
    }
    else
    {
       pHallSensor->value = pHallSensor->value;
       pHallSensor->previousValue = pHallSensor->previousValue;
       pHallSensor->hallChangeDetected = 0;
    }
}

/**
* <B> Function: MCAPP_MeasureSpeed(&pHallSensor) </B>
*
* @brief Function to calculate speed using hall sensor feedback.
*        
* @param none.
* @return none.
* 
* @example
* <CODE> MCAPP_MeasureSpeed(&pHallSensor); </CODE>
*
*/ 
void MCAPP_MeasureSpeed(MCAPP_HALL_SENSOR_T *pHallSensor)
{   
    MCAPP_CALC_SPEED_T *pCalculateSpeed = &pHallSensor->calculateSpeed;
    
    /* Calculating Moving Average of Period */
    pCalculateSpeed->avgPeriod = MCAPP_MovingAvgFilter(pCalculateSpeed->period);
    /* Calculating Speed using the period*/
    if(pCalculateSpeed->avgPeriod != 0)
    {
        pCalculateSpeed->speed = __builtin_divud(pCalculateSpeed->multiplier,pCalculateSpeed->avgPeriod);
    }
}

void HallSensorHandler(MCAPP_HALL_SENSOR_T *pHallSensor)
{
    MCAPP_CALC_SPEED_T *pCalculateSpeed = &pHallSensor->calculateSpeed;
    
    /* Starting the timer*/
    HallStateChangeTimerStart();
    /* Update the Hall pattern */
    MCAPP_HallSensorValue(pHallSensor);
    /* 
     * Check if the Hall change is detected to avoid spurious 
       change notification interrupts are detected.
     */
    if(pHallSensor->hallChangeDetected == 1)
    {
        /* Store the SCCP Timer Count */
        pCalculateSpeed->timerValue = HallStateChangeTimerDataRead();
        /* Clear the SCCP Timer */
        HallStateChangeTimerDataSet(0);
        /* Buffer for Period */
        pCalculateSpeed->period =  pCalculateSpeed->timerValue;
        
        /* Incorrect timer value */
        if(pCalculateSpeed->timerValue == 0)
        {
            pHallSensor->timerError = 1;
        }
        /*  Hall malfunction detection: check if the hall state is 
          0 or 7 and enable Hall failure flag */
        if((pHallSensor->value > 0)&&(pHallSensor->value < 7))
        {
            pHallSensor->hallFailure = 0;
            pHallSensor->sector = pHallSensor->value ;
        }
        else
        {
            pHallSensor->hallFailure = 1;
        }
            
        /*  Set the motor stop counter value */
        pHallSensor->motorStopCounter = pHallSensor->motorStopValue;
        /* Hall Change detection flag is cleared */
        pHallSensor->hallChangeDetected = 0;
    }
}