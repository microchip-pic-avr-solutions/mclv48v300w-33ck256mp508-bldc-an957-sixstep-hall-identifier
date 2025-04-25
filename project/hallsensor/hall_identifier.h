// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * hall.h
 *
 * This header file lists data type definitions and interface functions of the
 * Hall and Phase sequence identification
 *
 * Note : It is assumed that this function  HallSeqIdentifier_Execute() 
   is called at a specific interval and bus current is used as feedback 
   for current control.  
 * Also the change notification interrupt should be disabled when this 
 * identification technique is running.
 * 
 * Component: Hall Sequence Identifier
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

#ifndef __HALL_H
#define __HALL_H

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>     // include processor files - each processor file is guarded. 
#include "general.h"
#include "board_service.h"
#include "hall_identifier_types.h"
#include "hall_sensor.h"
#include "mc1_user_params.h"
#include "motor_control_types.h"
#include "motor_control_declarations.h"

// </editor-fold>

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// <editor-fold defaultstate="expanded" desc="TYPE DEFINITIONS ">

/** PI coefficients for current control of hall sequence identifier */        
#define HALLSEQ_CURRENT_KP          Q15(0.8)
#define HALLSEQ_CURRENT_KI          Q15(0.003)
#define HALLSEQ_CURRENT_KC          Q15(0.999)
#define HALLSEQ_CURRENT_OUTMAX      Q15(0.9)
        
/** Set the CURRENT LIMIT in Amps
*
* Current limit is set in Ampere to limit the current applied to the motor 
* during the hall sequence identification process. 
* The value for current limit is set to 10% to 30% of the motor rated current. 
* 
*/
#define HALLSEQ_CURRENT_LIMIT_AMPS 0.5   
/** CURRENT LIMIT in counts */        
#define HALLSEQ_CURRENT_LIMIT_COUNT (float) ((HALLSEQ_CURRENT_LIMIT_AMPS * 32767) / MAX_BOARDCURRENT )  
                
/* Set the Voltage VECTOR COMMUTATION INTERVAL
* 
* Specify the VECTOR COMMUTATION INTERVAL in terms ADC ISR cycles (counts).
* This sets the interval between two different voltage vectors applied to the
* motor to determine hall sequence of the motor.
* This interval must be varied depending on the application and motor inertia. 
* In this code the detection function is called in the ADC interrupt which 
* occurs every 50 microseconds.
* e.g. VECTOR_COMMUTATION_INTERVAL(in seconds) = 20,000 * 50 usec = 1 second
*/
#define VECTOR_COMMUTATION_INTERVAL 20000 

/* Hall sectors */
#define HALL_SECTOR 6
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
/* Initialize identifier */
void HallSeqIdentifier_Init(MCAPP_HALLSEQ_IDENT_T*, int16_t);
/* Function to execute hall sequence identifier */
void HallSeqIdentifier_Execute(MCAPP_HALLSEQ_IDENT_T*, int16_t); 

// <editor-fold defaultstate="colapsed" desc=" VARIABLES ">

// </editor-fold>

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of __HALL_H
