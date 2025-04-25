// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file srm_control.c
 *
 * @brief This module implements control state machine.
 *
 * Component: CONTROL ALGORITHIM
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
#include "board_service.h"
#include "sixstep_control.h"

// <editor-fold defaultstate="collapsed" desc="Global Variables  ">
/* Pointers to PWM states for inverter switching arrays */
uint16_t *PWM1_STATE;
uint16_t *PWM2_STATE;
uint16_t *PWM3_STATE;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">
static void MCAPP_GetControlInputs(MCAPP_BLDC_SIXSTEP_CONTROL_T *);
static void MCAPP_PWM_Override (uint16_t );

// </editor-fold>

/**
* <B> Function: MCAPP_SixstepControlInit(MCAPP_BLDC_SIXSTEP_CONTROL_T *)  </B>
*
* @brief Function to set variables for Control.
*        
* @param 
* @return none.
 * 
* @example
* <CODE> MCAPP_SixstepControlInit(&pSixstepControl); </CODE>
*
*/
void MCAPP_SixStepControlInit(MCAPP_BLDC_SIXSTEP_CONTROL_T *pSixStepControl)
{
    pSixStepControl->piCurrentInput.inMeasure   = 0;
    pSixStepControl->piCurrentInput.inReference = 0;
    pSixStepControl->piCurrentOutput.out        = 0;
    pSixStepControl->piSpeedInput.inMeasure     = 0;
    pSixStepControl->piSpeedInput.inReference   = 0;
    pSixStepControl->piSpeedOutput.out          = 0;

    
    pSixStepControl->avgCurrent                 = 0;
    pSixStepControl->commutationSector          = 0;
    pSixStepControl->controlState               = 0;
    pSixStepControl->directionCmd               = 0;
    pSixStepControl->faultStatus                = 0;
    pSixStepControl->measuredSpeed              = 0;
    pSixStepControl->pwmDuty                    = 0;
    pSixStepControl->sector                     = 0;

    pSixStepControl->ctrlParam.targetCurrent    = 0;
    pSixStepControl->ctrlParam.targetDuty       = 0;
    pSixStepControl->ctrlParam.targetSpeed      = 0;

    pSixStepControl->controlState = CONTROL_LOOP; 
}


/**
* <B> Function: void MCAPP_GetControlInputs (MCAPP_BLDC_SIXSTEP_CONTROL_T *) </B>
*
* @brief Function read motor control inputs.
*
* @param Pointer to the data structure containing Control parameters.
* @return none.
* @example
* <CODE> MCAPP_GetControlInputs(&pControl); </CODE>
*
*/ 
static void MCAPP_GetControlInputs(MCAPP_BLDC_SIXSTEP_CONTROL_T *pControl)
{ 
    MCAPP_MOTOR_T *pMotor = &pControl->motor;
    
    /* Motor current inputs */
    pControl->sector = *(pControl->pSector);
    pControl->measuredSpeed = *(pControl->pMeasuredSpeed);
    pControl->directionCmd  = *(pControl->pDirectionCmd);
    
    if(pControl->directionCmd == 1)
    {
        pControl->commutationSector = 7 - pControl->sector;
    }
    else
    {
        pControl->commutationSector = pControl->sector;
    }
    
    if(pControl->ctrlParam.controlLoop == SPEED_CONTROL)
    {
        /* Speed Input from control input for speed control */
       pControl->ctrlParam.targetSpeed = (uint16_t)(pMotor->MinSpeed + 
               ((__builtin_muluu(pMotor->MaxSpeed - pMotor->MinSpeed,
               pControl->ctrlParam.controlInput)) >> 15));
    }
    if(pControl->ctrlParam.controlLoop == CURRENT_CONTROL)
    {
        /* Current Input from control input for current control */
        pControl->ctrlParam.targetCurrent = (int16_t)
                ((__builtin_mulsu(pMotor->qRatedCurrent,
                pControl->ctrlParam.controlInput)) >> 15);
        /* Measured filtered bus current */
        pControl->avgCurrent = *(pControl->pAvgCurrent); 
    } 
}

/**
* <B> Function: void MCAPP_SixstepControlStateMachine(MCAPP_BLDC_SIXSTEP_CONTROL_T *)  </B>
*
* @brief Executes Speed, Current and Duty Control Loops and performs actions
*        associated with Six step control 
*
* @param Pointer to the data structure containing control parameters.
* @return none.
* @example
* <CODE> MCAPP_SixstepControlStateMachine(&pControl); </CODE>
*
*/
void MCAPP_SixStepControlStateMachine(MCAPP_BLDC_SIXSTEP_CONTROL_T *pControl)
{    
    MCAPP_CONTROL_T *pCtrlParam = &pControl->ctrlParam;
    
    switch (pControl->controlState)
    {
        case CONTROL_INIT:
            MCAPP_SixStepControlInit(pControl); 
            pControl->controlState = CONTROL_LOOP;
            break;
            
        case CONTROL_LOOP:
            
            if( pCtrlParam->controlLoop == CURRENT_CONTROL )
            {
                pControl->controlState = CURRENT_CONTROL_LOOP;
            }
            else if( pCtrlParam->controlLoop == SPEED_CONTROL )
            {
                pControl->controlState = SPEED_CONTROL_LOOP;
            }
            else
                pControl->controlState = CONTROL_OPEN_LOOP;
            break;
        
        case CONTROL_OPEN_LOOP:
            MCAPP_GetControlInputs(pControl);
            MCAPP_PWM_Override(pControl->commutationSector);
            pControl->ctrlParam.targetDuty = (uint16_t)((__builtin_mulss(pControl->ctrlParam.controlInput,
                                         pControl->pwmPeriod)>>15));
            pControl->pwmDuty = pControl->ctrlParam.targetDuty;
            
            break;  
          
        case SPEED_CONTROL_LOOP:
            MCAPP_GetControlInputs(pControl);
            MCAPP_PWM_Override(pControl->commutationSector);
            /* PI control in Speed Loop */
            pControl->piSpeedInput.inReference = pControl->ctrlParam.targetSpeed;
            pControl->piSpeedInput.inMeasure   = pControl->measuredSpeed;
            MC_ControllerPIUpdate_Assembly( pControl->piSpeedInput.inReference,
                                            pControl->piSpeedInput.inMeasure,
                                           &pControl->piSpeedInput.piState,
                                           &pControl->piSpeedOutput.out);
            pControl->pwmDuty = (uint16_t) 
                    (__builtin_mulss(pControl->piSpeedOutput.out, 
                    pControl->pwmPeriod) >> 15);
          
            break;
            
        case CURRENT_CONTROL_LOOP:
            MCAPP_GetControlInputs(pControl);
            MCAPP_PWM_Override(pControl->commutationSector);
            
            /* PI control in Current Loop */
            pControl->piCurrentInput.inReference = pControl->ctrlParam.targetCurrent;
            pControl->piCurrentInput.inMeasure   = pControl->avgCurrent;
            MC_ControllerPIUpdate_Assembly( pControl->piCurrentInput.inReference,
                                            pControl->piCurrentInput.inMeasure,
                                           &pControl->piCurrentInput.piState,
                                           &pControl->piCurrentOutput.out);
            pControl->pwmDuty = (uint16_t) 
                    (__builtin_mulss(pControl->piCurrentOutput.out, 
                    pControl->pwmPeriod) >> 15);
            
            break;

        case CONTROL_FAULT:
                    
            break;
        
        default:
            pControl->controlState = CONTROL_FAULT;
            break;

    } /* End Of switch - case */
}

 /**
* <B> Function: void MCAPP_PWM_Override (uint16_t )  </B>
*
* @brief Function to override PWM outputs.
*
* @param Commutation sector.
* @return none.
* @example
* <CODE> MCAPP_PWM_Override(sector); </CODE>
*
*/
static void MCAPP_PWM_Override(uint16_t sector)
{
   PWM3_OverrideEnableDataSet(PWM3_STATE[sector]); 
   PWM2_OverrideEnableDataSet(PWM2_STATE[sector]); 
   PWM1_OverrideEnableDataSet(PWM1_STATE[sector]); 
}

/**
* <B> Function: void MCAPP_LoadInverterSwitchingArray (uint16_t * , uint16_t *, uint16_t *)  </B>
*
* @brief Function to load switching arrays for BLDC inverter control.
*
* @param switching arrays for PWM1,PWM2, and PWM3.
* @return none.
* @example
* <CODE> MCAPP_LoadInverterSwitchingArray(sector); </CODE>
*
*/
void MCAPP_LoadInverterSwitchingArray(uint16_t *value3, uint16_t *value2, uint16_t *value1)
{
    PWM3_STATE = value3;
    PWM2_STATE = value2;
    PWM1_STATE = value1;
}