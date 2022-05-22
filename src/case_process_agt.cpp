/*
 * case_process_agt.cpp
 *
 * If we recently got a message from AGT and if there is anything for the FMX in it
 * then check and action it.
 * 
 */

#include "global.h"
#include "doFmxCmd_fns.h"

void case_process_agt()
{
    bool    processIt = true;  // gets set to false if for any reason I don't think I should process
                                // the current my AgtSharedSettings msg.

    if (flag_got_msg_from_agt)
    {
        debugPrintln("\ncase_process_agt() - starting as flag_got_msg_from_agt == true");
        
        // Start with some basic checks on the recently received myAgtSharedSettings
        if (myAgtSharedSettings.AGT_ACK_FOR_FMX)    // don't process it if its an ACK msg
            processIt = false;
        if (myAgtSharedSettings.mt_magic_number == 0)   // don't process it - mt_magic_number should never be 0
            processIt = false;
        if (myAgtSharedSettings.mt_send_timestamp_unix_s == 0) // don't process it - mt_send_timestamp_unix_s should never be 0
            processIt = false;

        // If no issues identified above, then process it (everything other than the params that are for the AP, we do that later)
        if (processIt)
        {
            debugPrintln("case_process_agt() - processing it");
            debugPrint("case_process_agt() - myAgtSharedSettings.mt_magic_number:");debugPrintlnInt(myAgtSharedSettings.mt_magic_number);
            debugPrint("case_process_agt() - myAgtSharedSettings.mt_send_timestamp_unix_s:");debugPrintInt(myAgtSharedSettings.mt_send_timestamp_unix_s);
                debugPrint("  aka  ");pulsar_timestamp_print(myAgtSharedSettings.mt_send_timestamp_unix_s);
        
            setFmxTxAgtInt(myAgtSharedSettings.set_fmx_txagtint_s);
            setFmxRxApInt(myAgtSharedSettings.set_fmx_rxapint_s);

            if (myAgtSharedSettings.fmx_cmd_byte1 != 0) doFmxCmd1(myAgtSharedSettings.fmx_cmd_byte1);
            if (myAgtSharedSettings.fmx_cmd_byte2 != 0) doFmxCmd2(myAgtSharedSettings.fmx_cmd_byte2);
            if (myAgtSharedSettings.fmx_cmd_byte3 != 0) doFmxCmd3(myAgtSharedSettings.fmx_cmd_byte3);
            if (myAgtSharedSettings.fmx_cmd_byte4 != 0) doFmxCmd4(myAgtSharedSettings.fmx_cmd_byte4);
            if (myAgtSharedSettings.fmx_cmd_byte5 != 0) doFmxCmd5(myAgtSharedSettings.fmx_cmd_byte5);

            // Remember, we do not action any MAVLink/mission content (destined for the AP) that may be in the
            // recent myAgtSharedSettings msg here. We do it in next STATE, case_process_agt_for_ap().
        }
        else
        {
            flag_got_msg_from_agt = false;  // clear this flasg as we don't want any further processing of the packet.
            debugPrintln("case_process_agt() - ERROR - RX'd msg failed some checks, so NOT processing it");
        }
        debugPrintln("case_process_agt() - Complete");
    }
}