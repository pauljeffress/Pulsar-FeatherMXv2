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
    fmx_tx_to_agt_numtries = 0; // zero the counter.

    /*
     *  Should we flag to do the initial AGT tx?
     *
     *  To help with testing, we want the FMX to do a TX to the AGT after being powered-up/reboot, but not immediately.
     *  We want it to wait a few minutes to let AGT and GSE32 that may possibly just have been rebooted, to also settle.
     */
    if ((seconds_since_reset_or_powercycle > FIRST_TX_TO_AGT_WAIT_S) && flag_do_first_agt_tx)
    {
        flag_do_first_agt_tx = false;   // Clear this flag so we never evaluate this again.
        flag_first_agt_tx = true;       // Flag that we want to fo the initial TX now.
    }
    else flag_first_agt_tx = false;     // Flag not to do the initial TX.




    // if its time to do a routine send (worst case sync method) OR something else has flagged that we should do it then
    if ((seconds_since_last_agt_tx > myFmxSettings.FMX_TXAGTINT) || (flag_tx_msg_to_agt) || (flag_first_agt_tx))
    {
        debugPrint("\ncase_tx_to_agt() - Starting");
        debugPrint(" - due to:");
        if (flag_tx_msg_to_agt) debugPrintln("flag_tx_msg_to_agt=TRUE");
        else if (flag_first_agt_tx) debugPrintln("flag_first_agt_tx=TRUE");
          else debugPrintln("seconds_since_last_agt_tx > myFmxSettings.FMX_TXAGTINT");

        loadupFmxSharedSettings();          // refresh all parameters in myFmxSharedSettings.
        setFmxSharedSettingNextMagicnum();  // update the MAGICNUM in myFmxSharedSettings.
        printFmxSharedSettings();
        
        while ((fmx_tx_to_agt_numtries < FMX_TX_TO_AGT_NUMTRIES_MAX) && (!result))  // Try to TX to AGT til successful or retries counts out.
        {
            fmx_tx_to_agt_numtries++;
            debugPrint("case_tx_to_agt() - TX try number:");debugPrintlnInt(fmx_tx_to_agt_numtries);
            
            set_agt_activate_pin(); // power up the AGT, we want to send something to it!

            if (seconds_since_last_ap_rx > 30)  // if we don't have recent data from the AP, go get it before we TX to the AGT.
                case_rx_from_autopilot();

            result = wait_agt_rx_ready_pin();   // is the AGT ready to receive a message from us?

            if (result) // there is no point doing the below if AGT is not ready.
            {
                // Send the Datum to peer
                debugPrint("case_tx_to_agt() - Sending FULL Datum to AGT. My MAGICNUM=");debugPrintlnInt(myFmxSharedSettings.FMX_MAGICNUM);
                uint8_t numbytessent = STdriverF2A.sendDatum(myFmxSharedSettings);  
                debugPrint("case_tx_to_agt() - FMX to AGT transfer finished - numbytessent:");debugPrintlnInt(numbytessent);  //debugPrintlnInt(numbytessent);

                // Check if the AGT has ACK'd our TX
                result = wait_agt_ack();
            }
        }
        if (result) // i.e. we succeeded in tx to AGT and receiving correct ACK back.
        {
            debugPrintln("case_tx_to_agt() - Success");
            flag_tx_msg_to_agt = false;         // Clear the flag as we have succeeded.
            seconds_since_last_agt_tx =  0; // reset this counter as we have succeeded.
        }
        else // i.e. we failed in tx to AGT.
        {
            debugPrintln("case_tx_to_agt() - FAILURE");
            flag_tx_msg_to_agt = false;         // Clear the flag as even though we failed, we have used up our 
                                                // attempts this time around so give up....until next time.
            seconds_since_last_agt_tx =  0;     // Even though we failed, reset this counter as we do not want the 
                                                // timer to kick this off again until the full myFmxSettings.FMX_TXAGTINT
                                                // duration has passed.
        }
        
        clear_agt_activate_pin(); // allow the AGT Power Timer to shutdown the AGT if it wants to.
        debugPrintln("case_tx_to_agt() - Complete");
    }

}   // END - case_tx_to_agt()