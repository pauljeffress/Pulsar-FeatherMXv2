/*
 * case_rx_from_autopilot.cpp
 * 
 */

#include "global.h"

/*============================
 * case_rx_from_autopilot()
 *
 * Do a periodic read of the various params I want from the AP, via MAVLink messages.
 * OR
 * Do it when a flag says so.  I.e. when we know we are about to TX to GROUND, we should
 *      do it to get most current values from the AP, as we may have recently issued some 
 *      commands to it.
 * 
 *============================*/
void case_rx_from_autopilot()
{
    // if its time to do a routine or on demand read of streamed AutoPilot MAVlink data?
    if ((seconds_since_last_ap_rx > myFmxSettings.FMX_RXAPINT) || flag_tx_msg_to_agt)
    {
        debugPrintln("\ncase_rx_from_autopilot() - Starting");
        seconds_since_last_ap_rx = 0;    // reset counter

        // Before trying to read from the streams we just ensure the 
        // AP is still streaming the messages we are going to look for.
        // Streaming may have been turned off for some reason (via GCS, FMX etc), so we just make sure
        // it is still/back on.
        mavlink_request_streaming_params_from_ap();



        // Collect the messages I am interested in as per requests above
        // =============================================================

        // Initialise temp holding spots and put a magic number in them so I can know when they have been updated successfully

        // xxx - I have not done this "Initialise temp holding..."


        // Do a bulk collection of data from the streamed messages

        debugPrint("case_rx_from_autopilot() - ATTEMPTING RX - starting at Millis:"); Serial.println(millis());
        // Explanation of the following while() loop
        // I need to ensure that the mavlink_receive() function gets a solid couple of contiguous
        // seconds to ensure it gets a good chance to snap up lots of mavlink data and form packets.
        // If I give it just a few milliseconds each time its not enough as it seems that the other
        // states in assess_situation() take up too much time and I think that serial chars from 
        // the autopilot were getting dropped and so I wasn't getting full packets reliably.
        // The other key thing to consider here is that the mavlink_request_streaming_params_from_ap() function
        // has asked the autopilot to send data once a second, so waiting here for a few seconds gives
        // me multiple chances to catch what I need in the mavlink stream.
        // Also, as I am concerned about this while loop playing up when millis() rolls over to zero, 
        // I am using the extra "&& (millis() > 4000)" in the while() to jump that bit close to 0.
        // xxx - AS PER https://techexplorations.com/guides/arduino/programming/millis-rollover/ I can simplify the below.
        uint32_t start = millis();  // xxx - need to review how I'm timing this loop...seems clunky. Also need to take any contants and set the as #defines.
        while ((millis() < (start + 3000)) && (millis() > 4000))  // For 3 seconds, see if we can assemble received msgs and if so process them.
            mavlink_receive(); 
        
        debugPrint("case_rx_from_autopilot() - Done RX - ending at Millis:"); Serial.println(millis());

        // At this point I should have gathered all of the data I needed from the streaming params, 
        // so we should turn them all back off and leave the MAVLink serial connection relatively quiet.
        mavlink_unrequest_streaming_params_from_ap();

        // xxx - see what we got (any magic nums still in place?) and maybe timestamp it.

        // xxx - I have not done this "see what we got..."
        debugPrintln("case_rx_from_autopilot() - xxx - MORE CODE NEEDED.");

        debugPrintln("case_rx_from_autopilot() - Complete");
    }
    else
    {
        #ifdef MAVLINK_DEBUG 
            //debugPrintln("case_rx_from_autopilot() - NOT NOW");
        #endif
    }

}