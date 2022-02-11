/*
 * case_tx_to_agt.cpp
 * 
 */

#include "global.h"

void case_tx_to_agt()
{
    // if its time to do a routine send (worst case sync method) OR something else has flagged that we should do it then
    if ((seconds_since_last_agt_tx > myFeatherMxSettings.FMX_TXAGTINT) or (flag_do_agt_tx))
    {
        debugPrintln(" ");
        debugPrintln("case_tx_to_agt() - Time to execute");
        sendSharedSettings_to_AGT();
        flag_do_agt_tx = false;
        seconds_since_last_agt_tx =  0; // reset this counter as we have sent....even though AGT may not have received.
        debugPrintln("case_tx_to_agt() - finished");
    }
    else
    {
        //debugPrintln("case_tx_to_agt() - NOT NOW");
    }


}