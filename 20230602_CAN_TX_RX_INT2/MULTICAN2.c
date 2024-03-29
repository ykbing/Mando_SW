/********************************************************************************************************************
 * \file MULTICAN.c
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
#include "MULTICAN2.h"

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
AppMulticanType   g_multican1;                               /* Global MULTICAN configuration and control structure   */
AppMulticanType   g_multican2;                               /* Global MULTICAN configuration and control structure   */
AppLedType        g_led;                                    /* Global LED configuration and control structure        */

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/

/* Macro to define Interrupt Service Routine.
 * This macro:
 * - defines linker section as .intvec_tc<vector number>_<interrupt priority>.
 * - defines compiler specific attribute for the interrupt functions.
 * - defines the Interrupt service routine as ISR function.
 *
 * IFX_INTERRUPT(isr, vectabNum, priority)
 *  - isr: Name of the ISR function.
 *  - vectabNum: Vector table number.
 *  - priority: Interrupt priority. Refer Usage of Interrupt Macro for more details.
 */
IFX_INTERRUPT(canIsrTxHandler1, 0, ISR_PRIORITY_CAN_TX1);
IFX_INTERRUPT(canIsrRxHandler2, 0, ISR_PRIORITY_CAN_RX2);

/* Interrupt Service Routine (ISR) called once the TX interrupt has been generated.
 * Turns on the LED1 to indicate successful CAN message transmission.
 */
void canIsrTxHandler1(void)
{
    /* Just to indicate that the CAN message has been transmitted by turning on LED1 */
    //IfxPort_setPinLow(g_led.led1.port, g_led.led1.pinIndex);

   blinkLED1();
}

/* Interrupt Service Routine (ISR) called once the RX interrupt has been generated.
 * Compares the content of the received CAN message with the content of the transmitted CAN message
 * and in case of success, turns on the LED2 to indicate successful CAN message reception.
 */
void canIsrRxHandler2(void)
{
    IfxMultican_Status readStatus;

    /* Read the received CAN message and store the status of the operation */
    readStatus = IfxMultican_Can_MsgObj_readMessage(&g_multican2.canDstMsgObj, &g_multican2.rxMsg);

    /* If no new data has been received, report an error */
    if( !( readStatus & IfxMultican_Status_newData ) )
    {
        while(1)
        {
        }
    }

    /* If new data has been received but with one message lost, report an error */
    if( readStatus == IfxMultican_Status_newDataButOneLost )
    {
        while(1)
        {
        }
    }

    /* Finally, check if the received data matches with the transmitted one */
    if( ( g_multican2.rxMsg.data[0] == g_multican2.txMsg.data[0] ) &&
        ( g_multican2.rxMsg.data[1] == g_multican2.txMsg.data[1] ) &&
        ( g_multican2.rxMsg.id == g_multican2.txMsg.id ) )
    {
        /* Turn on the LED2 to indicate correctness of the received message */
      //  IfxPort_setPinLow(g_led.led2.port, g_led.led2.pinIndex);
    }

    blinkLED2();
}

/* Function to initialize MULTICAN module, nodes and message objects related for this application use case */
//void initMultican1(void)
//{
//    /* ==========================================================================================
//     * CAN module configuration and initialization:
//     * ==========================================================================================
//     *  - load default CAN module configuration into configuration structure
//     *
//     *  - define the interrupt priority for both interrupt node pointers used in the example
//     *
//     *  - initialize CAN module with the modified configuration
//     * ==========================================================================================
//     */
//    IfxMultican_Can_initModuleConfig(&g_multican1.canConfig, &MODULE_CAN);
//
//    g_multican1.canConfig.nodePointer[TX_INTERRUPT_SRC_ID1].priority = ISR_PRIORITY_CAN_TX1;
//
//    IfxMultican_Can_initModule(&g_multican1.can, &g_multican1.canConfig);
//
//    /* ==========================================================================================
//     * Source CAN node configuration and initialization:
//     * ==========================================================================================
//     *  - load default CAN node configuration into configuration structure
//     *
//     *  - set source(TX) CAN node
//     *  - assign source CAN node to CAN node 0
//     *
//     *  - initialize the source CAN node with the modified configuration
//     * ==========================================================================================
//     */
//    IfxMultican_Can_Node_initConfig(&g_multican1.canNodeConfig, &g_multican1.can);
//
//    //g_multican1.canNodeConfig.loopBackMode = TRUE;
//    g_multican1.canNodeConfig.baudrate = 500000UL;
//    g_multican1.canNodeConfig.nodeId = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);                         // CAN�� TX Node ID ���� 0������ ������
//    g_multican1.canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                          // RX�� ����
//    g_multican1.canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
//    g_multican1.canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                          // TX�� ����
//
//    IfxMultican_Can_Node_init(&g_multican1.canSrcNode, &g_multican1.canNodeConfig);                              // TX configuration ����
//
//    /* ==========================================================================================
//     * Destination CAN node configuration and initialization:
//     * ==========================================================================================
//     *  - load default CAN node configuration into configuration structure
//     *
//     *  - set destination(RX) CAN node
//     *  - assign destination CAN node to CAN node 1
//     *
//     *  - initialize the destination CAN node with the modified configuration
//     * ==========================================================================================
//     */
//    IfxMultican_Can_Node_initConfig(&g_multican1.canNodeConfig, &g_multican1.can);
//
//    g_multican1.canNodeConfig.baudrate = 500000UL;                                                              // CAN �ӵ� ���� 500kbps
//    g_multican1.canNodeConfig.nodeId    = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);                      // CAN�� RX Node ID ���� 0������ ������
//    g_multican1.canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                          // RX�� ����
//    g_multican1.canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
//    g_multican1.canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                          // TX�� ����
//
//
//    IfxMultican_Can_Node_init(&g_multican1.canDstNode, &g_multican1.canNodeConfig);                              // RX configuration ����
//
//
//    /* ==========================================================================================
//     * Destination message object configuration and initialization:
//     * ==========================================================================================
//     *  - load default CAN message object configuration into configuration structure
//     *
//     *  - define the message object ID (different than the ID used for source MO)
//     *  - define the CAN message ID used during arbitration phase (same as ID used for source MO)
//     *  - define the message object as a receive message object
//     *  - enable interrupt generation in case of CAN message transmission
//     *  - define interrupt node pointer to be used (different than the one used for source MO)
//     *
//     *  - initialize the destination CAN message object with the modified configuration
//     * ==========================================================================================
//     */
//
//    IfxMultican_Can_MsgObj_initConfig(&g_multican1.canMsgObjConfig, &g_multican1.canDstNode);
//
//    g_multican1.canMsgObjConfig.msgObjId = DST_MESSAGE_OBJECT_ID1;
//    g_multican1.canMsgObjConfig.messageId = CAN_MESSAGE_ID_RX1;
//    g_multican1.canMsgObjConfig.frame = IfxMultican_Frame_receive;
//    g_multican1.canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
//   // g_multican1.canMsgObjConfig.rxInterrupt.enabled = TRUE;
//   // g_multican1.canMsgObjConfig.rxInterrupt.srcId = RX_INTERRUPT_SRC_ID1;
//
//    IfxMultican_Can_MsgObj_init(&g_multican1.canDstMsgObj, &g_multican1.canMsgObjConfig);
//
//    /* ==========================================================================================
//       * Source message object configuration and initialization:
//       * ==========================================================================================
//       *  - load default CAN message object configuration into configuration structure
//       *
//       *  - define the message object ID
//       *  - define the CAN message ID used during arbitration phase
//       *  - define the message object as a transmit message object
//       *  - enable interrupt generation in case of CAN message transmission
//       *  - define interrupt node pointer to be used
//       *
//       *  - initialize the source CAN message object with the modified configuration
//       * ==========================================================================================
//       */
//      IfxMultican_Can_MsgObj_initConfig(&g_multican1.canMsgObjConfig, &g_multican1.canSrcNode);
//
//      g_multican1.canMsgObjConfig.msgObjId            = SRC_MESSAGE_OBJECT_ID1;
//      g_multican1.canMsgObjConfig.messageId           = CAN_MESSAGE_ID_TX1;
//      g_multican1.canMsgObjConfig.frame               = IfxMultican_Frame_transmit;
//      g_multican1.canMsgObjConfig.control.messageLen  = IfxMultican_DataLengthCode_8;
//      g_multican1.canMsgObjConfig.txInterrupt.enabled = TRUE;
//      g_multican1.canMsgObjConfig.txInterrupt.srcId   = TX_INTERRUPT_SRC_ID1;
//
//      IfxMultican_Can_MsgObj_init(&g_multican1.canSrcMsgObj, &g_multican1.canMsgObjConfig);
//
//
//
//
//    IfxPort_setPinModeOutput(STB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
//    /* Set STB Pin of CAN chip (low-level active) */
//    IfxPort_setPinLow(STB);
//}

void initMultican2(void)
{
    /* ==========================================================================================
     * CAN module configuration and initialization:
     * ==========================================================================================
     *  - load default CAN module configuration into configuration structure
     *
     *  - define the interrupt priority for both interrupt node pointers used in the example
     *
     *  - initialize CAN module with the modified configuration
     * ==========================================================================================
     */
    IfxMultican_Can_initModuleConfig(&g_multican2.canConfig, &MODULE_CAN);
    g_multican2.canConfig.nodePointer[RX_INTERRUPT_SRC_ID2].priority = ISR_PRIORITY_CAN_RX2;
    IfxMultican_Can_initModule(&g_multican2.can, &g_multican2.canConfig);

    /* ==========================================================================================
     * Source CAN node configuration and initialization:
     * ==========================================================================================
     *  - load default CAN node configuration into configuration structure
     *
     *  - set source(TX) CAN node
     *  - assign source CAN node to CAN node 0
     *
     *  - initialize the source CAN node with the modified configuration
     * ==========================================================================================
     */
    IfxMultican_Can_Node_initConfig(&g_multican2.canNodeConfig, &g_multican2.can);

    //g_multican2.canNodeConfig.loopBackMode = TRUE;
    g_multican2.canNodeConfig.baudrate = 500000UL;
    g_multican2.canNodeConfig.nodeId = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);                         // CAN�� TX Node ID ���� 0������ ������
    g_multican2.canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                          // RX�� ����
    g_multican2.canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    g_multican2.canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                          // TX�� ����

    IfxMultican_Can_Node_init(&g_multican2.canSrcNode, &g_multican2.canNodeConfig);                              // TX configuration ����

    /* ==========================================================================================
     * Destination CAN node configuration and initialization:
     * ==========================================================================================
     *  - load default CAN node configuration into configuration structure
     *
     *  - set destination(RX) CAN node
     *  - assign destination CAN node to CAN node 1
     *
     *  - initialize the destination CAN node with the modified configuration
     * ==========================================================================================
     */
    IfxMultican_Can_Node_initConfig(&g_multican2.canNodeConfig, &g_multican2.can);

    g_multican2.canNodeConfig.baudrate = 500000UL;                                                              // CAN �ӵ� ���� 500kbps
    g_multican2.canNodeConfig.nodeId    = (IfxMultican_NodeId)((int)IfxMultican_NodeId_0);                      // CAN�� RX Node ID ���� 0������ ������
    g_multican2.canNodeConfig.rxPin     = &IfxMultican_RXD0B_P20_7_IN;                                          // RX�� ����
    g_multican2.canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
    g_multican2.canNodeConfig.txPin     = &IfxMultican_TXD0_P20_8_OUT;                                          // TX�� ����


    IfxMultican_Can_Node_init(&g_multican2.canDstNode, &g_multican2.canNodeConfig);                              // RX configuration ����

    /* ==========================================================================================
     * Source message object configuration and initialization:
     * ==========================================================================================
     *  - load default CAN message object configuration into configuration structure
     *
     *  - define the message object ID
     *  - define the CAN message ID used during arbitration phase
     *  - define the message object as a transmit message object
     *  - enable interrupt generation in case of CAN message transmission
     *  - define interrupt node pointer to be used
     *
     *  - initialize the source CAN message object with the modified configuration
     * ==========================================================================================
     */
    IfxMultican_Can_MsgObj_initConfig(&g_multican2.canMsgObjConfig, &g_multican2.canSrcNode);

    g_multican2.canMsgObjConfig.msgObjId = SRC_MESSAGE_OBJECT_ID2;
    g_multican2.canMsgObjConfig.messageId = CAN_MESSAGE_ID_TX2;
    g_multican2.canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    g_multican2.canMsgObjConfig.txInterrupt.enabled = TRUE;
    g_multican2.canMsgObjConfig.txInterrupt.srcId = TX_INTERRUPT_SRC_ID2;

    IfxMultican_Can_MsgObj_init(&g_multican2.canSrcMsgObj, &g_multican2.canMsgObjConfig);

    /* ==========================================================================================
     * Destination message object configuration and initialization:
     * ==========================================================================================
     *  - load default CAN message object configuration into configuration structure
     *
     *  - define the message object ID (different than the ID used for source MO)
     *  - define the CAN message ID used during arbitration phase (same as ID used for source MO)
     *  - define the message object as a receive message object
     *  - enable interrupt generation in case of CAN message transmission
     *  - define interrupt node pointer to be used (different than the one used for source MO)
     *
     *  - initialize the destination CAN message object with the modified configuration
     * ==========================================================================================
     */
    IfxMultican_Can_MsgObj_initConfig(&g_multican2.canMsgObjConfig, &g_multican2.canDstNode);

    g_multican2.canMsgObjConfig.msgObjId = DST_MESSAGE_OBJECT_ID2;
    g_multican2.canMsgObjConfig.messageId = CAN_MESSAGE_ID_TX1;
    g_multican2.canMsgObjConfig.frame = IfxMultican_Frame_receive;
    g_multican2.canMsgObjConfig.rxInterrupt.enabled = TRUE;
    g_multican2.canMsgObjConfig.rxInterrupt.srcId = RX_INTERRUPT_SRC_ID2;

    IfxMultican_Can_MsgObj_init(&g_multican2.canDstMsgObj, &g_multican2.canMsgObjConfig);

    IfxPort_setPinModeOutput(STB, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
    /* Set STB Pin of CAN chip (low-level active) */
    IfxPort_setPinLow(STB);
}
/* Function to initialize both TX and RX messages with the default data values.
 * After initialization of the messages, the TX message will be transmitted.
 */
void transmitCanMessage(void)
{
    /* Define the content of the data to be transmitted */
    const uint32 dataLow  = 0xC0CAC01A;
    const uint32 dataHigh = 0xBA5EBA11;

    /* Invalidation of the RX message */
    IfxMultican_Message_init(&g_multican2.rxMsg,
                             INVALID_ID_VALUE,
                             INVALID_DATA_VALUE,
                             INVALID_DATA_VALUE,
                             g_multican2.canMsgObjConfig.control.messageLen);

    /* Initialization of the TX message */
    IfxMultican_Message_init(&g_multican2.txMsg,
                             g_multican2.canMsgObjConfig.messageId,
                             dataLow,
                             dataHigh,
                             g_multican2.canMsgObjConfig.control.messageLen);

    /* Send the CAN message with the previously defined TX message content */
    while( IfxMultican_Status_notSentBusy ==
           IfxMultican_Can_MsgObj_sendMessage(&g_multican2.canSrcMsgObj, &g_multican2.txMsg) )
    {
    }


}

void initLed(void)
{
    /* ======================================================================
     * Configuration of the pins connected to the LEDs:
     * ======================================================================
     *  - define the GPIO port
     *  - define the GPIO pin that is the connected to the LED
     *  - define the general GPIO pin usage (no alternate function used)
     *  - define the pad driver strength
     * ======================================================================
     */
    g_led.led1.port      = &MODULE_P00;
    g_led.led1.pinIndex  = PIN5;
    g_led.led1.mode      = IfxPort_OutputIdx_general;
    g_led.led1.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    g_led.led2.port      = &MODULE_P00;
    g_led.led2.pinIndex  = PIN6;
    g_led.led2.mode      = IfxPort_OutputIdx_general;
    g_led.led2.padDriver = IfxPort_PadDriver_cmosAutomotiveSpeed1;

    /* Initialize the pins connected to LEDs to level "HIGH"; will keep the LEDs turned off as default state */
    IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);
    IfxPort_setPinHigh(g_led.led2.port, g_led.led2.pinIndex);

    /* Set the pin input/output mode for both pins connected to the LEDs */
    IfxPort_setPinModeOutput(g_led.led1.port, g_led.led1.pinIndex, IfxPort_OutputMode_pushPull, g_led.led1.mode);
    IfxPort_setPinModeOutput(g_led.led2.port, g_led.led2.pinIndex, IfxPort_OutputMode_pushPull, g_led.led2.mode);

    /* Set the pad driver mode for both pins connected to the LEDs */
    IfxPort_setPinPadDriver(g_led.led1.port, g_led.led1.pinIndex, g_led.led1.padDriver);
    IfxPort_setPinPadDriver(g_led.led2.port, g_led.led2.pinIndex, g_led.led2.padDriver);
}


void blinkLED1(void)
{
   IfxPort_setPinHigh(g_led.led1.port, g_led.led1.pinIndex);
   waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_10ms));    /* Wait 1 milliseconds            */
   IfxPort_setPinLow(g_led.led1.port, g_led.led1.pinIndex);
}
void blinkLED2(void)
{
    IfxPort_setPinHigh(g_led.led2.port, g_led.led2.pinIndex);                                                    /* Toggle the state of the LED      */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME_20ms));    /* Wait 500 milliseconds            */
    IfxPort_setPinLow(g_led.led2.port, g_led.led2.pinIndex);
}
