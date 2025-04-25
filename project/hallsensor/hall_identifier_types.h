// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file hall_identifier_types.h
 *
 * @brief This module initializes data structure variable type definitions of 
 * Hall identifier structure and enumerations
 * 
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

#ifndef HALL_IDENTIFIER_TYPES_H
#define	HALL_IDENTIFIER_TYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>
#include <stdbool.h>
#include "hall_sensor_types.h"  
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    uint16_t
        state,      /* Identifier state */
        pwmPeriod,  /* Variable for PWM period */
        dutyCycle,  /* Duty cycle */
        vector,     /* SVM vector index to fetch switching sequence */
        hallSector, /* Hall sector */
        presentValue,       /* Present value of Hall value */
        previousValue,      /* Previous value of Hall value */
        intervalCount,     /* Interval counter */
        sectorSequence[7], /* Array to store the Hall sector sequence */
        /* PWM override data obtained from the identified hall sequence for the motor */
        ovrDataOutPWM3[7],
        ovrDataOutPWM2[7],
        ovrDataOutPWM1[7];        
    
    bool
        status, /* status of hall sequence identifier */ 
        /* Flag to indicate whether the algorithm is currently running. */
        executionFlag,
        failure; /* to indicate failure in sequence identification */
    
    MC_PIPARMIN_T      piInputCurrent; 
    MC_PIPARMOUT_T     piOutputCurrent;
    
    MCAPP_HALL_INPUT_T  hallInput;
            
}MCAPP_HALLSEQ_IDENT_T;

// </editor-fold>

#ifdef	__cplusplus
}
#endif

#endif	/* HALL_SENSOR_TYPES_H */

