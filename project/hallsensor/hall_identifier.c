// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file hall.c
 *
 * @brief This module can be used for detecting hall pattern
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>
#include "hall_identifier.h"

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="CONSTANTS">

/* The values 0x1000,0x2000 and 0x3000 are written to the PWM PGxIOCONL register
*      0x1000 corresponds to Override enable to PWMxL with override data = 0b00
*      0x2000 corresponds to Override enable to PWMxH with override data = 0b00
*      0x3000 corresponds to Override enable to PWMxL and PWMxH 
*                                                     with override data = 0b00
* Voltage vector vs electrical angle in degrees
* Voltage vector sequence applied to the motor :
*                                   [ 100  ,  110 ,  010 ,  011 , 001 , 101 ]              
* Rotor position electrical (degrees) :        
*                                   [  0   ,  60  ,  120 ,  180 , 240 , 300 ]
* 
*/
/* Three-phase vector (as in Space Vector Modulation) applied to the motor 
                                   to determine the hall sequence pattern */
const uint16_t SVMvector3[6]	=	{0x2000,0x2000,0x2000,0x1000,0x1000,0x1000};
const uint16_t SVMvector2[6]	=	{0x2000,0x1000,0x1000,0x1000,0x2000,0x2000};
const uint16_t SVMvector1[6]	=	{0x1000,0x1000,0x2000,0x2000,0x2000,0x1000};

/*  Two-phase voltage vector typically applied to the motor 
                                           for trapezoidal control  */
const uint16_t bldcVector3[6]	=	{0x2000,0x3000,0x1000,0x1000,0x3000,0x2000};
const uint16_t bldcVector2[6]	=	{0x1000,0x1000,0x3000,0x2000,0x2000,0x3000};
const uint16_t bldcVector1[6]	=	{0x3000,0x2000,0x2000,0x3000,0x1000,0x1000};


// </editor-fold>

/**
* <B> Function: HallSeqIdentifier_Init(MCAPP_HALLSEQ_IDENT_T*, int16_t) </B>
*
* @brief Function initializes the parameters of hall sequence identifier
*        .
* @param Pointer to the data structure containing parameters of 
         the hall sequence identifier.
* @param PWM period.
* @return none.
* @example
* <CODE> HallSeqIdentifier_Init(&hallSeqIdentifier, pwmPeriod); </CODE>
*
*/
void HallSeqIdentifier_Init(MCAPP_HALLSEQ_IDENT_T* pData, int16_t pwmPeriod)
{
    pData->piInputCurrent.piState.kp         = HALLSEQ_CURRENT_KP;
    pData->piInputCurrent.piState.ki         = HALLSEQ_CURRENT_KI;
    pData->piInputCurrent.piState.kc         = HALLSEQ_CURRENT_KC;
    pData->piInputCurrent.piState.outMax     = HALLSEQ_CURRENT_OUTMAX;
    pData->piInputCurrent.piState.outMin     = 0;
    pData->piInputCurrent.piState.integrator = 0;
    pData->piOutputCurrent.out               = 0;
    
    pData->pwmPeriod                         = pwmPeriod;
    /* Status to state algorithm is running*/
    pData->status = 0;
    /* Flag to indicate whether the algorithm is currently running */
    pData->executionFlag = 0;
}

/**
* <B> Function: HallSeqIdentifier_Execute(MCAPP_HALLSEQ_IDENT_T*, int16_t) </B>
*
* @brief Function to execute the hall sequence identifier. 
*        .
* @param Pointer to the data structure containing parameters of 
         the hall sequence identifier. 
* @param Measured bus current feedback.
* @return none.
* @example
* <CODE> HallSeqIdentifier_Execute(&hallSeqIdentifier, Ibus); </CODE>
*
*/
void HallSeqIdentifier_Execute(MCAPP_HALLSEQ_IDENT_T* pData, int16_t Ibus)
{            
    if(pData->vector < HALL_SECTOR)
    {
        /* Current Control based on bus current feedback. For limiting 
         the current to the motor winding during the hall sequence identification*/
        pData->piInputCurrent.inReference = (int16_t) HALLSEQ_CURRENT_LIMIT_COUNT; 
        pData->piInputCurrent.inMeasure = Ibus;
        MC_ControllerPIUpdate_Assembly(pData->piInputCurrent.inReference,
                                       pData->piInputCurrent.inMeasure,
                                       &pData->piInputCurrent.piState,
                                       &pData->piOutputCurrent.out);
        
        /* Compute duty cycle */
        pData->dutyCycle = (int16_t) (__builtin_mulss(pData->piOutputCurrent.out,
                                                    pData->pwmPeriod) >> 15);

        /* Load the duty cycle */
        HAL_PWM_DutyCycleRegister_Set(pData->dutyCycle);

        /* Load the voltage vector to corresponding PWM registers of each phase 
           of three phase inverter */      
        PWM3_OverrideEnableDataSet(SVMvector3[pData->vector]);
        PWM2_OverrideEnableDataSet(SVMvector2[pData->vector]);
        PWM1_OverrideEnableDataSet(SVMvector1[pData->vector]); 
        
        /* Increment the interval counter */
        pData->intervalCount++;  
        
        if ( pData->intervalCount > VECTOR_COMMUTATION_INTERVAL) 
        {
            /* Reading the Hall sensor value from the input port */
            pData->hallSector = MCAPP_HallSensorRead(&pData->hallInput);
            /* Present Hall value */
            pData->presentValue = pData->hallSector;
            /* Checking for change in  Hall sector */
            if(pData->presentValue != pData->previousValue)
            {
                pData->previousValue = pData->presentValue;
            }
            else
            {
                /* Failure in hall sequence detection */
                pData->failure = 1;
            }
            /* Loading the hall sensor values into an array for reference */
            pData->sectorSequence[pData->hallSector] = pData->hallSector; 
            
            /* Load the PWM override data based on the hall sequence for 
               trapezoidal commutation. */
            pData->ovrDataOutPWM3[pData->hallSector] = bldcVector3[pData->vector];
            pData->ovrDataOutPWM2[pData->hallSector] = bldcVector2[pData->vector];
            pData->ovrDataOutPWM1[pData->hallSector] = bldcVector1[pData->vector];
            
            /* Increment Vector index for next voltage vector. */
            pData->vector++; 
            
            pData->intervalCount = 0;
        }
    }
    else
    {
        pData->vector = 0;
        /* Indicates the execution is completed.  */
        pData->executionFlag = 1; 
        /* Disable PWM outputs. */
        HAL_MC1PWMDisableOutputs(); 
    }

}

