// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file hall_sensor_types.h
 *
 * @brief This module initializes data structure variable type definitions of 
 * Hall sensor structure and enumerations
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

#ifndef HALL_SENSOR_TYPES_H
#define	HALL_SENSOR_TYPES_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>
#include <stdbool.h>
  
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    uint16_t
        hall_1,              /* Hall Input a-phase */
        hall_2,              /* Hall Input b-phase */
        hall_3,              /* Hall Input c-phase */
        hallValue;          /* Hall sector value */
}MCAPP_HALL_INPUT_T;

typedef struct
{ 
    uint16_t
        timerValue,         /* SCCP Timer value on every Hall sequence change */
        period,             /* SCCP Timer value  */
        avgPeriod,          /* Moving average output of period */
        speed;              /* Measured speed */
    
    uint32_t    
        multiplier;    /* Speed Multiplier */
  
}MCAPP_CALC_SPEED_T;

typedef struct
{
    uint16_t   
        motorStopCounter,   /* Counter for motor stop */
        motorStopValue,     /* Initial value of motor stop counter */
        presentValue,       /* Present value of Hall value */
        previousValue,      /* Previous value of Hall value */
        sector,             /* Hall sector number */
        value;        /* Hall Sequence Value constructed based on Hall inputs */
        

    bool 
        hallChangeDetected, /* Hall sequence change detection flag */
        hallFailure,        /* Hall failure detection flag */
        timerError;       /* Timer failure detection flag */

    MCAPP_HALL_INPUT_T  hallInput;
    
    MCAPP_CALC_SPEED_T calculateSpeed;
        
}MCAPP_HALL_SENSOR_T;
// </editor-fold>

#ifdef	__cplusplus
}
#endif

#endif	/* HALL_SENSOR_TYPES_H */

