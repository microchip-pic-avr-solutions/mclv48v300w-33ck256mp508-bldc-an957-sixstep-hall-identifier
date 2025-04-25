// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_user_params.h
 *
 * @brief This file has definitions to be configured by the user for spinning
 * 		  motor 1 using six step control.
 *
 * Component: APPLICATION (motor control 1 - mc1)
 * Motor : To be selected from '#define MOTOR  1'
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

#ifndef __MC1_USER_PARAMS_H
#define __MC1_USER_PARAMS_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <stdint.h>
#include <math.h>

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="MACROS ">

/* Define macros for Operational Modes */
        
/*Control Loop Selection : 
                        0 = Open-loop duty control
                        1 = Closed-loop speed control using a PI controller
                        2 = Closed-loop current control using a PI controller */
#define CLOSED_LOOP 1
    
/* Define INTERNAL_OPAMP_CONFIG to use internal op-amp outputs(default), 
 * Undefine INTERNAL_OPAMP_CONFIG to use external op-amp outputs from the 
   development board;Ensure the jumper resistors are modified on DIM  */
#define INTERNAL_OPAMP_CONFIG

/*Motor Selection : 1 = Hurst DMA0204024B101(AC300022: Hurst300 or Long Hurst)
                    2 = Hurst DMB0224C10002(AC300020: Hurst075 or Short Hurst)
                    3 = ACT 24V 3-Phase Brushless DC Motor - ACT 57BLF02
                    4 = Leadshine 24V Servo Motor ELVM6020V24FH-B25-HD (200W) */
#define MOTOR  1
    
// </editor-fold> 
    
// <editor-fold defaultstate="collapsed" desc="MOTOR SELECTION HEADER FILES ">
    
#if MOTOR == 1
    #include "hurst300.h"
#elif MOTOR == 2
    #include "hurst075.h"
#elif MOTOR == 3
    #include "act02.h"
#elif MOTOR == 4
    #include "leadshine24v.h"
#else
    #include "hurst300.h" 
#endif
// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="DEFINITIONS ">    
/** Board Parameters */
/* Peak measurement voltage of the board (unit : volts)*/
#define MC1_PEAK_VOLTAGE                75.9   
/* Peak measurement voltage of the board (unit : amps)*/
#define MAX_BOARDCURRENT                22.0     
/* Nominal DC Bus Voltage required by the motor (unit : volts)*/ 
#define DC_LINK_VOLTAGE                 24.0f 

/** The SCCP1 Timer Pre-scaler Value set to 1:64 */
#define	SPEED_MEASURE_TIMER_PRESCALER     64  
    
// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif	/* end of __MC1_USER_PARAMS_H */
