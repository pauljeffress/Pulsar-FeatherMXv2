/*
 * case_tx_to_agt.cpp
 * 
 * Note: This function is called when the FMX sees the need to send data to the Ground (it goes via the AGT).
 * There are three types of information we send to the AGT;
 *  1. FMX Status - basic stuff about the FMX itself, its always included in any TX to the .
 *  2. PF Status - data originally from the PowerFeather, the most recent version is always included in any TX to the AGT.
 *  3. AP Status - data originally from the AutoPilot, the most recent version is always included in any TX to the AGT.
 * 
 */

#include "global.h"

void case_tx_to_agt()
{
    bool result = false;    // track success/failure of tx to agt.
    
    // if its time to do a routine send (worst case sync method) OR something else has flagged that we should do it then
    if ((seconds_since_last_agt_tx > myFmxSettings.FMX_TXAGTINT) or (flag_tx_msg_to_agt))
    {
        debugPrintln("\ncase_tx_to_agt() - Starting");

        loadupFmxSharedSettings();  // refresh all parameters in myFmxSharedSettings.
        setFmxSharedSettingNextMagicnum();  // update the MAGICNUM in myFmxSharedSettings.

        set_agt_activate_pin(); // power up the AGT, we want to send something to it!

        result = wait_agt_rx_ready_pin();

        if (result) // there is no point doing the below if AGT is not ready.
        {
            // Send the Datum to peer
            debugPrint("case_tx_to_agt() - Sending FULL Datum to AGT. My MAGICNUM=");debugPrintlnInt(myFmxSharedSettings.FMX_MAGICNUM);
            uint8_t numbytessent = STdriverF2A.sendDatum(myFmxSharedSettings);  
            debugPrint("case_tx_to_agt() - FMX to AGT transfer finished - numbytessent:");debugPrintlnInt(numbytessent);  //debugPrintlnInt(numbytessent);

            // Check if the AGT has ACK'd our TX
            result = wait_agt_ack();
        }

        if (result) // i.e. we succeeded in tx to AGT and receiving correct ACK back.
        {
            flag_tx_msg_to_agt = false;         // Clear the flag as we have succeeded.
            seconds_since_last_agt_tx =  0; // reset this counter as we have succeeded.
            clear_agt_activate_pin(); // allow the AGT Power Timer to shutdown the AGT if it wants to.
        }
        else // i.e. we failed in tx to AGT.
        {
            flag_tx_msg_to_agt = true;  // Explicitly set this flag as we needed to TX to AGT but failed. 
                                    // This will cause FMX to retry TX to AGT, next time around loop().
                                    // xxx - do I need to put a limit on num retries, so this doesn't keep getting tried forever.
        }
        
        debugPrintln("case_tx_to_agt() - Complete");
    }
    else
    {
        //debugPrintln("case_tx_to_agt() - NOT NOW");
    }

}   // END - case_tx_to_agt()