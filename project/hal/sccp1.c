// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file change_notification.c
 *
 * @brief This module configures and enables the SCCP1 Module 
 * 
 * Definitions in this file are for dsPIC33CK256MP508
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

// <editor-fold defaultstate="collapsed" desc="Header Files ">
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "sccp1.h"
// </editor-fold> 

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">
/**
* <B> Function: SCCP1_Timer_Initialize() </B>
*
* @brief Function configures SCCP1 Module in 32bit timer mode
*        
* @param none.
* @return none.
* 
* @example
* <CODE> SCCP1_Timer_Initialize(); </CODE>
*
*/
void SCCP1_Timer_Initialize(void)
{
    /* Set SCCP1 operating OFF */
    CCP1CON1Lbits.CCSEL = 0;     
    /* Set timebase width (16-bit = 0) */
    CCP1CON1Lbits.T32 = 0;       
    /* Set mode to 16/32 bit timer mode features to Output Timer Mode */
    CCP1CON1Lbits.MOD = 0b0000;  
    /* No external synchronization; timer rolls over at FFFFh or matches with the Timer Period register */
    CCP1CON1Hbits.SYNC = 0b00000;
    /* Set timebase synchronization (Synchronized) */
    CCP1CON1Lbits.TMRSYNC = 0;   
    /* Set the clock source (Tcy) */
    CCP1CON1Lbits.CLKSEL = 0b000;
    /* Set the clock pre-scaler (1:64) */
    CCP1CON1Lbits.TMRPS = 0b00;  
    /* Set Sync/Triggered mode (Synchronous) */
    CCP1CON1Hbits.TRIGEN = 0;    
    
    /* Initialize timer prior to enable module. */
    CCP1TMRL = 0x0000;           
    /* Initialize timer prior to enable module. */
    CCP1TMRH = 0x0000;           
    
    /* Set timer period register low */
    CCP1PRL = 0xFFFF;           
    /* Set timer period register high */
    CCP1PRH = 0x0000;             
    
    /* Interrupt Priority set */
    IPC1bits.CCT1IP = 6;        
    /* Clear Interrupt flag */
    IFS0bits.CCT1IF = 0;         
    /* Disable Interrupt */
    IEC0bits.CCT1IE = 0;         
    /* Disable CCP/input capture */
    CCP1CON1Lbits.CCPON = 0;     
}

void SCCP1_SetTimerPrescaler(uint16_t timerPrescaler)
{
    if(timerPrescaler == 64)
    {
        CCP1CON1Lbits.TMRPS = SCCP1_CLOCK_PRESCALER_64;
    }
    else if(timerPrescaler == 16)
    {
        CCP1CON1Lbits.TMRPS = SCCP1_CLOCK_PRESCALER_16;
    }
    else if(timerPrescaler == 4)
    {
        CCP1CON1Lbits.TMRPS = SCCP1_CLOCK_PRESCALER_4;
    }
    else if(timerPrescaler == 1)
    {
        CCP1CON1Lbits.TMRPS = SCCP1_CLOCK_PRESCALER_1;
    }  
}

void SCCP1_SetTimerPeriod(uint32_t timerPeriod) 
{ 
    CCP1PRL = (uint16_t)(timerPeriod & 0x0000FFFF);           
    CCP1PRH = (uint16_t)(timerPeriod >> 16);    
}

void SCCP1_Timer_Start( void )
{
    /* Start the Timer */
    CCP1CON1Lbits.CCPON = true;
    
}

void SCCP1_Timer_Stop( void )
{
    /* Stop the Timer */
    CCP1CON1Lbits.CCPON = false;
}

uint32_t SCCP1_TimerDataRead(void) 
{ 
    uint32_t timervalue;
    uint32_t timerLvalue;
    uint32_t timerHvalue;
    timerLvalue = CCP1TMRL;
    timerHvalue = CCP1TMRH;
    timervalue =  (timerHvalue << 16) + timerLvalue;
    return timervalue;
}

void SCCP1_TimerDataSet(uint32_t value) 
{ 
   /* Update the counter values */
    CCP1TMRL = (uint16_t)(value & 0x0000FFFF);           
    CCP1TMRH = (uint16_t)(value >> 16); 
}
