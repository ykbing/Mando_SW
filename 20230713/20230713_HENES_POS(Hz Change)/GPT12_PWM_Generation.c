/**********************************************************************************************************************
 * \file GPT12_PWM_Generation.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "GPT12_PWM_Generation.h"
#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_GPT12_TIMER    6                       /* Define the GPT12 Timer interrupt priority            */
#define ISR_PROVIDER_GPT12_TIMER    IfxSrc_Tos_cpu0         /* Interrupt provider                                   */
#define PWM_FREQUENCY               500.0f                    /* Frequency of the PWM signal in Hz                    */
#define PWM_DUTY_CYCLE              85                      /* Duty cycle of the PWM signal in percentage           */
#define LED                         &MODULE_P00, 5          /* LED which toggles in the Interrupt Service Routine   */
#define GPT1_BLOCK_PRESCALER        32                      /* GPT1 block prescaler value                           */
#define TIMER_T3_INPUT_PRESCALER    32                      /* Timer input prescaler value                          */
#define MAX_VAL_16_BIT              10

float32 fTimer=0;
uint16 dutyUpTime=0;
uint16 dutyDownTime=0;
uint16 count=0;
uint32  g_previousCntVal= 0;
uint32 finalCntVal = 0;

uint32 counter_value(void);

/*********************************************************************************************************************/
/*--------------------------------------------Function Implementations-----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define Interrupt Service Routine */
IFX_INTERRUPT(interruptGpt12, 0, ISR_PRIORITY_GPT12_TIMER);

/* Interrupt Service Routine of the GPT12 */
void interruptGpt12(void)
{
    /* Toggle the state of the LED */
    IfxPort_togglePin(LED);
    count++;
}

/* Function to initialize the GPT12 module and the LED */
void initGpt12PWM(void)
{
    /* Enable the GPT12 module */
    IfxGpt12_enableModule(&MODULE_GPT120);
    /* Set GPT1 block prescaler to divide the module frequency */
    IfxGpt12_setGpt1BlockPrescaler(&MODULE_GPT120, IfxGpt12_Gpt1BlockPrescaler_32);
    /* Set the GPT12 timer T3 in timer mode */
    IfxGpt12_T3_setMode(&MODULE_GPT120, IfxGpt12_Mode_timer);
    /* Set the Timer T3 direction   */
    IfxGpt12_T3_setTimerDirection(&MODULE_GPT120, IfxGpt12_TimerDirection_down);
    /* Set timer T3 input prescaler to divide the timer frequency */
    IfxGpt12_T3_setTimerPrescaler(&MODULE_GPT120, IfxGpt12_TimerInputPrescaler_32);

    /* Calculate dutyUpTime and dutyDownTime for reloading timer T3 */
    float32 moduleFreq = IfxGpt12_getModuleFrequency(&MODULE_GPT120);
    fTimer = (moduleFreq / (GPT1_BLOCK_PRESCALER * TIMER_T3_INPUT_PRESCALER));
    dutyUpTime = (fTimer * (PWM_DUTY_CYCLE / 100.0f)) / PWM_FREQUENCY;
    dutyDownTime = (fTimer * (1 - (PWM_DUTY_CYCLE / 100.0f))) / PWM_FREQUENCY;

    /* Set timer T3 value */
    IfxGpt12_T3_setTimerValue(&MODULE_GPT120, dutyDownTime);

    /* Timer T2: reloads the value dutyDownTime in timer T3 */
    /* Set the timer T2 in reload mode */
    IfxGpt12_T2_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
    /* Set the T2 input parameter: Negative transition (falling edge) of T3 toggle latch T3OTL */
    IfxGpt12_T2_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_fallingEdgeTxOTL);
    /* Set timer T2 value (the value that is reloaded in T3 on negative transition) */
    IfxGpt12_T2_setTimerValue(&MODULE_GPT120, dutyDownTime);

    /* Timer T4: reloads the value dutyUpTime in timer T3 */
    /* Set timer T4 in reload mode */
    IfxGpt12_T4_setMode(&MODULE_GPT120, IfxGpt12_Mode_reload);
    /* Set the T4 input parameter: Positive transition (rising edge) of T3 toggle latch T3OTL */
    IfxGpt12_T4_setReloadInputMode(&MODULE_GPT120, IfxGpt12_ReloadInputMode_risingEdgeTxOTL);
    /* Set timer T4 value (the value that is reloaded in T3 on positive transition) */
    IfxGpt12_T4_setTimerValue(&MODULE_GPT120, dutyUpTime);

    /* Configure the GPT12 interrupt */
    volatile Ifx_SRC_SRCR *src = IfxGpt12_T3_getSrc(&MODULE_GPT120);            /* Get interrupt address            */
    IfxSrc_init(src, ISR_PROVIDER_GPT12_TIMER, ISR_PRIORITY_GPT12_TIMER);       /* Initialize the service request   */
    IfxSrc_enable(src);                                                         /* Enable GPT12 interrupt           */

    /* Initialize the LED */
    IfxPort_setPinModeOutput(LED, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
}

/* Function to start the GPT12 timer */
void runGpt12PWM(void)
{
    IfxGpt12_T3_run(&MODULE_GPT120, IfxGpt12_TimerRun_start);                   /* Start the timer T3               */
}

uint32 counter_value(void){
    uint32 currentCntVal = IfxGpt12_T3_getTimerValue(&MODULE_GPT120);

    if(count == 0)
        {
            /* If no overflow detected */
            finalCntVal = currentCntVal - g_previousCntVal; /* Total counter value calculation */
        }
        else
        {
            /* One or more overflows detected */
            /* Add to the current counter value, the amount of counter ticks which passed before the first overflow,
             * plus 65525 (0xFFFF) for each additional overflow since the previous rising edge.
             */
            finalCntVal = (uint32)(currentCntVal + (MAX_VAL_16_BIT - g_previousCntVal) + ((count - 1) * MAX_VAL_16_BIT));
        }

        /* Calculation of the PWM frequency by dividing the frequency of timer T3 through the final total counter value */
        //g_measuredPwmFreq_Hz = IfxGpt12_T3_getFrequency(&MODULE_GPT120) / finalCntVal;

        g_previousCntVal = currentCntVal;    /* Set g_previousCntVal to currentCntVal for the next calculation */
        //g_cntOverflow = 0;                   /* Reset overflow flag */

        return finalCntVal;
}

void clear_cnt(void){
    count=0;
}
