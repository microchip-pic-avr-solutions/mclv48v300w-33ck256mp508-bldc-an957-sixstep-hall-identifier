// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file mc1_service.c
 *
 * @brief This module implements motor control.
 *
 * Component: MOTOR CONTROL APPLICATION
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

#include <libq.h>
#include "diagnostics.h"
#include "board_service.h"
#include "mc1_init.h"
#include "sixstep_control.h"
#include "mc1_user_params.h"
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="VARIABLES ">

MC1APP_DATA_T mc1;
MC1APP_DATA_T *pMC1Data = &mc1;

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="STATIC FUNCTIONS ">

static void MC1APP_StateMachine(MC1APP_DATA_T *);
static void MCAPP_MC1ReceivedDataProcess(MC1APP_DATA_T *);
static void MCAPP_HallSequenceIdentifier(MC1APP_DATA_T *);

// </editor-fold>

/**
* <B> Function: void MC1APP_StateMachine (MC1APP_DATA_T *)  </B>
*
* @brief Application state machine.
*
* @param Pointer to the data structure containing Application parameters.
* @return none.
* 
* @example
* <CODE> MC1APP_StateMachine(&mc); </CODE>
*
*/
static void MC1APP_StateMachine(MC1APP_DATA_T *pMCData)
{
    MCAPP_MEASURE_T *pMotorInputs = pMCData->pMotorInputs;
    MCAPP_CONTROL_SCHEME_T *pControlScheme = pMCData->pControlScheme;

    switch(pMCData->appState)
    {
    case MCAPP_INIT:

        HAL_MC1PWMDisableOutputs();

        /* Stop the motor */
        pMCData->runCmd = 0;       
        MCAPP_SixStepControlInit(pControlScheme);
        MCAPP_MeasureInit(pMotorInputs);      
        pMCData->appState = MCAPP_HALLSEQ_IDENT;
        break;
        
    case MCAPP_HALLSEQ_IDENT:    
        
        /* Hall sequence identifier */
        if (pMCData->hallSeqIdent.status == 0) 
        {
            MCAPP_HallSequenceIdentifier(pMCData);
            /* Check for failure in pattern identification */
            if(pMCData->hallSeqIdent.failure == 1)
            {
                pMCData->faultStatus = MCAPP_HALLSEQ_IDENT_FAILURE;
                pMCData->appState = MCAPP_FAULT;
                break;
            }
        }
        else
        {
            pMCData->appState = MCAPP_CMD_WAIT;
        }
        break;
        
    case MCAPP_CMD_WAIT:
        
        if(pMCData->runCmd == 1)
        {
            /* Function call to charge Bootstrap capacitors*/
            ChargeBootstarpCapacitors();
            pMCData->appState = MCAPP_OFFSET;
        }
       break;
       
    case MCAPP_OFFSET:
        
        /* Measure Initial Offsets */
        MCAPP_MeasureCurrentOffset(pMotorInputs);

        if(MCAPP_MeasureCurrentOffsetStatus(pMotorInputs))
        {
            /* Detect Hall initial position */
            MCAPP_HallSensorValue(&pMotorInputs->detectRotorPosition);
            HAL_MC1PWMEnableOutputs();
            pMCData->appState = MCAPP_RUN;
        }

        break;
            
    case MCAPP_RUN:
        /* Check for change direction command flag */
        if(pMCData->directionCmdFlag == 1)
        {
            /* Disable PWM outputs while motor is slowing down for change direction*/
            HAL_MC1PWMDisableOutputs();
            /* Change run direction */
            pMCData->appState = MCAPP_DIRECTION_CHANGE;
            break;
        }
        
        /* Compensate motor current offsets */
        MCAPP_MeasureCurrentCalibrate(pMotorInputs);

        MCAPP_MeasureSpeed(&pMotorInputs->detectRotorPosition);
 
        MCAPP_SixStepControlStateMachine(pControlScheme);
        
        /* Check for control scheme faults */
        if(pControlScheme->faultStatus == 1 ) 
        {
            pMCData->faultStatus = MCAPP_CONTROL_FAULT;
            pMCData->appState = MCAPP_FAULT;
            break;
        }

        if (pMCData->runCmd == 0)
        {
            /* Exit loop if motor not run */
            pMCData->appState = MCAPP_STOP;
        }
        
        break;

    case MCAPP_DIRECTION_CHANGE:
        
        /* Check if motor has stopped */
        if(pMotorInputs->detectRotorPosition.motorStopCounter == 0)
        {
            /* Change direction */
            pMCData->directionCmd = pMCData->directionCmdBuffer;
            /* Indicate direction change completed*/
            pMCData->directionCmdFlag = 0;
            /* Move state machine state to Init */
            pMCData->appState = MCAPP_INIT;
        }
        else
        {
            /* Decrement counter till motor stops running */
            pMotorInputs->detectRotorPosition.motorStopCounter--;
        }
        break;

    case MCAPP_STOP:
        HAL_MC1PWMDisableOutputs();
        pMCData->appState = MCAPP_INIT;
        
        break;
        
    case MCAPP_FAULT:
        /* Based on the application handle the fault state appropriately, 
           in this example there is no recovery from fault state */
        HAL_MC1PWMDisableOutputs();
        
        break;
        
    default:
        HAL_MC1PWMDisableOutputs();
        break;     

    } /* end of switch-case */
    
    /* Fault Handler */
    if (pMotorInputs->detectRotorPosition.hallFailure == 1 || 
                            pMotorInputs->detectRotorPosition.timerError == 1)
    {
        HAL_MC1PWMDisableOutputs();
        if(pMotorInputs->detectRotorPosition.hallFailure == 1)
        {
            pMCData->faultStatus = MCAPP_HALL_FAILURE;
        }
        else
        {
            pMCData->faultStatus = MCAPP_TIMER_ERROR;
        }
        pMCData->appState = MCAPP_FAULT;
    } 
}

/**
* <B> Function: void MCAPP_HallSequenceIdentifier (MC1APP_DATA_T *)  </B>
*
* @brief State machine for hall sequence identifier.
*
* @param Pointer to the data structure containing Application parameters.
* @return none.
* 
* @example
* <CODE> MCAPP_HallSequenceIdentifier(&mc); </CODE>
*
*/
static void MCAPP_HallSequenceIdentifier(MC1APP_DATA_T *pMCData)
{
    switch(pMCData->hallSeqIdent.state)
    {
        case MCAPP_HALLSEQ_INIT:
            /* Initialize the sequence identifier */
            MC1_DisableCNInterrupt();
            /* Setting the ADC sampling point for the identifier */
            SetADCSamplingPoint(ADC_SAMPLING_POINT2);
            /* Initialize the identifier parameters. */
            HallSeqIdentifier_Init(&pMCData->hallSeqIdent,pMCData->pControlScheme->pwmPeriod);
            pMCData->hallSeqIdent.state = MCAPP_HALLSEQ_OFFSET;
            break;
        case MCAPP_HALLSEQ_OFFSET:
            /* Measure Initial Offsets */
            MCAPP_MeasureCurrentOffset(pMCData->pMotorInputs);

            if(MCAPP_MeasureCurrentOffsetStatus(pMCData->pMotorInputs))
            {
               ChargeBootstarpCapacitors();
               pMCData->hallSeqIdent.state = MCAPP_HALLSEQ_EXECUTE;
            }
            break;
        case MCAPP_HALLSEQ_EXECUTE:
            /* Executing the identifier */
            if (pMCData->hallSeqIdent.executionFlag == 0) 
            {
                /* Compensate motor current offsets */
                MCAPP_MeasureCurrentCalibrate(pMCData->pMotorInputs);

                /* Function to execute hall sequence identifier */
                HallSeqIdentifier_Execute(&pMCData->hallSeqIdent, 
                                   pMCData->pMotorInputs->filterBusCurrent); 
            }
            else 
            { 
                pMCData->hallSeqIdent.state  = MCAPP_HALLSEQ_COMPLETE;
            }
            break;
        case MCAPP_HALLSEQ_COMPLETE:
            
            /* Load the inverter switching array */
            MCAPP_LoadInverterSwitchingArray(pMCData->hallSeqIdent.ovrDataOutPWM3, 
                    pMCData->hallSeqIdent.ovrDataOutPWM2,
                    pMCData->hallSeqIdent.ovrDataOutPWM1);
            /* Setting the ADC sampling point for the control */
            SetADCSamplingPoint(ADC_SAMPLING_POINT1);
            MC1_EnableCNInterrupt();

            /* Indicates the hall sequence identification is completed.  */
            pMCData->hallSeqIdent.status = 1; 
            break;
            
        default:
        
        break;
    }
}

/**
* <B> Function: MC1_ADC_INTERRUPT()  </B>
*
* @brief ADC interrupt vector ,and it performs following actions:
*        (1) Increments DiagnosticsStepIsr for X2C Scope 
*        (2) Reads motor 1 bus current and phase voltage
*            feedbacks from ADC data buffers.
 *       (3) Measures motor speed using SCCP timer value.
*        (3) Executes Six Step Control based on the current and speed feedbacks.
*        (4) Loads duty cycle  to the registers of PWM Generators 
*             controlling motor 1.
* 
* @param none.
* @return none.
* 
* @example none
*
*/
void __attribute__((__interrupt__,no_auto_psv)) MC1_ADC_INTERRUPT()
{
    int16_t __attribute__((__unused__)) adcBuffer;
    
    #ifdef ENABLE_DIAGNOSTICS
        DiagnosticsStepIsr();
    #endif
    
    HAL_MC1MotorInputsRead(pMC1Data->pMotorInputs);
    
    MC1APP_StateMachine(pMC1Data);
    
    HAL_PWM_DutyCycleRegister_Set(pMC1Data->pControlScheme->pwmDuty);
    
    adcBuffer = MC1_ClearADCIF_ReadADCBUF();
	MC1_ClearADCIF();
}

/**
* <B> Function: MC1_HallCN_INTERRUPT()     </B>
*
* @brief Function to service Hall signal transition and 
* read the SCCP timer value to calculate speed. 
*        
* @param none.
* @return none.
* 
* @example none
* 
*/
void __attribute__((__interrupt__,no_auto_psv)) MC1_HallCN_Interrupt()
{
    HallSensorHandler(&pMC1Data->pMotorInputs->detectRotorPosition);
    MC1_ClearCNIF(); 
}

/**
* <B> Function: _PWMInterrupt()     </B>
*
* @brief Function to handle PWM Fault Interrupt from Fault PCI
*        
* @param none.
* @return none.
* 
* @example none
* 
*/
void __attribute__((__interrupt__,no_auto_psv)) _PWMInterrupt()
{
    ClearPWMPCIFault();
    mc1.appState = MCAPP_FAULT;
    mc1.faultStatus = MCAPP_DCBUS_OV_OC_FAULT;
    ClearPWMIF(); 
}

/**
* <B> Function: void MCAPP_MC1ServiceInit (void)  </B>
*
* @brief Function to initialize the MC1 parameters
*
* @param Pointer to the data structure containing Application parameters.
* @return none.
* 
* @example
* <CODE> MCAPP_MC1ServiceInit(); </CODE>
*
*/
void MCAPP_MC1ServiceInit(void)
{
    MCAPP_MC1ParamsInit(pMC1Data);

    MC1_ClearADCIF();
    MC1_EnableADCInterrupt();
    
    HAL_MC1PWMDisableOutputs();
}

/**
* <B> Function: void MCAPP_MC1InputBufferSet (uint16_t, uint16_t)  </B>
*
* @brief Function store the run command and direction change command
*
* @param run and direction command
* @return none.
* 
* @example
* <CODE> MCAPP_MC1InputBufferSet(runCmdMC1,directionCmdMC1); </CODE>
*
*/
void MCAPP_MC1InputBufferSet(uint16_t runCmd, uint16_t directionCmd )
{ 
    pMC1Data->runCmdBuffer = runCmd;
    pMC1Data->directionCmdBuffer = directionCmd;
    
    MCAPP_MC1ReceivedDataProcess(pMC1Data);
}

/**
* <B> Function: void MCAPP_MC1ReceivedDataProcess (MC1APP_DATA_T *)  </B>
*
* @brief Function to process the received ADC and digital input data
*
* @param Pointer to the data structure containing Application parameters.
* @return none.
* 
* @example
* <CODE> MCAPP_MC1ReceivedDataProcess(&pMCData); </CODE>
*
*/
static void MCAPP_MC1ReceivedDataProcess(MC1APP_DATA_T *pMCData)
{
    MCAPP_CONTROL_SCHEME_T *pControlScheme = pMCData->pControlScheme;
    MCAPP_MEASURE_T *pMotorInputs = pMCData->pMotorInputs;
    
    /* Update the run command with run command buffer value */
    pMCData->runCmd = pMCData->runCmdBuffer;
        
    /* If there is a change direction command */
    if( pMCData->directionCmd != pMCData->directionCmdBuffer)
    {
        /* Indicates change in direction to state machine*/
        pMCData->directionCmdFlag = 1;
    }

    /* Control Input from pot */
    pControlScheme->ctrlParam.controlInput = pMotorInputs->measurePot;
}