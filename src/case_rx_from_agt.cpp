/*
 * case_rx_from_agt.cpp
 *
 * Note: This function is called on every iteration of loop(). We check for data, it may not always be sent.
 * There are multiple types of information we receive from the AGT;
 *  1. AGT Status - basic stuff about the AGT itself, its always included in any TX to the FMX.
 *  2. FMX CONFIG Data - if we need to configure the FMX (due to new MT msg from Ground
 *                        or any other reason), we will RX new MT based values from the AGT 
 *                        otherwise it will send us NC (No Change) values.
 *  3. AP CONFIG Data - if we need to configure the AP (due to new MT msg from Ground
 *                        or any other reason), we will RX new MT based values from the AGT 
 *                        otherwise it will send us NC (No Change) values.
 *  4. AP MISSION Data - if we have new mission information (due to new MT msg from Ground
 *                        or any other reason), we will RX new MT based values from the AGT 
 *                        otherwise it will send us NC (No Change) values.
 */

#include "global.h"

void case_rx_from_agt()
{
    //debugPrintln("case_rx_from_agt() - executing");

    // No need to set any Pins, the FMX just regularly checks if the AGT has sent it anything.
    // And if it has, then it flags it for processing later.


    // Check if the AGT has sent us anything
    // Use SerialTransfer receive code to get anything from our peer.
    int numbytesexpected = sizeof(myAgtSharedSettings);
    // Serial.print("   ...");Serial.println(numbytesexpected);
    uint8_t numavail = STdriverF2A.available();
    if (numavail > 0)
    {
        debugPrint("case_rx_from_agt() - ST num chars avail:"); debugPrintlnInt(numavail);
    }

    if(numavail > (numbytesexpected - 1))  //  (sizeof(myAgtSharedSettings)-1)) i.e only proceed if the full ST blob is available for us to collect.
    {        
        STdriverF2A.rxObj(myAgtSharedSettings);     // Receive msg from AGT and store it in global myAgtSharedSettings.

        debugPrintln("case_rx_from_agt() - Received a new Datum from AGT");

        // Just to be defensive, check if the msg I got from AGT was NOT an ACK msg, and
        // was in fact a full msg that we want to act on etc.
        // One thing we want to avoid is ACK'ing an ACK...that could be a vicious cycle!!
        if (myAgtSharedSettings.AGT_ACK_FOR_FMX == 0)
        {
            debugPrintln("case_rx_from_agt() - Its a FULL msg so setting the following flags...");
            debugPrintln("case_rx_from_agt() - flag_got_msg_from_agt = true");
            // Set flag(s) for subsequent states to use.
            flag_got_msg_from_agt = true;   // flag for subsequent processing

            send_ack_to_agt();  // Immediately let the AGT know we have received the msg it sent.
        }
        printMyAgtSharedSettings();
    }
    else
        // Set flag(s) for subsequent states to use.
        flag_got_msg_from_agt = false;   // flag for subsequent processing

} // END - case_rx_from_agt()