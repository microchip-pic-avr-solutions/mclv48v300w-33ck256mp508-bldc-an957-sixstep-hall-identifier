// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file change_notification.h
 *
 * @brief This header file lists the functions and definitions - to configure 
 * and enable Change Notification Module and its features 
 *
 * Component: CHANGE NOTIFICATION
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

#ifndef CHANGE_NOTIFICATION_H
#define	CHANGE_NOTIFICATION_H

#ifdef	__cplusplus
extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">      
#include <xc.h>
#include <stdint.h>
// </editor-fold>
    
// <editor-fold defaultstate="collapsed" desc="DEFINITIONS/MACROS ">
#define MC1_HallCN_Interrupt       _CNDInterrupt  
#define MC1_EnableCNInterrupt()    _CNDIE = 1
#define MC1_DisableCNInterrupt()   _CNDIE = 0
#define MC1_ClearCNIF()            _CNDIF = 0

// </editor-fold>
    
// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
        
void CN_Configure(void);

/**
 * Set CN interrupt priority.
 * Summary: Set priority for CN interrupt.
 * @example
 * <code>
 * CN_InterrupPrioritySet();
 * </code>
 */
inline static void CN_InterrupPrioritySet(uint16_t priorityValue)
{
    _CNDIP = 0x7 & priorityValue;
}

// </editor-fold>

#ifdef	__cplusplus
}
#endif

#endif	/* CHANGE_NOTIFICATION_H */

