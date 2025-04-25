// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_init.h
 *
 * @brief This module initializes data structure holding motor control
 * parameters required to run motor 1 using field oriented control.
 * In this application to initialize variable required to run the application.
 *
 * Component: APPLICATION (Motor Control 1 - mc1)
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

#ifndef __MC1_INIT_H
#define __MC1_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <stdbool.h>

#include "measure.h"
#include "motor_types.h"
#include "sixstep_types.h"
#include "board_service.h"
#include "hall_identifier.h"
    
// </editor-fold>
   
// <editor-fold defaultstate="collapsed" desc="DEFINITIONS ">
    
#define MCAPP_CONTROL_SCHEME_T              MCAPP_BLDC_SIXSTEP_CONTROL_T
    
// </editor-fold>
    
typedef enum
{
    MCAPP_INIT = 0,                     /* Initialize Run time parameters */
    MCAPP_CMD_WAIT = 1,                 /* Wait for Run command */
    MCAPP_OFFSET = 2,                   /* Measure current offsets */
    MCAPP_RUN = 3,                      /* Run the motor */
    MCAPP_DIRECTION_CHANGE = 4,         /* Wait for load to be ready to stop */
    MCAPP_STOP = 5,                     /* Stop the motor */
    MCAPP_FAULT = 6,                    /* Motor is in Fault mode */
    MCAPP_HALLSEQ_IDENT = 7,         /* Run Hall Phase Sequence Identifier */

}MCAPP_STATE_T;

typedef enum
{
    MCAPP_HALLSEQ_INIT = 0,                     /* Initialize Run time parameters */
    MCAPP_HALLSEQ_OFFSET  = 1,                  /* Measure current offsets */ 
    MCAPP_HALLSEQ_EXECUTE = 2,      /* Execute Hall Phase Sequence Identifier */
    MCAPP_HALLSEQ_COMPLETE = 3,/* Hall Phase Sequence Identification completed */

}MCAPP_HALLSEQ_T;

typedef enum
{
    /* Hardware DC bus Over Voltage or Over Current fault */
    MCAPP_DCBUS_OV_OC_FAULT = 1,       
    MCAPP_CONTROL_FAULT = 2,            /* Control loop fault */
    MCAPP_HALL_FAILURE = 3,             /* Hall sensor feedback fault */
    MCAPP_TIMER_ERROR = 4,              /* Timer value error */
    MCAPP_HALLSEQ_IDENT_FAILURE = 5,    /* Failure in detecting Hall sequence */

}MCAPP_FAULTS_T;
    
// <editor-fold defaultstate="collapsed" desc="VARIABLE TYPE DEFINITIONS ">

typedef struct
{
    uint16_t
        appState,                   /* Application State */
        runCmd,                     /* Run command for motor */
        runCmdBuffer,               /* Run command buffer for validation */
        directionCmd,               /* Direction Change command for motor */
        directionCmdBuffer,         /* Direction Change command buffer for validation */
        directionCmdFlag,           /* Flag to indicate change direction command */
        faultStatus;                /* Fault status */
    
    MCAPP_MEASURE_T
        motorInputs;
    
    MCAPP_CONTROL_SCHEME_T
        controlScheme;              /* Motor Control parameters */
    
    MCAPP_HALLSEQ_IDENT_T
        hallSeqIdent;               /* Hall sequence identifier parameters */
    MCAPP_MEASURE_T *pMotorInputs;
    
    MCAPP_CONTROL_SCHEME_T *pControlScheme;    

}MC1APP_DATA_T;

// </editor-fold>
    
// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void MCAPP_MC1ParamsInit(MC1APP_DATA_T *);
// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif /* end of __MC1_INIT_H */
