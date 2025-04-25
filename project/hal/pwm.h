// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file pwm.h
 *
 * @brief This header file lists the functions and definitions to configure the 
 * PWM Module
 * 
 * Definitions in the file are for dsPIC33CK256MP508 MC DIM plugged onto 
 * Motor Control Development board from Microchip
 *
 * Component: PWM
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

#ifndef _PWM_H
#define _PWM_H

#ifdef __cplusplus  // Provide C++ Compatability
    extern "C" {
#endif

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <xc.h>
#include <stdint.h>
        
#include "clock.h"
        
// </editor-fold> 
        
// <editor-fold defaultstate="collapsed" desc="DEFINITIONS/MACROS ">

/* Define to enable PWM Fault PCI*/         
#define ENABLE_PWM_FAULT
/* Define to enable PWM Current Limit PCI*/ 
#define CYCLE_BY_CYCLE_CURRENT_LIMIT


/* MC PWM MODULE Related Definitions  */      
#define PWM_PDC1      PG1DC
#define PWM_PDC2      PG2DC
#define PWM_PDC3      PG3DC
        
        
/* Specify bootstrap charging time in Seconds (mention at least 10mSecs) */
#define BOOTSTRAP_CHARGING_TIME_SECS 0.01
  
/* Calculate Bootstrap charging time in number of PWM Half Cycles */
#define BOOTSTRAP_CHARGING_COUNTS (uint16_t)((BOOTSTRAP_CHARGING_TIME_SECS/LOOPTIME_SEC )* 2)
        

#define PWM_FAULT_STATUS        PG1STATbits.FLTACT
        
#define _PWMInterrupt           _PWM1Interrupt
#define ClearPWMIF()            _PWM1IF = 0 
#define EnablePWMIF()           _PWM1IE = 1
#define DisablePWMIF()          _PWM1IE = 0
 
/*Specify PWM Switching Frequency in Hertz*/
#define PWMFREQUENCY_HZ         20000
/* Specify PWM module dead time in micro seconds*/
#define DEADTIME_MICROSEC       1.0
/* Loop Time in seconds, (1/ PWMFREQUENCY_HZ) */
#define LOOPTIME_SEC            0.00005
/* Loop Time in micro seconds*/
#define LOOPTIME_MICROSEC       50       
#define DDEADTIME               (uint16_t)(DEADTIME_MICROSEC*FOSC_MHZ)
/*Loop time in terms of PWM clock period*/
#define LOOPTIME_TCY            (uint16_t)(((LOOPTIME_MICROSEC*FOSC_MHZ)/2)-1)

/*Specify ADC Triggering Point w.r.t PWM Output for sensing Analog Inputs*/ 
#define ADC_SAMPLING_POINT1      0x89C2 /* (32767 + 2499) */
#define ADC_SAMPLING_POINT2      0x09C3 /* (2499) */
        
#define MIN_DUTY            0
#define MAX_DUTY            MPER-1 
// </editor-fold>      

    
// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

void InitPWMGenerators(void); 
void ChargeBootstarpCapacitors(void);
        
// </editor-fold>

#ifdef __cplusplus  // Provide C++ Compatibility
    }
#endif
#endif      // end of PWM_H
