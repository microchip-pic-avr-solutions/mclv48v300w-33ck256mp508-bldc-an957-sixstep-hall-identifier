// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file leadshine24v.h
 *
 * @brief This file has definitions to be configured by the user for spinning
 * motor using field oriented control.
 *
 * Motor : Leadshine 24V Servo Motor ELVM6020V24FH-B25-HD (24V 200W)
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

#ifndef __LEADSHINE24V_H
#define __LEADSHINE24V_H

#ifdef __cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <math.h>
#include <stdint.h>

#include "../mc1_user_params.h"
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="DEFINITIONS/CONSTANTS ">
    
/*Parameters of Leadshine 24V Servo Motor ELVM6020V24FH-B25-HD (24V 200W)*/
/** Motor Name Plate Parameters */
/* No.of pole pairs*/
#define POLE_PAIRS                                    5
/* Minimum motor speed at the lowest potentiometer position (unit : RPM)*/
#define MINIMUM_SPEED_RPM                             200.0f
/* Maximum motor speed at the highest potentiometer position (unit : RPM)*/
#define MAXIMUM_SPEED_RPM                             3000.0f
/* Safe speed to change direction (unit : RPM) */
#define DIRECTION_CHANGE_SPEED_RPM                    50.0f
/* Motor Rated Bus Current in RMS (unit : amps) */
#define NOMINAL_CURRENT_BUS_RMS                       5.0f

/*PI Controller Parameters*/    
/* Speed Control Loop - PI Coefficients */
#define SPEEDCNTR_PTERM                               7500
#define SPEEDCNTR_ITERM                               50
#define SPEEDCNTR_CTERM                               Q15(0.999)
#define SPEEDCNTR_OUTMAX                              Q15(0.999)
#define SPEEDCNTR_OUTMIN                              Q15(0.0)

/* Current Control Loop - PI Coefficients */
#define CURRCNTR_PTERM                               25000
#define CURRCNTR_ITERM                               90
#define CURRCNTR_CTERM                               Q15(0.999)
#define CURRCNTR_OUTMAX                              Q15(0.999)
    
/*Fault parameters*/ 
/* Overcurrent fault limit(comparator and Fault PCI) - bus current (unit : amps)*/
#define OC_FAULT_LIMIT_DCBUS                            7.0f  
    
// </editor-fold>

#ifdef __cplusplus
}
#endif

#endif	/* end of __LEADSHINE24V_H */
