// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file change_notification.h
 *
 * @brief This header file lists the functions and definitions - to configure 
 * and enable SCCP1 Module and its features 
 *
 * Component: SCCP1
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

#ifndef SCCP1_H
#define	SCCP1_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">      
#include <xc.h>
#include <stdint.h>
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="TYPE DEFINITIONS ">  
        
/** SCCP1 Clock Pre-scalers */
typedef enum tagSCCP1_CLOCK_PRESCALER
{ 
    /** TMRPS<1:0>: CCPx Time Base Prescale Select bits
        0b11 = 1:64 , 0b10 = 1:16 ,0b01 = 1:4 0b00 = 1:1                     */
    SCCP1_CLOCK_PRESCALER_64    = 3,
    SCCP1_CLOCK_PRESCALER_16    = 2,
    SCCP1_CLOCK_PRESCALER_4     = 1,
    SCCP1_CLOCK_PRESCALER_1     = 0,
            
}SCCP1_CLOCK_PRESCALER_TYPE;

// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void SCCP1_Timer_Initialize(void);

/**
 * Read timer counters.
 * Summary: Read timer high and low counters.
 * @example
 * <code>
 * SCCP1_TimerDataRead();
 * </code>
 */
uint32_t SCCP1_TimerDataRead(void); 


/**
 * Set timer counters.
 * Summary: Read timer high and low counters.
 * @example
 * <code>
 * SCCP1_TimerDataRead();
 * </code>
 */
void SCCP1_TimerDataSet(uint32_t ); 


/**
 * Set Timer period register.
 * Summary: Set timer period register.
 * @example
 * <code>
 * SCCP1_SetTimerPeriod();
 * </code>
 */
void SCCP1_SetTimerPeriod(uint32_t ); 


/**
 * Sets the SCCP1 Input Clock Select bits.
 * @example
 * <code>
 * SCCP1_SetTimerPrescaler();
 * </code>
 */
void SCCP1_SetTimerPrescaler(uint16_t);


/**
 * Starts SCCP1 Timer module.
 * Summary: Starts SCCP1 Timer module.
 * @example
 * <code>
 * SCCP1_Timer_Start();
 * </code>
 */
void SCCP1_Timer_Start();
 

// </editor-fold>
#ifdef	__cplusplus
}
#endif

#endif	/* SCCP1_H */

