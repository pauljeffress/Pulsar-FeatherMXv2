/*
 * doFmxCmd_fns.cpp
 * 
 * Ground can send commands to the FMX by setting
 * myAgtSharedSettings.fmx_cmd_byte1 thru myAgtSharedSettings.fmx_cmd_byte5 to a non zero value
 * 
 * When an fmx_byte_cmdX is received from the Ground, and it is non zero, the corresponding
 * doFmxCmdX(uint8_t ...) is called here.
 * 
 */

#include "global.h"

#include "doFmxCmd_fns.h"


void doFmxCmd1(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd1() - Starting.");
    debugPrint("doFmxCmd1() - cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP1 = cmdByte;
    flag_tx_msg_to_agt = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd1() - Complete.");
}   // END - setFmxTxAgtInt()

void doFmxCmd2(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd2() - Starting.");
    debugPrint("doFmxCmd2() - cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP2 = cmdByte;
    flag_tx_msg_to_agt = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd2() - Complete.");
}   // END - setFmxTxAgtInt()

void doFmxCmd3(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd3() - Starting.");
    debugPrint("doFmxCmd3() - cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP3 = cmdByte;
    flag_tx_msg_to_agt = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd3() - Complete.");
}   // END - setFmxTxAgtInt()

void doFmxCmd4(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd4() - Starting.");
    debugPrint("doFmxCmd4() - cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP4 = cmdByte;
    flag_tx_msg_to_agt = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd4() - Complete.");
}   // END - setFmxTxAgtInt()

void doFmxCmd5(uint8_t cmdByte)
{
    debugPrintln("doFmxCmd5() - Starting.");
    debugPrint("doFmxCmd5() - cmdByte="); debugPrintlnInt(cmdByte);

    // Dummy instructions until I have a proper use for this particular cmdByte
    // We just copy the cmdByte into the corresponding RESP byte and flag to send it back to the Ground.
    myFmxSettings.FMX_CMD_RESP5 = cmdByte;
    flag_tx_msg_to_agt = true;  // flag that we have something to send to ground

    debugPrintln("doFmxCmd5() - Complete.");
}   // END - setFmxTxAgtInt()
