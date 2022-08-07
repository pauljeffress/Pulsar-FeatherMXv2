/*
 * case_heartbeat_to_autopilot.cpp
 * 
 */

#include "global.h"

void case_heartbeat_to_autopilot()
{
    // if its time to send a MAVLink HEARTBEAT to the AutoPilot
    if (seconds_since_last_mavlink_heartbeat_tx > FMX_MAVLINKHEARTBEATPERIODSECONDS)
    {
        debugPrintln("case_heartbeat_to_autopilot() - time to execute");
        //oled.println("heartbeat_to_autopilot()");

        mavlink_fmx_send_heartbeat_to_ap();

        seconds_since_last_mavlink_heartbeat_tx = 0;  // reset the counter

        debugPrintln("case_heartbeat_to_autopilot() - finishing");
        //oled.println("case_heartbeat_to_autopilot() - finishing");
    }
    else
    {
        #ifdef MAVLINK_DEBUG 
            //debugPrintln("case_heartbeat_to_autopilot() - NOT NOW");
        #endif
    }

}