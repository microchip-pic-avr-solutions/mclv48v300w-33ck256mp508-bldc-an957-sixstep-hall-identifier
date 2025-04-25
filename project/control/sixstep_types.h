// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
* @file sixstep_types.h
*
* @brief This module has variable type definitions of data structure
* holding Six step control parameters and enumerated constants.
*
* Component: Six Step Control
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

#ifndef __SIXSTEP_TYPES_H
#define	__SIXSTEP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>
#include "motor_types.h"
#include "sixstep_control_types.h"
#include "motor_control_declarations.h"
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="ENUMERATED CONSTANTS ">

typedef enum
{
    CONTROL_INIT = 0,                   /* Initialize control parameters */
    CONTROL_LOOP = 1,                   /* Select control loop */
    CONTROL_OPEN_LOOP = 2,              /* Open loop duty control */
    SPEED_CONTROL_LOOP = 3,             /* Closed loop Current control */
    CURRENT_CONTROL_LOOP = 4,           /* Closed loop Speed control */
    CONTROL_FAULT = 5,                  /* Control state machine is in Fault */ 
            
}SIXSTEP_CONTROL_STATE_T;

typedef enum
{
    SPEED_CONTROL       = 1,       
    CURRENT_CONTROL     = 2,       
    OPEN_LOOP           = 3,       
            
}MCAPP_CRTL_LOOP_T;
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    uint16_t
        *pSector,           /* Pointer for Hall sector */
        *pMeasuredSpeed,    /* Pointer for Speed */
        *pDirectionCmd,     /* Pointer for direction command */
        sector,             /* Variable for Hall sector value */
        commutationSector,  /* Variable for sector to commutate */
        measuredSpeed,      /* Variable for speed */
        directionCmd,       /* Variable for direction command */
        faultStatus,        /* Variable for Fault Status */
        pwmPeriod,          /* Variable for PWM period */
        pwmDuty,            /* Variable for PWM duty */
        controlState;       /* State variable for control state machine */;
    
    int16_t
        *pAvgCurrent,       /* Pointer for average current */
        avgCurrent;         /* Variable for average current */
    
    MCAPP_MOTOR_T  motor;   /* Motor parameters */
    
    /* Parameters for PI Current controllers */ 
    MC_PIPARMIN_T   piCurrentInput;
    MC_PIPARMOUT_T  piCurrentOutput;
    
    /* Parameters for PI Speed controllers */ 
    MC_PIPARMIN_T   piSpeedInput;
    MC_PIPARMOUT_T  piSpeedOutput;
    
    MCAPP_CONTROL_T
        ctrlParam;          /* Parameters for control references */
    
}MCAPP_BLDC_SIXSTEP_CONTROL_T;

// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* end of __SIXSTEP_TYPES_H */

