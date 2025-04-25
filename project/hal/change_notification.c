// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file change_notification.c
 *
 * @brief This module configures and enables the Change Notification Module 
 * 
 * Definitions in this file are for dsPIC33CK256MP508
 *
 * Component: CHANGE NOTIFICATION
 *
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Disclaimer ">

/*******************************************************************************
* SOFTWARE LICENSE AGREEMENT
* 
* � [2024] Microchip Technology Inc. and its subsidiaries
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

// <editor-fold defaultstate="collapsed" desc="Header Files ">
#include <xc.h>
#include <stdint.h>

#include "change_notification.h"
// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
/**
* <B> Function: CN_Configure() </B>
*
* @brief Function configures Change Notification.
*        
* @param none.
* @return none.
* 
* @example
* <CODE> CN_Configure(); </CODE>
*
*/
void CN_Configure(void)
{
    CNCOND = 0;
/*  ON: Change Notification (CN) Control for PORTx On bit
    1 = CN is enabled
    0 = CN is disabled   */
    CNCONDbits.ON = 1;
/*    CNSTYLE: Change Notification Style Selection bit
    1 = Edge style (detects edge transitions, bits are used for a CNE)
    0 = Mismatch style (detects change from last port read event)       */    
    CNCONDbits.CNSTYLE = 0;
     
    CNEN0D = 0;
    CNEN0Dbits.CNEN0D5 = 1;
    CNEN0Dbits.CNEN0D6 = 1;
    CNEN0Dbits.CNEN0D7 = 1;

    _CNDIF = 0;
    _CNDIE = 0;
    _CNDIP = 0;
}