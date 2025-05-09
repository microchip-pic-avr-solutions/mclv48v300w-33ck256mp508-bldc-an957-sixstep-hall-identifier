// <editor-fold defaultstate="collapsed" desc="Description/Instruction ">
/**
 * @file pwm.c
 *
 * @brief This module configures and enables the PWM Module 
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

// <editor-fold defaultstate="collapsed" desc="HEADER FILES ">
#include <xc.h>
#include <stdint.h>
#include "pwm.h"

// </editor-fold> 

// <editor-fold defaultstate="collapsed" desc="Function Declarations">
void InitPWMGenerator1 (void);
void InitPWMGenerator2 (void);
void InitPWMGenerator3 (void);
void InitDutyPWM123Generators(void);
// </editor-fold>

// <editor-fold defaultstate="expanded" desc="INTERFACE FUNCTIONS ">

/**
* <B> Function: InitPWMGenerators()    </B>
*
* @brief Function initializes the registers common for all PWM modules and 
* configures individual PWM module. The function enables the PWM module 
* and completes the boot strap charging sequence
*        
* @param none.
* @return none.
* 
* @example
* <CODE> InitPWMGenerators();     </CODE>
*
*/
void InitPWMGenerators(void)
{

    PCLKCON      = 0x0000;
    /* PWM Clock Divider Selection bits
       0b11 = 1:16 ; 0b10 = 1:8 ;0b01 = 1:4 ; 0b00 = 1:2*/
    PCLKCONbits.DIVSEL = 0;
    /* PWM Master Clock Selection bits
       0b11 = AFPLLO ; 0b10 = FPLLO ; 0b01 = AFVCO/2 ; 0b00 = FOSC */
    PCLKCONbits.MCLKSEL = 0;
    /* Lock bit: 0 = Write-protected registers and bits are unlocked   */
    PCLKCONbits.LOCK = 0;

    /* Initialize Master Phase Register */
    MPHASE       = 0x0000;
    /* Initialize Master Duty Cycle */
    MDC          = 0x0000;
    /* Initialize Master Period Register */
    MPER         = LOOPTIME_TCY;
    
    /* Initialize FREQUENCY SCALE REGISTER*/
    FSCL          = 0x0000;
    /* Initialize FREQUENCY SCALING MINIMUM PERIOD REGISTER */
    FSMINPER     = 0x0000;
    /* Initialize Linear Feedback Shift Register */
    LFSR         = 0x0000;
    /* Initialize Combinational Trigger Register Low */
    CMBTRIGL     = 0x0000;
    /* Initialize Combinational Trigger Register High */
    CMBTRIGH     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER A */
    LOGCONA     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER B*/
    LOGCONB     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER C */
    LOGCONC     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER D */
    LOGCOND     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER E*/
    LOGCONE     = 0x0000;
    /* Initialize LOGIC CONTROL REGISTER F */
    LOGCONF     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER A */
    PWMEVTA     = 0x0000;    
    /* PWM EVENT OUTPUT CONTROL REGISTER B */
    PWMEVTB     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER C */
    PWMEVTC     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER D */
    PWMEVTD     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER E */
    PWMEVTE     = 0x0000;
    /* PWM EVENT OUTPUT CONTROL REGISTER F */
    PWMEVTF     = 0x0000;
    
    /* Function call to Initialize individual PWM modules*/
    InitPWMGenerator1 ();
    InitPWMGenerator2 ();
    InitPWMGenerator3 (); 
    
    InitDutyPWM123Generators();

    /* Clearing and disabling PWM Interrupt */
    IFS4bits.PWM1IF = 0;
    IEC4bits.PWM1IE = 0;
    IPC16bits.PWM1IP = 7;
    /* Enable the PWM module after initializing generators*/
	PG2CONLbits.ON = 1;      
    PG3CONLbits.ON = 1;      
    PG1CONLbits.ON = 1;      
}
// *****************************************************************************
/**
* <B> Function: InitDutyPWM123Generators()    </B>
*
* @brief Function initialize the duty to zero by overriding and 
* reset the duty registers for Boot strap charging
*        
* @param none.
* @return none.
* 
* @example
* <CODE> InitDutyPWM123Generators();     </CODE>
*
*/
void InitDutyPWM123Generators(void)
{
    /** Set Override Data on all PWM outputs */
    /* 0b00 = State for PWM3H,L, if Override is Enabled */
    PG3IOCONLbits.OVRDAT = 0;
    /* 0b00 = State for PWM2H,L, if Override is Enabled */
    PG2IOCONLbits.OVRDAT = 0; 
    /* 0b00 = State for PWM1H,L, if Override is Enabled */
    PG1IOCONLbits.OVRDAT = 0; 
    
    /* 1 = OVRDAT<1> provides data for output on PWM3H */
    PG3IOCONLbits.OVRENH = 1; 
    /* 1 = OVRDAT<0> provides data for output on PWM3L */
    PG3IOCONLbits.OVRENL = 1; 
    
    /* 1 = OVRDAT<1> provides data for output on PWM2H */
    PG2IOCONLbits.OVRENH = 1;
    /* 1 = OVRDAT<0> provides data for output on PWM2L */
    PG2IOCONLbits.OVRENL = 1; 
    
    /* 1 = OVRDAT<1> provides data for output on PWM1H */
    PG1IOCONLbits.OVRENH = 1;  
    /* 1 = OVRDAT<0> provides data for output on PWM1L */
    PG1IOCONLbits.OVRENL = 1;  
    
    /* Set all PWM Duty to zero */
    PG3DC = 0;
    PG2DC = 0;      
    PG1DC = 0;

}

// *****************************************************************************
/**
* <B> Function: ChargeBootstrapCapacitors()    </B>
*
* @brief Function to implement Boot Strap Capacitor charging sequence
*        
* @param none.
* @return none.
* 
* @example
* <CODE> ChargeBootstrapCapacitors();     </CODE>
*
*/
void ChargeBootstarpCapacitors(void)
{
    uint16_t i = BOOTSTRAP_CHARGING_COUNTS;
    uint16_t prevStatusCAHALF = 0,currStatusCAHALF = 0;
    uint16_t k = 0;
    
    /* Enable PWMs only on PWMxL ,to charge bootstrap capacitors at the beginning
     * Hence PWMxH is over-ridden to "LOW" */
    /* 0b00 = State for PWM3H-L,PWM2H-L and PWM1H-L if Override is Enabled*/
    PG3IOCONLbits.OVRDAT = 0;  
    PG2IOCONLbits.OVRDAT = 0;  
    PG1IOCONLbits.OVRDAT = 0;  

    /* 1 = OVRDAT<1> provides data for output on PWM3H,PWM2H and PWM1H*/
    PG3IOCONLbits.OVRENH = 1;  
    PG2IOCONLbits.OVRENH = 1;  
    PG1IOCONLbits.OVRENH = 1;  

    /* 1 = OVRDAT<1> provides data for output on PWM3L,PWM2L and PWM1L*/
    PG3IOCONLbits.OVRENL = 1;  
    PG2IOCONLbits.OVRENL = 1;  
    PG1IOCONLbits.OVRENL = 1;  

    /* PDCx: PWMx GENERATOR DUTY CYCLE REGISTER
     * Initialize the PWM duty cycle for charging */
    PWM_PDC3 = LOOPTIME_TCY - (DDEADTIME/2 + 5);
    PWM_PDC2 = LOOPTIME_TCY - (DDEADTIME/2 + 5);
    PWM_PDC1 = LOOPTIME_TCY - (DDEADTIME/2 + 5);
    
    /*While loop for the Bootstrap charging time in number of PWM Half Cycles*/
    while(i)
    {
        prevStatusCAHALF = currStatusCAHALF;
        currStatusCAHALF = PG1STATbits.CAHALF;
        if (prevStatusCAHALF != currStatusCAHALF)
        {
            /*Monitoring the Bootstrap charging time in number of PWM Half Cycles*/
            if (currStatusCAHALF == 0)
            {
                i--; 
                k++;
                if(i == (BOOTSTRAP_CHARGING_COUNTS - 50))
                {
                    /* 0 = PWM generator provides data for PWM1L pin */
                    PG1IOCONLbits.OVRENL = 0;
                }
                else if(i == (BOOTSTRAP_CHARGING_COUNTS - 150))
                {
                    /* 0 = PWM generator provides data for PWM2L pin */
                    PG2IOCONLbits.OVRENL = 0;  
                }
                else if(i == (BOOTSTRAP_CHARGING_COUNTS - 250))
                {
                    /* 0 = PWM generator provides data for PWM3L pin */
                    PG3IOCONLbits.OVRENL = 0;  
                }
                if(k > 25)
                {
                    if(PG3IOCONLbits.OVRENL == 0)
                    {
                        if(PWM_PDC3 > 2)
                        {
                            PWM_PDC3 -= 2;
                        }
                        else
                        {
                           PWM_PDC3 = 0; 
                        }
                    }
                    if(PG2IOCONLbits.OVRENL == 0)
                    {
                        if(PWM_PDC2 > 2)
                        {
                            PWM_PDC2 -= 2;
                        }
                        else
                        {
                            PWM_PDC2 = 0; 
                        }
                    }
                    if(PG1IOCONLbits.OVRENL == 0)
                    {
                        if(PWM_PDC1 > 2)
                        {
                            PWM_PDC1 -= 2;
                        }
                        else
                        {
                            PWM_PDC1 = 0; 
                        }
                    }
                    k = 0;
                } 
            }
        }
    }

    /* PDCx: PWMx GENERATOR DUTY CYCLE REGISTER
     * Reset the PWM duty cycle to zero after charging */
    PWM_PDC3 = 0;
    PWM_PDC2 = 0;
    PWM_PDC1 = 0;

    /* 0 = PWM generator provides data for output on PWM3H,PWM2H and PWM1H*/
    PG3IOCONLbits.OVRENH = 0;  
    PG2IOCONLbits.OVRENH = 0;  
    PG1IOCONLbits.OVRENH = 0;  
	
}

/**
* <B> Function: InitPWMGenerator1()    </B>
*
* @brief Function to configure PWM Module # 1
*        
* @param none.
* @return none.
* 
* @example
* <CODE> InitPWMGenerator1();     </CODE>
*
*/
void InitPWMGenerator1 (void)
{

    /* Initialize PWM GENERATOR 1 CONTROL REGISTER LOW */
    PG1CONL      = 0x0000;
    /* PWM Generator 1 Enable bit : 1 = Is enabled, 0 = Is not enabled */
    /* Ensuring PWM Generator is disabled prior to configuring module */
    PG1CONLbits.ON = 0;
    /* Clock Selection bits
       0b01 = Macro uses Master clock selected by the PCLKCON.MCLKSEL bits*/
    PG1CONLbits.CLKSEL = 1;
    /* PWM Mode Selection bits
       100 = Center-Aligned PWM mode(interrupt/register update once per cycle)*/
    PG1CONLbits.MODSEL = 4;
    /* Trigger Count Select bits
       000 = PWM Generator produces 1 PWM cycle after triggered */
    PG1CONLbits.TRGCNT = 0;
    
    /* Initialize PWM GENERATOR 1 CONTROL REGISTER HIGH */
    PG1CONH      = 0x0000;
    /* Master Duty Cycle Register Select bit
       1 = Macro uses the MDC register instead of PG1DC
       0 = Macro uses the PG1DC register*/
    PG1CONHbits.MDCSEL = 0;
    /* Master Period Register Select bit
       1 = Macro uses the MPER register instead of PG1PER
       0 = Macro uses the PG1PER register */
    PG1CONHbits.MPERSEL = 1;
    /* MPHSEL: Master Phase Register Select bit
       1 = Macro uses the MPHASE register instead of PG1PHASE
       0 = Macro uses the PG1PHASE register */
    PG1CONHbits.MPHSEL = 1;
    /* Master Update Enable bit
       1 = PWM Generator broadcasts software set/clear of UPDATE status bit and 
           EOC signal to other PWM Generators
       0 = PWM Generator does not broadcast UPDATE status bit or EOC signal */
    PG1CONHbits.MSTEN = 1;
    /* PWM Buffer Update Mode Selection bits 
       000 = SOC update
       Data registers at start of next PWM cycle if UPDREQ = 1. */
    PG1CONHbits.UPDMOD = 0;
    /* PWM Generator Trigger Mode Selection bits
       0b00 = PWM Generator operates in Single Trigger mode */
    PG1CONHbits.TRGMOD = 0;
    /* Start of Cycle Selection bits
       0000 = Local EOC */
    PG1CONHbits.SOCS = 0;
    
    /* Clear PWM GENERATOR 1 STATUS REGISTER*/
    PG1STAT      = 0x0000;
    /* Initialize PWM GENERATOR 1 I/O CONTROL REGISTER LOW */
    PG1IOCONL    = 0x0000;

    /* Current Limit Mode Select bit
       0 = If PCI current limit is active, then the CLDAT<1:0> bits define 
       the PWM output levels */
    PG1IOCONLbits.CLMOD = 0;
    /* Swap PWM Signals to PWM1H and PWM1L Device Pins bit 
       0 = PWM1H/L signals are mapped to their respective pins */
    PG1IOCONLbits.SWAP = 0;
    /* User Override Enable for PWM1H Pin bit
       0 = PWM Generator provides data for the PWM1H pin*/
    PG1IOCONLbits.OVRENH = 0;
    /* User Override Enable for PWM1L Pin bit
       0 = PWM Generator provides data for the PWM1L pin*/
    PG1IOCONLbits.OVRENL = 0;
    /* Data for PWM1H/PWM1L Pins if Override is Enabled bits
       If OVERENH = 1, then OVRDAT<1> provides data for PWM1H.
       If OVERENL = 1, then OVRDAT<0> provides data for PWM1L */
    PG1IOCONLbits.OVRDAT = 0;
    /* User Output Override Synchronization Control bits
       00 = User output overrides via the OVRENL/H and OVRDAT<1:0> bits are 
       synchronized to the local PWM time base (next start of cycle)*/
    PG1IOCONLbits.OSYNC = 0;
    /* Data for PWM1H/PWM1L Pins if FLT Event is Active bits
       If Fault is active, then FLTDAT<1> provides data for PWM1H.
       If Fault is active, then FLTDAT<0> provides data for PWM1L.*/
    PG1IOCONLbits.FLTDAT = 0;
    /* Data for PWM1H/PWM1L Pins if CLMT Event is Active bits
       If current limit is active, then CLDAT<1> provides data for PWM1H.
       If current limit is active, then CLDAT<0> provides data for PWM1L.*/
    PG1IOCONLbits.CLDAT = 0;
    /* Data for PWM1H/PWM1L Pins if Feed-Forward Event is Active bits
       If feed-forward is active, then FFDAT<1> provides data for PWM1H.
       If feed-forward is active, then FFDAT<0> provides data for PWM1L.*/
    PG1IOCONLbits.FFDAT = 0;
    /* Data for PWM1H/PWM1L Pins if Debug Mode is Active and PTFRZ = 1 bits
       If Debug mode is active and PTFRZ=1,then DBDAT<1> provides PWM1H data.
       If Debug mode is active and PTFRZ=1,then DBDAT<0> provides PWM1L data. */
    PG1IOCONLbits.DBDAT = 0;
    
    /* Initialize PWM GENERATOR 1 I/O CONTROL REGISTER HIGH */    
    PG1IOCONH    = 0x0000;
    /* Time Base Capture Source Selection bits
       000 = No hardware source selected for time base capture ? software only*/
    PG1IOCONHbits.CAPSRC = 0;
    /* Dead-Time Compensation Select bit 
       0 = Dead-time compensation is controlled by PCI Sync logic */
    PG1IOCONHbits.DTCMPSEL = 0;
    /* PWM Generator Output Mode Selection bits
	   01 = PWM Generator outputs operate in Independent mode*/
    PG1IOCONHbits.PMOD = 1;
    /* PWM1H Output Port Enable bit
       1 = PWM Generator controls the PWM1H output pin
       0 = PWM Generator does not control the PWM1H output pin */
    PG1IOCONHbits.PENH = 1;
    /* PWM1L Output Port Enable bit
       1 = PWM Generator controls the PWM1L output pin
       0 = PWM Generator does not control the PWM1L output pin */
    PG1IOCONHbits.PENL = 1;
    /* PWM1H Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG1IOCONHbits.POLH = 0;
    /* PWM1L Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG1IOCONHbits.POLL = 0;
    
    /* Initialize PWM GENERATOR 1 EVENT REGISTER LOW*/
    PG1EVTL      = 0x0000;
    /* ADC Trigger 1 Post-scaler Selection bits
       00000 = 1:1 */
    PG1EVTLbits.ADTR1PS = 0;
    /* ADC Trigger 1 Source is PG1TRIGC Compare Event Enable bit
       0 = PG1TRIGC register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG1EVTLbits.ADTR1EN3  = 0;
    /* ADC Trigger 1 Source is PG1TRIGB Compare Event Enable bit
       0 = PG1TRIGB register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG1EVTLbits.ADTR1EN2 = 0;
    /* ADC Trigger 1 Source is PG1TRIGA Compare Event Enable bit
       1 = PG1TRIGA register compare event is enabled as trigger source for 
           ADC Trigger 1 */
    PG1EVTLbits.ADTR1EN1 = 1;
    /* Update Trigger Select bits
       01 = A write of the PG1DC register automatically sets the UPDATE bit*/
    PG1EVTLbits.UPDTRG = 1;
    /* PWM Generator Trigger Output Selection bits
       000 = EOC event is the PWM Generator trigger*/
    PG1EVTLbits.PGTRGSEL = 0;
    
    /* Initialize PWM GENERATOR 1 EVENT REGISTER HIGH */
    PG1EVTH      = 0x0000;
    /* FLTIEN: PCI Fault Interrupt Enable bit
       0 = Fault interrupt is disabled */
    PG1EVTHbits.FLTIEN = 1;
    /* PCI Current Limit Interrupt Enable bit
       0 = Current limit interrupt is disabled */
    PG1EVTHbits.CLIEN = 0;
    /* PCI Feed-Forward Interrupt Enable bit
       0 = Feed-forward interrupt is disabled */
    PG1EVTHbits.FFIEN = 0;
    /* PCI Sync Interrupt Enable bit
       0 = Sync interrupt is disabled */
    PG1EVTHbits.SIEN = 0;
    /* Interrupt Event Selection bits
       00 = Interrupts CPU at EOC
       01 = Interrupts CPU at TRIGA compare event
       10 = Interrupts CPU at ADC Trigger 1 event
       11 = Time base interrupts are disabled */
    PG1EVTHbits.IEVTSEL = 3;
    /* ADC Trigger 2 Source is PG1TRIGC Compare Event Enable bit
       0 = PG1TRIGC register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG1EVTHbits.ADTR2EN3 = 0;
    /* ADC Trigger 2 Source is PG1TRIGB Compare Event Enable bit
       0 = PG1TRIGB register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG1EVTHbits.ADTR2EN2 = 0;
    /* ADC Trigger 2 Source is PG1TRIGA Compare Event Enable bit
       0 = PG1TRIGA register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG1EVTHbits.ADTR2EN1 = 0;
    /* ADC Trigger 1 Offset Selection bits
       00000 = No offset */
    PG1EVTHbits.ADTR1OFS = 0;
    
#ifdef ENABLE_PWM_FAULT
    /* PWM GENERATOR 1 Fault PCI REGISTER LOW */
    PG1FPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG1FPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG1FPCILbits.TERM = 1;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG1FPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG1FPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG1FPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted */
    PG1FPCILbits.PPS = 1;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG1FPCILbits.PSS = 9;
    
    /* PWM GENERATOR 1 Fault PCI REGISTER HIGH */
    PG1FPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG1FPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG1FPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG1FPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG1FPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG1FPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG1FPCIHbits.TQSS  = 0;
#else
    /* PWM GENERATOR 1 Fault PCI REGISTER LOW */
    PG1FPCIL     = 0x0000;
    /* PWM GENERATOR 1 Fault PCI REGISTER HIGH */
    PG1FPCIH     = 0x0000;
#endif
#ifdef CYCLE_BY_CYCLE_CURRENT_LIMIT
/*PCI logic configuration for current limit cycle by cycle mode, comparator 1 
 output as PCI source*/

    /* PWM GENERATOR 1 Current Limit PCI REGISTER LOW */
    PG1CLPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG1CLPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG1CLPCILbits.TERM = 0b001;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG1CLPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG1CLPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG1CLPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted */
    PG1CLPCILbits.PPS = 0;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG1CLPCILbits.PSS = 0b11011;
    
    /* PWM GENERATOR 1 Current Limit PCI REGISTER HIGH */
    PG1CLPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG1CLPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG1CLPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG1CLPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG1CLPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG1CLPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG1CLPCIHbits.TQSS  = 0;
#else
    /* PWM GENERATOR 1 Current Limit PCI REGISTER LOW */
    PG1CLPCIL    = 0x0000;
    /* PWM GENERATOR 1 Current Limit PCI REGISTER HIGH */
    PG1CLPCIH    = 0x0000;
#endif    

    /* PWM GENERATOR 1 Feed Forward PCI REGISTER LOW */
    PG1FFPCIL    = 0x0000;
    /* PWM GENERATOR 1 Feed Forward  PCI REGISTER HIGH */
    PG1FFPCIH    = 0x0000;
    /* PWM GENERATOR 1 Sync PCI REGISTER LOW */
    PG1SPCIL     = 0x0000;
    /* PWM GENERATOR 1 Sync PCI REGISTER LOW */
    PG1SPCIH     = 0x0000;
    
    /* Initialize PWM GENERATOR 1 LEADING-EDGE BLANKING REGISTER LOW */
    PG1LEBL      = 0x0000;
    /* Initialize PWM GENERATOR 1 LEADING-EDGE BLANKING REGISTER HIGH*/
    PG1LEBH      = 0x0000;
    
    /* Initialize PWM GENERATOR 1 PHASE REGISTER */
    PG1PHASE     = 0x0000;
    /* Initialize PWM GENERATOR 1 DUTY CYCLE REGISTER */
    PG1DC        = MIN_DUTY;
    /* Initialize PWM GENERATOR 1 DUTY CYCLE ADJUSTMENT REGISTER */
    PG1DCA       = 0x0000;
    /* Initialize PWM GENERATOR 1 PERIOD REGISTER */
    PG1PER       = 0x0000;
    /* Initialize PWM GENERATOR 1 DEAD-TIME REGISTER LOW */
    PG1DTL       = DDEADTIME;
    /* Initialize PWM GENERATOR 1 DEAD-TIME REGISTER HIGH */
    PG1DTH       = DDEADTIME;

    /* Initialize PWM GENERATOR 1 TRIGGER A REGISTER */
    PG1TRIGA     = ADC_SAMPLING_POINT1;
    /* Initialize PWM GENERATOR 1 TRIGGER B REGISTER */
    PG1TRIGB     = 0x0000;
    /* Initialize PWM GENERATOR 1 TRIGGER C REGISTER */
    PG1TRIGC     = 0x0000;
    
} 

/**
* <B> Function: InitPWMGenerator2()    </B>
*
* @brief Function to configure PWM Module # 2
*        
* @param none.
* @return none.
* 
* @example
* <CODE> InitPWMGenerator2();     </CODE>
*
*/
void InitPWMGenerator2 (void)
{

    /* Initialize PWM GENERATOR 2 CONTROL REGISTER LOW */
    PG2CONL      = 0x0000;
    /* PWM Generator 2 Enable bit : 1 = Is enabled, 0 = Is not enabled */
    /* PWM Generator is disabled prior to configuring module */
    PG2CONLbits.ON = 0;
    /* Clock Selection bits
       0b01 = Macro uses Master clock selected by the PCLKCON.MCLKSEL bits*/
    PG2CONLbits.CLKSEL = 1;
    /* PWM Mode Selection bits
       100 = Center-Aligned PWM mode(interrupt/register update once per cycle)*/
    PG2CONLbits.MODSEL = 4;
    /* Trigger Count Select bits
       000 = PWM Generator produces 1 PWM cycle after triggered */
    PG2CONLbits.TRGCNT = 0;
    
    /* Initialize PWM GENERATOR 2 CONTROL REGISTER HIGH */
    PG2CONH      = 0x0000;
    /* Master Duty Cycle Register Select bit
       1 = Macro uses the MDC register instead of PG2DC
       0 = Macro uses the PG2DC register*/
    PG2CONHbits.MDCSEL = 0;
    /* Master Period Register Select bit
       1 = Macro uses the MPER register instead of PG2PER
       0 = Macro uses the PG2PER register */
    PG2CONHbits.MPERSEL = 1;
    /* MPHSEL: Master Phase Register Select bit
       1 = Macro uses the MPHASE register instead of PG2PHASE
       0 = Macro uses the PG2PHASE register */
    PG2CONHbits.MPHSEL = 1;
    /* Master Update Enable bit
       1 = PWM Generator broadcasts software set/clear of UPDATE status bit and 
           EOC signal to other PWM Generators
       0 = PWM Generator does not broadcast UPDATE status bit or EOC signal */
    PG2CONHbits.MSTEN = 0;
     /* PWM Buffer Update Mode Selection bits 
       0b010 = Slaved SOC Update Data registers at start of next cycle if a 
       master update request is received. A master update request will be 
       transmitted if MSTEN = 1 and UPDATE = 1 for the requesting PWM
       Generator.. */
	PG2CONHbits.UPDMOD = 0b010;
    /* PWM Generator Trigger Mode Selection bits
       0b00 = PWM Generator operates in Single Trigger mode */
    PG2CONHbits.TRGMOD = 0;
    /* Start of Cycle Selection bits
       0001 = PWM1 trigger o/p selected by PG1 PGTRGSEL<2:0> bits(PGxEVT<2:0>) */
    PG2CONHbits.SOCS = 1;
    
    /* Clear PWM GENERATOR 2 STATUS REGISTER*/
    PG2STAT      = 0x0000;
    /* Initialize PWM GENERATOR 2 I/O CONTROL REGISTER LOW */
    PG2IOCONL    = 0x0000;

    /* Current Limit Mode Select bit
       0 = If PCI current limit is active, then the CLDAT<1:0> bits define 
       the PWM output levels */
    PG2IOCONLbits.CLMOD = 0;
    /* Swap PWM Signals to PWM2H and PWM2L Device Pins bit 
       0 = PWM2H/L signals are mapped to their respective pins */
    PG2IOCONLbits.SWAP = 0;
    /* User Override Enable for PWM2H Pin bit
       0 = PWM Generator provides data for the PWM2H pin*/
    PG2IOCONLbits.OVRENH = 0;
    /* User Override Enable for PWM2L Pin bit
       0 = PWM Generator provides data for the PWM2L pin*/
    PG2IOCONLbits.OVRENL = 0;
    /* Data for PWM2H/PWM2L Pins if Override is Enabled bits
       If OVERENH = 1, then OVRDAT<1> provides data for PWM2H.
       If OVERENL = 1, then OVRDAT<0> provides data for PWM2L */
    PG2IOCONLbits.OVRDAT = 0;
    /* User Output Override Synchronization Control bits
       00 = User output overrides via the OVRENL/H and OVRDAT<1:0> bits are 
       synchronized to the local PWM time base (next start of cycle)*/
    PG2IOCONLbits.OSYNC = 0;
    /* Data for PWM2H/PWM2L Pins if FLT Event is Active bits
       If Fault is active, then FLTDAT<1> provides data for PWM2H.
       If Fault is active, then FLTDAT<0> provides data for PWM2L.*/
    PG2IOCONLbits.FLTDAT = 0;
    /* Data for PWM2H/PWM2L Pins if CLMT Event is Active bits
       If current limit is active, then CLDAT<1> provides data for PWM2H.
       If current limit is active, then CLDAT<0> provides data for PWM2L.*/
    PG2IOCONLbits.CLDAT = 0;
    /* Data for PWM2H/PWM2L Pins if Feed-Forward Event is Active bits
       If feed-forward is active, then FFDAT<1> provides data for PWM2H.
       If feed-forward is active, then FFDAT<0> provides data for PWM2L.*/
    PG2IOCONLbits.FFDAT = 0;
    /* Data for PWM2H/PWM2L Pins if Debug Mode is Active and PTFRZ = 1 bits
       If Debug mode is active and PTFRZ=1,then DBDAT<1> provides PWM2H data.
       If Debug mode is active and PTFRZ=1,then DBDAT<0> provides PWM2L data. */
    PG2IOCONLbits.DBDAT = 0;
    
    /* Initialize PWM GENERATOR 2 I/O CONTROL REGISTER HIGH */    
    PG2IOCONH    = 0x0000;
    /* Time Base Capture Source Selection bits
       000 = No hardware source selected for time base capture ? software only*/
    PG2IOCONHbits.CAPSRC = 0;
    /* Dead-Time Compensation Select bit 
       0 = Dead-time compensation is controlled by PCI Sync logic */
    PG2IOCONHbits.DTCMPSEL = 0;
    /* PWM Generator Output Mode Selection bits
        01 = PWM Generator outputs operate in Independent mode */
    PG2IOCONHbits.PMOD = 1;
    /* PWM2H Output Port Enable bit
       1 = PWM Generator controls the PWM2H output pin
       0 = PWM Generator does not control the PWM2H output pin */
    PG2IOCONHbits.PENH = 1;
    /* PWM2L Output Port Enable bit
       1 = PWM Generator controls the PWM2L output pin
       0 = PWM Generator does not control the PWM2L output pin */
    PG2IOCONHbits.PENL = 1;
    /* PWM2H Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG2IOCONHbits.POLH = 0;
    /* PWM2L Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG2IOCONHbits.POLL = 0;
    
    /* Initialize PWM GENERATOR 2 EVENT REGISTER LOW*/
    PG2EVTL      = 0x0000;
    /* ADC Trigger 1 Post-scaler Selection bits
       00000 = 1:1 */
    PG2EVTLbits.ADTR1PS = 0;
    /* ADC Trigger 1 Source is PG2TRIGC Compare Event Enable bit
       0 = PG2TRIGC register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG2EVTLbits.ADTR1EN3  = 0;
    /* ADC Trigger 1 Source is PG2TRIGB Compare Event Enable bit
       0 = PG2TRIGB register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG2EVTLbits.ADTR1EN2 = 0;
    /* ADC Trigger 1 Source is PG2TRIGA Compare Event Enable bit
       0 = PG2TRIGA register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG2EVTLbits.ADTR1EN1 = 0;
    /* Update Trigger Select bits
       00 = User must set the UPDATE bit manually*/
    PG2EVTLbits.UPDTRG = 0;
    /* PWM Generator Trigger Output Selection bits
       000 = EOC event is the PWM Generator trigger*/
    PG2EVTLbits.PGTRGSEL = 0;
    
    /* Initialize PWM GENERATOR 2 EVENT REGISTER HIGH */
    PG2EVTH      = 0x0000;
    /* FLTIEN: PCI Fault Interrupt Enable bit
       0 = Fault interrupt is disabled */
    PG2EVTHbits.FLTIEN = 0;
    /* PCI Current Limit Interrupt Enable bit
       0 = Current limit interrupt is disabled */
    PG2EVTHbits.CLIEN = 0;
    /* PCI Feed-Forward Interrupt Enable bit
       0 = Feed-forward interrupt is disabled */
    PG2EVTHbits.FFIEN = 0;
    /* PCI Sync Interrupt Enable bit
       0 = Sync interrupt is disabled */
    PG2EVTHbits.SIEN = 0;
    /* Interrupt Event Selection bits
       00 = Interrupts CPU at EOC
       01 = Interrupts CPU at TRIGA compare event
       10 = Interrupts CPU at ADC Trigger 1 event
       11 = Time base interrupts are disabled */
    PG2EVTHbits.IEVTSEL = 3;
    /* ADC Trigger 2 Source is PG2TRIGC Compare Event Enable bit
       0 = PG2TRIGC register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG2EVTHbits.ADTR2EN3 = 0;
    /* ADC Trigger 2 Source is PG2TRIGB Compare Event Enable bit
       0 = PG2TRIGB register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG2EVTHbits.ADTR2EN2 = 0;
    /* ADC Trigger 2 Source is PG2TRIGA Compare Event Enable bit
       0 = PG2TRIGA register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG2EVTHbits.ADTR2EN1 = 0;
    /* ADC Trigger 1 Offset Selection bits
       00000 = No offset */
    PG2EVTHbits.ADTR1OFS = 0;
    
#ifdef ENABLE_PWM_FAULT 
    /* PWM GENERATOR 1 Fault PCI REGISTER LOW */
    PG2FPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG2FPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG2FPCILbits.TERM = 1;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG2FPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG2FPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG2FPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted*/
    PG2FPCILbits.PPS = 1;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG2FPCILbits.PSS = 9;
    
    /* PWM GENERATOR 1 Fault PCI REGISTER HIGH */
    PG2FPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG2FPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG2FPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG2FPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG2FPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG2FPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG2FPCIHbits.TQSS  = 0; 
#else
    /* PWM GENERATOR 2 Fault PCI REGISTER LOW */
    PG2FPCIL     = 0x0000;
    /* PWM GENERATOR 2 Fault PCI REGISTER HIGH */
    PG2FPCIH     = 0x0000;
#endif
    
#ifdef CYCLE_BY_CYCLE_CURRENT_LIMIT
/*PCI logic configuration for current limit cycle by cycle mode, comparator 1 
 output as PCI source*/

    /* PWM GENERATOR 2 Current Limit PCI REGISTER LOW */
    PG2CLPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG2CLPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG2CLPCILbits.TERM = 0b001;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG2CLPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG2CLPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG2CLPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted */
    PG2CLPCILbits.PPS = 0;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG2CLPCILbits.PSS = 0b11011;
    
    /* PWM GENERATOR 2 Current Limit PCI REGISTER HIGH */
    PG2CLPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG2CLPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG2CLPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG2CLPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG2CLPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG2CLPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG2CLPCIHbits.TQSS  = 0;
#else
    /* PWM GENERATOR 2 Current Limit PCI REGISTER LOW */
    PG2CLPCIL    = 0x0000;
    /* PWM GENERATOR 2 Current Limit PCI REGISTER HIGH */
    PG2CLPCIH    = 0x0000;
#endif
    
    /* PWM GENERATOR 2 Feed Forward PCI REGISTER LOW */
    PG2FFPCIL    = 0x0000;
    /* PWM GENERATOR 2 Feed Forward  PCI REGISTER HIGH */
    PG2FFPCIH    = 0x0000;
    /* PWM GENERATOR 2 Sync PCI REGISTER LOW */
    PG2SPCIL     = 0x0000;
    /* PWM GENERATOR 2 Sync PCI REGISTER LOW */
    PG2SPCIH     = 0x0000;
    
    /* Initialize PWM GENERATOR 2 LEADING-EDGE BLANKING REGISTER LOW */
    PG2LEBL      = 0x0000;
    /* Initialize PWM GENERATOR 2 LEADING-EDGE BLANKING REGISTER HIGH*/
    PG2LEBH      = 0x0000;
    
    /* Initialize PWM GENERATOR 2 PHASE REGISTER */
    PG2PHASE     = 0x0000;
    /* Initialize PWM GENERATOR 2 DUTY CYCLE REGISTER */
    PG2DC        = MIN_DUTY;
    /* Initialize PWM GENERATOR 2 DUTY CYCLE ADJUSTMENT REGISTER */
    PG2DCA       = 0x0000;
    /* Initialize PWM GENERATOR 2 PERIOD REGISTER */
    PG2PER       = 0x0000;
    /* Initialize PWM GENERATOR 2 DEAD-TIME REGISTER LOW */
    PG2DTL       = DDEADTIME;
    /* Initialize PWM GENERATOR 2 DEAD-TIME REGISTER HIGH */
    PG2DTH       = DDEADTIME;

    /* Initialize PWM GENERATOR 2 TRIGGER A REGISTER */
    PG2TRIGA     = 0x0000;
    /* Initialize PWM GENERATOR 2 TRIGGER B REGISTER */
    PG2TRIGB     = 0x0000;
    /* Initialize PWM GENERATOR 2 TRIGGER C REGISTER */
    PG2TRIGC     = 0x0000;
    
}

/**
* <B> Function: InitPWMGenerator3()    </B>
*
* @brief Function to configure PWM Module # 3
*        
* @param none.
* @return none.
* 
* @example
* <CODE> InitPWMGenerator3();     </CODE>
*
*/
void InitPWMGenerator3 (void)
{

    /* Initialize PWM GENERATOR 3 CONTROL REGISTER LOW */
    PG3CONL      = 0x0000;
    /* PWM Generator 3 Enable bit : 1 = Is enabled, 0 = Is not enabled */
    /* PWM Generator is disabled prior to configuring module */
    PG3CONLbits.ON = 0;
    /* Clock Selection bits
       0b01 = Macro uses Master clock selected by the PCLKCON.MCLKSEL bits*/
    PG3CONLbits.CLKSEL = 1;
    /* PWM Mode Selection bits
       100 = Center-Aligned PWM mode(interrupt/register update once per cycle)*/
    PG3CONLbits.MODSEL = 4;
    /* Trigger Count Select bits
       000 = PWM Generator produces 1 PWM cycle after triggered */
    PG3CONLbits.TRGCNT = 0;
    
    /* Initialize PWM GENERATOR 3 CONTROL REGISTER HIGH */
    PG3CONH      = 0x0000;
    /* Master Duty Cycle Register Select bit
       1 = Macro uses the MDC register instead of PG3DC
       0 = Macro uses the PG3DC register*/
    PG3CONHbits.MDCSEL = 0;
    /* Master Period Register Select bit
       1 = Macro uses the MPER register instead of PG3PER
       0 = Macro uses the PG3PER register */
    PG3CONHbits.MPERSEL = 1;
    /* MPHSEL: Master Phase Register Select bit
       1 = Macro uses the MPHASE register instead of PG3PHASE
       0 = Macro uses the PG3PHASE register */
    PG3CONHbits.MPHSEL = 1;
    /* Master Update Enable bit
       1 = PWM Generator broadcasts software set/clear of UPDATE status bit and 
           EOC signal to other PWM Generators
       0 = PWM Generator does not broadcast UPDATE status bit or EOC signal */
    PG3CONHbits.MSTEN = 0;
    /* PWM Buffer Update Mode Selection bits 
       0b010 = Slaved SOC Update Data registers at start of next cycle if a 
       master update request is received. A master update request will be 
       transmitted if MSTEN = 1 and UPDATE = 1 for the requesting PWM
       Generator.. */
	PG3CONHbits.UPDMOD = 0b010;
    /* PWM Generator Trigger Mode Selection bits
       0b00 = PWM Generator operates in Single Trigger mode */
    PG3CONHbits.TRGMOD = 0;
    /* Start of Cycle Selection bits
       0001 = PWM1 trigger o/p selected by PG1 PGTRGSEL<2:0> bits(PGxEVT<2:0>)*/
    PG3CONHbits.SOCS = 1;
    
    /* Clear PWM GENERATOR 3 STATUS REGISTER*/
    PG3STAT      = 0x0000;
    /* Initialize PWM GENERATOR 3 I/O CONTROL REGISTER LOW */
    PG3IOCONL    = 0x0000;

    /* Current Limit Mode Select bit
       0 = If PCI current limit is active, then the CLDAT<1:0> bits define 
       the PWM output levels */
    PG3IOCONLbits.CLMOD = 0;
    /* Swap PWM Signals to PWM3H and PWM3L Device Pins bit 
       0 = PWM3H/L signals are mapped to their respective pins */
    PG3IOCONLbits.SWAP = 0;
    /* User Override Enable for PWM3H Pin bit
       0 = PWM Generator provides data for the PWM3H pin*/
    PG3IOCONLbits.OVRENH = 0;
    /* User Override Enable for PWM3L Pin bit
       0 = PWM Generator provides data for the PWM3L pin*/
    PG3IOCONLbits.OVRENL = 0;
    /* Data for PWM3H/PWM3L Pins if Override is Enabled bits
       If OVERENH = 1, then OVRDAT<1> provides data for PWM3H.
       If OVERENL = 1, then OVRDAT<0> provides data for PWM3L */
    PG3IOCONLbits.OVRDAT = 0;
    /* User Output Override Synchronization Control bits
       00 = User output overrides via the OVRENL/H and OVRDAT<1:0> bits are 
       synchronized to the local PWM time base (next start of cycle)*/
    PG3IOCONLbits.OSYNC = 0;
    /* Data for PWM3H/PWM3L Pins if FLT Event is Active bits
       If Fault is active, then FLTDAT<1> provides data for PWM3H.
       If Fault is active, then FLTDAT<0> provides data for PWM3L.*/
    PG3IOCONLbits.FLTDAT = 0;
    /* Data for PWM3H/PWM3L Pins if CLMT Event is Active bits
       If current limit is active, then CLDAT<1> provides data for PWM3H.
       If current limit is active, then CLDAT<0> provides data for PWM3L.*/
    PG3IOCONLbits.CLDAT = 0;
    /* Data for PWM3H/PWM3L Pins if Feed-Forward Event is Active bits
       If feed-forward is active, then FFDAT<1> provides data for PWM3H.
       If feed-forward is active, then FFDAT<0> provides data for PWM3L.*/
    PG3IOCONLbits.FFDAT = 0;
    /* Data for PWM3H/PWM3L Pins if Debug Mode is Active and PTFRZ = 1 bits
       If Debug mode is active and PTFRZ=1,then DBDAT<1> provides PWM3H data.
       If Debug mode is active and PTFRZ=1,then DBDAT<0> provides PWM3L data. */
    PG3IOCONLbits.DBDAT = 0;
    
    /* Initialize PWM GENERATOR 3 I/O CONTROL REGISTER HIGH */    
    PG3IOCONH    = 0x0000;
    /* Time Base Capture Source Selection bits
       000 = No hardware source selected for time base capture ? software only*/
    PG3IOCONHbits.CAPSRC = 0;
    /* Dead-Time Compensation Select bit 
       0 = Dead-time compensation is controlled by PCI Sync logic */
    PG3IOCONHbits.DTCMPSEL = 0;
    /* PWM Generator Output Mode Selection bits
       01 = PWM Generator outputs operate in Independent mode*/
    PG3IOCONHbits.PMOD = 1;
    /* PWM3H Output Port Enable bit
       1 = PWM Generator controls the PWM3H output pin
       0 = PWM Generator does not control the PWM3H output pin */
    PG3IOCONHbits.PENH = 1;
    /* PWM3L Output Port Enable bit
       1 = PWM Generator controls the PWM3L output pin
       0 = PWM Generator does not control the PWM3L output pin */
    PG3IOCONHbits.PENL = 1;
    /* PWM3H Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG3IOCONHbits.POLH = 0;
    /* PWM3L Output Polarity bit
       1 = Output pin is active-low
       0 = Output pin is active-high*/
    PG3IOCONHbits.POLL = 0;
    
    /* Initialize PWM GENERATOR 3 EVENT REGISTER LOW*/
    PG3EVTL      = 0x0000;
    /* ADC Trigger 1 Post-scaler Selection bits
       00000 = 1:1 */
    PG3EVTLbits.ADTR1PS = 0;
    /* ADC Trigger 1 Source is PG3TRIGC Compare Event Enable bit
       0 = PG3TRIGC register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG3EVTLbits.ADTR1EN3  = 0;
    /* ADC Trigger 1 Source is PG3TRIGB Compare Event Enable bit
       0 = PG3TRIGB register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG3EVTLbits.ADTR1EN2 = 0;
    /* ADC Trigger 1 Source is PG3TRIGA Compare Event Enable bit
       0 = PG3TRIGA register compare event is disabled as trigger source for 
           ADC Trigger 1 */
    PG3EVTLbits.ADTR1EN1 = 0;
    /* Update Trigger Select bits
       00 = User must set the UPDATE bit manually */
    PG3EVTLbits.UPDTRG = 0;
    /* PWM Generator Trigger Output Selection bits
       000 = EOC event is the PWM Generator trigger*/
    PG3EVTLbits.PGTRGSEL = 0;
    
    /* Initialize PWM GENERATOR 3 EVENT REGISTER HIGH */
    PG3EVTH      = 0x0000;
    /* FLTIEN: PCI Fault Interrupt Enable bit
       0 = Fault interrupt is disabled */
    PG3EVTHbits.FLTIEN = 0;
    /* PCI Current Limit Interrupt Enable bit
       0 = Current limit interrupt is disabled */
    PG3EVTHbits.CLIEN = 0;
    /* PCI Feed-Forward Interrupt Enable bit
       0 = Feed-forward interrupt is disabled */
    PG3EVTHbits.FFIEN = 0;
    /* PCI Sync Interrupt Enable bit
       0 = Sync interrupt is disabled */
    PG3EVTHbits.SIEN = 0;
    /* Interrupt Event Selection bits
       00 = Interrupts CPU at EOC
       01 = Interrupts CPU at TRIGA compare event
       10 = Interrupts CPU at ADC Trigger 1 event
       11 = Time base interrupts are disabled */
    PG3EVTHbits.IEVTSEL = 3;
    /* ADC Trigger 3 Source is PG3TRIGC Compare Event Enable bit
       0 = PG3TRIGC register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG3EVTHbits.ADTR2EN3 = 0;
    /* ADC Trigger 2 Source is PG3TRIGB Compare Event Enable bit
       0 = PG3TRIGB register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG3EVTHbits.ADTR2EN2 = 0;
    /* ADC Trigger 2 Source is PG3TRIGA Compare Event Enable bit
       0 = PG3TRIGA register compare event is disabled as 
           trigger source for ADC Trigger 2 */
    PG3EVTHbits.ADTR2EN1 = 0;
    /* ADC Trigger 1 Offset Selection bits
       00000 = No offset */
    PG3EVTHbits.ADTR1OFS = 0;
    
    
#ifdef ENABLE_PWM_FAULT
    /* PWM GENERATOR 1 Fault PCI REGISTER LOW */
    PG3FPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG3FPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG3FPCILbits.TERM = 1;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG3FPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG3FPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG3FPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted*/
    PG3FPCILbits.PPS = 1;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG3FPCILbits.PSS = 9;
    
    /* PWM GENERATOR 1 Fault PCI REGISTER HIGH */
    PG3FPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG3FPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG3FPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG3FPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG3FPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG3FPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced '1')*/
    PG3FPCIHbits.TQSS  = 0;
#else
    /* PWM GENERATOR 1 Fault PCI REGISTER LOW */
    PG3FPCIL     = 0x0000;
    /* PWM GENERATOR 1 Fault PCI REGISTER HIGH */
    PG3FPCIH     = 0x0000;
#endif
#ifdef CYCLE_BY_CYCLE_CURRENT_LIMIT
/*PCI logic configuration for current limit cycle by cycle mode, comparator 1 
 output as PCI source*/

    /* PWM GENERATOR 3 Current Limit PCI REGISTER LOW */
    PG3CLPCIL     = 0x0000;
    /* Termination Synchronization Disable bit
       1 = Termination of latched PCI occurs immediately
       0 = Termination of latched PCI occurs at PWM EOC */
    PG3CLPCILbits.TSYNCDIS = 0;
    /* Termination Event Selection bits
       001 = Auto-Terminate: Terminate when PCI source transitions from 
             active to inactive */
    PG3CLPCILbits.TERM = 0b001;
    /* Acceptance Qualifier Polarity Select bit: 0 = Not inverted 1 = Inverted*/
    PG3CLPCILbits.AQPS = 0;
    /* Acceptance Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to 0)
       110 = Selects PCI Source #9
       101 = Selects PCI Source #8
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)        
       000 = No acceptance qualifier is used (qualifier forced to 1) */
    PG3CLPCILbits.AQSS = 0;
    /* PCI Synchronization Control bit
       1 = PCI source is synchronized to PWM EOC
       0 = PCI source is not synchronized to PWM EOC*/
    PG3CLPCILbits.PSYNC = 0;
    /* PCI Polarity Select bit 0 = Not inverted 1 = Inverted */
    PG3CLPCILbits.PPS = 0;
    /* PCI Source Selection bits
       11111 = PCI Source #31
       ? ?
       00001 = PCI Source #1
       00000 = Software PCI control bit (SWPCI) only*/
    PG3CLPCILbits.PSS = 0b11011;
    
    /* PWM GENERATOR 3 Current Limit PCI REGISTER HIGH */
    PG3CLPCIH     = 0x0000;
    /* PCI Bypass Enable bit
       0 = PCI function is not bypassed */
    PG3CLPCIHbits.BPEN   = 0;
    /* PCI Bypass Source Selection bits(1)
       000 = PCI control is sourced from PG1 PCI logic when BPEN = 1 */
    PG3CLPCIHbits.BPSEL   = 0;
    /* PCI Acceptance Criteria Selection bits
       101 = Latched any edge(2)
       100 = Latched rising edge
       011 = Latched
       010 = Any edge
       001 = Rising edge
       000 = Level-sensitive*/
    PG3CLPCIHbits.ACP   = 3;
    /* PCI SR Latch Mode bit
       1 = SR latch is Reset-dominant in Latched Acceptance modes
       0 = SR latch is Set-dominant in Latched Acceptance modes*/
    PG3CLPCIHbits.PCIGT  = 0;
    /* Termination Qualifier Polarity Select bit 1 = Inverted 0 = Not inverted*/
    PG3CLPCIHbits.TQPS   = 0;
    /* Termination Qualifier Source Selection bits
       111 = SWPCI control bit only (qualifier forced to ?1?b0?)(3)
       110 = Selects PCI Source #9 (pwm_pci[9] input port)
       101 = Selects PCI Source #8 (pwm_pci[8] input port)
       100 = Selects PCI Source #1 (PWM Generator output selected by the PWMPCI<2:0> bits)
       011 = PWM Generator is triggered
       010 = LEB is active
       001 = Duty cycle is active (base PWM Generator signal)
       000 = No termination qualifier used (qualifier forced to ?1?b1?)(3)*/
    PG3CLPCIHbits.TQSS  = 0;
#else 
    /* PWM GENERATOR 3 Current Limit PCI REGISTER LOW */
    PG3CLPCIL    = 0x0000;
    /* PWM GENERATOR 3 Current Limit PCI REGISTER HIGH */
    PG3CLPCIH    = 0x0000;
#endif
    
    /* PWM GENERATOR 3 Feed Forward PCI REGISTER LOW */
    PG3FFPCIL    = 0x0000;
    /* PWM GENERATOR 3 Feed Forward  PCI REGISTER HIGH */
    PG3FFPCIH    = 0x0000;
    /* PWM GENERATOR 3 Sync PCI REGISTER LOW */
    PG3SPCIL     = 0x0000;
    /* PWM GENERATOR 3 Sync PCI REGISTER LOW */
    PG3SPCIH     = 0x0000;
    
    /* Initialize PWM GENERATOR 3 LEADING-EDGE BLANKING REGISTER LOW */
    PG3LEBL      = 0x0000;
    /* Initialize PWM GENERATOR 3 LEADING-EDGE BLANKING REGISTER HIGH*/
    PG3LEBH      = 0x0000;
    
    /* Initialize PWM GENERATOR 3 PHASE REGISTER */
    PG3PHASE     = 0x0000;
    /* Initialize PWM GENERATOR 3 DUTY CYCLE REGISTER */
    PG3DC        = MIN_DUTY;
    /* Initialize PWM GENERATOR 3 DUTY CYCLE ADJUSTMENT REGISTER */
    PG3DCA       = 0x0000;
    /* Initialize PWM GENERATOR 3 PERIOD REGISTER */
    PG3PER       = 0x0000;
    /* Initialize PWM GENERATOR 3 DEAD-TIME REGISTER LOW */
    PG3DTL       = DDEADTIME;
    /* Initialize PWM GENERATOR 3 DEAD-TIME REGISTER HIGH */
    PG3DTH       = DDEADTIME;

    /* Initialize PWM GENERATOR 3 TRIGGER A REGISTER */
    PG3TRIGA     = 0x0000;
    /* Initialize PWM GENERATOR 3 TRIGGER B REGISTER */
    PG3TRIGB     = 0x0000;
    /* Initialize PWM GENERATOR 3 TRIGGER C REGISTER */
    PG3TRIGC     = 0x0000;
    
}

// </editor-fold>