/*
 * case_heartbeat_to_autopilot.cpp
 * 
 */

#include "global.h"

void case_heartbeat_to_autopilot()
{
    // if its time to send a MAVLink HEARTBEAT to the AutoPilot
    if (seconds_since_last_mavlink_heartbeat_tx > MAVLINKHEARTBEATPERIODSECONDS)
    {
        debugPrintln(" ");
        debugPrintln("case_heartbeat_to_autopilot() - Time to execute.");

        mavlink_fmx_send_heartbeat_to_ap();

        seconds_since_last_mavlink_heartbeat_tx = 0;  // reset the counter
    }
    else
    {
        #ifdef MAVLINK_DEBUG 
            //debugPrintln("case_heartbeat_to_autopilot() - NOT NOW");
        #endif
    }

}