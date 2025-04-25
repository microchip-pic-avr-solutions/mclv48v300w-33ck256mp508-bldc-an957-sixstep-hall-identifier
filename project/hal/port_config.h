// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file port_config.h
 *
 * @brief This header file lists the functions and definitions for initializing 
 * GPIO pins as analog/digital,input or output etc. Also to PPS functionality to 
 * Re-mappable input or output pins.
 * 
 * Definitions in the file are for dsPIC33CK256MP508 MC DIM plugged onto 
 * Motor Control Development board from Microchip
 *
 * Component: PORTS
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

#ifndef _PORTCONFIG_H
#define _PORTCONFIG_H

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">

#include <xc.h>

// </editor-fold>

#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif
       
// <editor-fold defaultstate="expanded" desc="DEFINITIONS ">

/* Digital I/O definitions */
        
/*Push buttons*/
/* SW1 : DIM:034 - PIN #57 : RE10 */
#define SW1                   PORTEbits.RE10
/* SW2 : DIM:036 - PIN #59 : RE11 */ 
#define SW2                   PORTEbits.RE11
        
/* Used as START/STOP button of Motor */
#define BUTTON_START_STOP              SW1
/* Used as Direction Change button of Motor */
#define BUTTON_DIRECTION_CHANGE        SW2	

/* Debug LEDs */
/* LED1(LD2) : DIM:030 - PIN #62 : RE12*/
#define LED1                  LATEbits.LATE12
/* LED2(LD3) : DIM:032 - PIN #64 : RE13 */
#define LED2                  LATEbits.LATE13
        
/* Hall Sensor Inputs */
/* M1_HALL_A: DIM:66 : PIN54 - RP69/PMA15/PMCS2/RD5 */
#define M1_HALL_A       PORTDbits.RD5 
/* M1_HALL_A: DIM:68 : PIN53 - RP70/PMD14/RD6 */
#define M1_HALL_B       PORTDbits.RD6
/* M1_HALL_A: DIM:70 : PIN52 - RP71/PMD15/RD7 */
#define M1_HALL_C       PORTDbits.RD7
        
// </editor-fold> 

// <editor-fold defaultstate="collapsed" desc="INTERFACE FUNCTIONS ">

void SetupGPIOPorts(void);
void OpampConfig (void);
void MapGPIOHWFunction (void);

// </editor-fold>

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif

#endif      // end of __PORT_CONFIG_H
