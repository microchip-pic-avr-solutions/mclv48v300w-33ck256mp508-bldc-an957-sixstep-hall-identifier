// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file port_config.c
 *
 * @brief This module initializes the GPIO pins as analog/digital,input or 
 * output etc. 
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

#include <xc.h>

#include "port_config.h"

// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

/**
* <B> Function: SetupGPIOPorts() </B>
*
* @brief Function initialize all ports as input and digital pins
*        
* @param none.
* @return none.
* 
* @example
* <CODE> SetupGPIOPorts(); </CODE>
*
*/
void SetupGPIOPorts(void)
{
    /* Reset all PORTx register (all inputs) */
    
    #ifdef TRISA
        TRISA = 0xFFFF;
        LATA  = 0x0000;
    #endif
    #ifdef ANSELA
        ANSELA = 0x0000;
    #endif

    #ifdef TRISB
        TRISB = 0xFFFF;
        LATB  = 0x0000;
    #endif
    #ifdef ANSELB
        ANSELB = 0x0000;
    #endif

    #ifdef TRISC
        TRISC = 0xFFFF;
        LATC  = 0x0000;
    #endif
    #ifdef ANSELC
        ANSELC = 0x0000;
    #endif

    #ifdef TRISD
        TRISD = 0xFFFF;
        LATD  = 0x0000;
    #endif
    #ifdef ANSELD
        ANSELD = 0x0000;
    #endif

    #ifdef TRISE
        TRISE = 0xFFFF;
        LATE  = 0x0000;
    #endif
    #ifdef ANSELE
        ANSELE = 0x0000;
    #endif

    MapGPIOHWFunction();

    return;
}

/**
* <B> Function: MapGPIOHWFunction() </B>
*
* @brief Function maps port pins as input or output, analog or digital
*        
* @param none.
* @return none.
* 
* @example
* <CODE> MapGPIOHWFunction(); </CODE>
*
*/
void MapGPIOHWFunction(void)
{
    /* Amplifier input and output pins */      
#ifdef INTERNAL_OPAMP_CONFIG
    
    /* Amplifier input and output pins */       
    /* IBUS : OA1
     * Positive Input   : DIM:029 - PIN20: OA1IN+/AN9/PMA6/RA2
     * Negative Input   : DIM:031 - PIN18: OA1IN-/ANA1/RA1
     * Amplifier Output : DIM:033 - PIN16:  OA1OUT/AN0/CMP1A/IBIAS0/RA0 */
    ANSELAbits.ANSELA1 = 1;
    TRISAbits.TRISA1 = 1;   
    ANSELAbits.ANSELA2 = 1;
    TRISAbits.TRISA2 = 1;   
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 0;        
#else
    /* External Amplifier mode - Amplifier output pins are configured as
     analog input channels
     * IBUS : DIM:033 - PIN #16: OA1OUT/AN0/CMP1A/IBIAS0/RA0 */
    ANSELAbits.ANSELA0 = 1;
    TRISAbits.TRISA0 = 1;   
#endif
    
    /* Potentiometer  input (POT1) - used as Speed Reference 
     * DIM:028 - PIN #30: AN17/ANN1/IBIAS1/RP54/PMD12/PMA12/RC6 */
    ANSELCbits.ANSELC6= 1;
    TRISCbits.TRISC6 = 1;   
    
    /*TEMPERATURE*/
	
	/* Phase Voltages (VA, VB, and VC)
	 * VA   : DIM:009 - PIN #60: PGD1/AN10/RP40/SCL1/RB8  
     * VB   : DIM:011 - PIN #61: PGC1/AN11/RP41/SDA1/RB9
     * VC   : DIM:022 - PIN #16: AN12/ANN0/RP48/RC0 */
    ANSELBbits.ANSELB8 = 1;
    TRISBbits.TRISB8 = 1;    
    ANSELBbits.ANSELB9 = 1;
    TRISBbits.TRISB9 = 1;    
    ANSELCbits.ANSELC0 = 1;
    TRISCbits.TRISC0 = 1;    
	
    /* DC Bus Voltage (VBUS) 
     * DIM:039 -  PIN #33: AN15/CMP2A/IBIAS2/RP51/PMD11/PMA11/RC3 */
    ANSELCbits.ANSELC3 = 1;
    TRISCbits.TRISC3 = 1;   
    /* Digital SIGNALS */   
	/* Inverter Control - PWM Outputs
     * PWM1L : DIM:003 - PIN #03  RP47/PWM1L/PMD6/RB15
     * PWM1H : DIM:001 - PIN #01  RP46/PWM1H/PMD5/RB14
     * PWM2L : DIM:007 - PIN #80  RP45/PWM2L/PMD4/RB13
     * PWM2H : DIM:005 - PIN #78  TDI/RP44/PWM2H/PMD3/RB12
     * PWM3L : DIM:004 - PIN #76  TCK/RP43/PWM3L/PMD2/RB11
     * PWM3H : DIM:002 - PIN #75  TMS/RP42/PWM3H/PMD1/RB10      */
    TRISBbits.TRISB14 = 0 ;          
    TRISBbits.TRISB15 = 0 ;         
    TRISBbits.TRISB12 = 0 ;          
    TRISBbits.TRISB13 = 0 ;           
    TRISBbits.TRISB11 = 0 ;          
    TRISBbits.TRISB10 = 0 ;        
    
    /* Debug LEDs */
    /* LED1 : DIM:030 - PIN #62 : RE12   */
    TRISEbits.TRISE13 = 0;           
    /* LED2 : DIM:032 - PIN #64 : RE13  */ 
    TRISEbits.TRISE12 = 0;           

    /* Push button Switches */
    /* SW1 : DIM:034 - PIN #57 : RE10   */
    TRISEbits.TRISE10 = 1;           
    /* SW2 : DIM:036 - PIN #59 : RE11  */ 
    TRISEbits.TRISE11 = 1;          
    
    /* Hall sensor input signals 
	 * HA   : DIM:066 - PIN #54: RP69/PMA15/PMCS2/RD5 
     * HB   : DIM:068 - PIN #53: RP70/PMD14/RD6
     * HC   : DIM:070 - PIN #52: RP71/PMD15/RD7 */ 
    TRISDbits.TRISD5 = 1;          
    TRISDbits.TRISD6 = 1;         
    TRISDbits.TRISD7 = 1; 
	
	 /* Configuring FLTLAT_OC_OV (DIM:040) - Pin #27 :  RP76/RD12 as PCI9, */
	_PCI9R = 76;
	
	/** Diagnostic Interface for MCLV-48V-300W.
        Re-map UART Channels to the device pins connected to the following 
        pins on the Motor Control Development Boards .
        UART_RX : DIM:054 - PIN #14 - ANN2/RP77/RD13(Input)
        UART_TX : DIM:052 - PIN #13 - RP78/PCI21/RD14 (Output)   */
    _U1RXR = 77;
    _RP78R = 0b000001;

}

/**
* <B> Function: OpampConfig() </B>
*
* @brief Function to configure and enable the Op-Amp Module
*        
* @param none.
* @return none.
* 
* @example
* <CODE> OpampConfig(); </CODE>
*
*/
void OpampConfig (void)
{
    /* Wide input range for Op Amp #1 */
    AMPCON1Hbits.NCHDIS1 = 0;    
    /* Enables Op Amp #1 */
    AMPCON1Lbits.AMPEN1 = 1;     
    /* Enables op amp modules if their respective AMPENx bits are also asserted */
    AMPCON1Lbits.AMPON = 1;      
}
 //</editor-fold> 
