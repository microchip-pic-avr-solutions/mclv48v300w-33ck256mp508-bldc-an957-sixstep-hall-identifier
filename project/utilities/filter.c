// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file filter.c
 *
 * @brief This module implements filers.
 *
 * Component: FILTER
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include "filter.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="VARIABLES ">

MCAPP_FILTER_LPF_T lowPassFilter;
MCAPP_FILTER_AVG_T movingAvgFilter;

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

/**
* <B> Function: MCAPP_LowPassFilter((input) </B>
*
* @brief Function to implement low pass filter. 
*        
* @param Pointer to the data structure containing variables for LPF.
* @return filtered output.
* 
* @example
* <CODE> MCAPP_LowPassFilter(input); </CODE>
*
*/
int16_t MCAPP_LowPassFilter (int16_t input)
{
    /* Filter input using a first order low-pass filter */
    const int16_t delta = (int16_t) (input - lowPassFilter.qFilterOutput);
    lowPassFilter.accumalator += __builtin_mulss(delta, LFP_CUTOFF_FREQUENCY);
    lowPassFilter.qFilterOutput = (int16_t) (lowPassFilter.accumalator >> 15);
    
    return lowPassFilter.qFilterOutput;
}

/**
* <B> Function: MCAPP_MovingAvgFilter(input) </B>
*
* @brief Function to implement moving average filter. 
*        
* @param Pointer to the data structure containing variables for filter.
* @return average filter output.
* 
* @example
* <CODE> MCAPP_MovingAvgFilter(input); </CODE>
*
*/
int16_t MCAPP_MovingAvgFilter (int16_t input)
{
    movingAvgFilter.accumalator += input;
    movingAvgFilter.mean = (int16_t) (movingAvgFilter.accumalator >> AVGFILTER_SCALER);
    movingAvgFilter.accumalator -= movingAvgFilter.mean;
    movingAvgFilter.average = movingAvgFilter.mean;
    
    return movingAvgFilter.average;
}
// </editor-fold> 