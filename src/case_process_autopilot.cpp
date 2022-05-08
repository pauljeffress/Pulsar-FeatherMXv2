/*
 * case_process_autopilot.cpp
 * 
 */

#include "global.h"

void case_process_autopilot()
{
    // Given we may have received or not stuff in case_rx_from_autopilot(), is there any post processing/action we need to do?
    
    //debugPrintln("case_process_autopilot() - executing");
    myFmxSettings.FMX_LAST_AP_HEARTBEAT_S = seconds_since_last_mavlink_heartbeat_rx;  // keep myFmxSettings in sync with source of the data.
    


}