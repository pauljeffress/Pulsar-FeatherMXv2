/*
 * case_tx_to_autopilot.cpp
 * 
 */

#include "global.h"

/*============================
 * case_tx_to_autopilot()
 *
 * Do a periodic TX to the AP, via MAVLink messages. 
 * AND/OR
 * Do a specifically flagged TX to the AP (usually due to a newly received msg from Ground).
 * 
 *============================*/
void case_tx_to_autopilot()
{
    bool result = false;

    // if its time to do a TX to the AP due to a recent message from the AGT?
    if (flag_tx_msg_to_ap)
    {
        debugPrintln("\ncase_tx_to_autopilot() - Starting for (flag_tx_msg_to_ap == true) section");
        flag_tx_msg_to_ap = false;  // Clear flag as we don't need it after this.
        seconds_since_last_ap_tx = 0;   // reset timer.
        
        /*
         * Check and action ARM/DISARM commands.
         * =====================================
         */
        if (myAgtSharedSettings.set_ap_arm_state_b && myAgtSharedSettings.set_ap_disarm_state_b)    // if both ARM and DISARM commands are set then thats a problem
                                                                                                    // and we will not execute either of them!
        {
            debugPrintln("case_tx_to_autopilot() - ERROR - both ARM and DISARM are requested, so not doing either!");
        }
        else    // i.e. only one, or none of the two ARM/DISARM commands are being requested.
        {
            if (myAgtSharedSettings.set_ap_arm_state_b) 
            {
                debugPrintln("case_tx_to_autopilot() - ARMing the AP");
                mavlink_set_arm_ap();           // xxx - set_arm_ap() needs improvement, see notes in its comments.
            }
            if (myAgtSharedSettings.set_ap_disarm_state_b)
            {
                debugPrintln("case_tx_to_autopilot() - DISarming the AP");
                mavlink_set_disarm_ap();     // xxx - set_disarm_ap() needs improvement, see notes in its comments.
            }
        }



        /*
         * Check and action SET_AP_PARAM commands.
         * =======================================
         */
        if (myAgtSharedSettings.set_ap_param_paramvaluetype1)   // If anything but zero then go ahead and SET the param,
                                                                // as type = 0 indicates the Ground has not populated
                                                                // this SET_AP_PARAM command.
        {
             debugPrintln("case_tx_to_autopilot() - setting ap param 1 - NOT FULLY CODED YET!");
             result = mavlink_set_one_param_on_ap(myAgtSharedSettings.set_ap_param_paramname1,
                                     myAgtSharedSettings.set_ap_param_paramvalue_int1,
                                     myAgtSharedSettings.set_ap_param_paramvalue_float1,
                                     myAgtSharedSettings.set_ap_param_paramvaluetype1);  
        
            if (result) // i.e we did send cmd to AP (it may not have received or actioned it though...we don't know yet)
            {
                // do a get AP PARAM and confirm the GOT value = the SET value,




                
                //    then either copy the GOT value into myFmxSharedSettings or just set a boolean in myFmxSharedSettings
            }

            //     xxx - copy result into correct myFmxSharedSettings field for return to Agt/Ground 
            //           to report on success/failure of 1st set param command.
            //           and also set flag that we have something to send to AGT/Ground.

        }


        /*
         * Check and action any REQUEST_AP_PARAM commands from Ground.
         * ===========================================================
         */
        int32_t tempInt = 0;
        float   tempFloat = 0;

        uint8_t AP_REQ_PARAMVALUETYPE1 = 0;
        int32_t AP_REQ_PARAMVALUE_INT1 = 0;
        float   AP_REQ_PARAMVALUE_FLOAT1 = 0;

        if (myAgtSharedSettings.request_ap_param_paramname1 != "")      // If not empty then go ahead and REQUEST the param,
                                                                        // as empty indicates the Ground has not populated
                                                                        // this REQUEST_AP_PARAM command.
        {
             debugPrint("case_tx_to_autopilot() - requesting paramname1:");Serial.println(myAgtSharedSettings.request_ap_param_paramname1);
             result = mavlink_get_one_param_from_ap(myAgtSharedSettings.request_ap_param_paramname1,
                                                    &tempInt,
                                                    &tempFloat,
                                                    myAgtSharedSettings.request_ap_param_paramvaluetype1);  
            if (result) // i.e we requested and received the desired param from the  AP
            {
                debugPrint("case_tx_to_autopilot() - request successful - MORE CODE REQUIRED HERE");
                // copy the received results into data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = myAgtSharedSettings.request_ap_param_paramvaluetype1;
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = tempInt;
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = tempFloat;
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - request FAILED - MORE CODE REQUIRED HERE");
                // flag the failure into the data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = MAV_REQ_PARAM_FAIL;    // signal to ground that we did not get the param value that was requested.
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = 66;    // just to ensure there is a value stored, and its obviously a placeholder only
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = 66.66;    // just to ensure there is a value stored, and its obviously a placeholder only
            }

            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAMVALUETYPE1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUETYPE1);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAMVALUE_INT1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUEINT1);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAMVALUE_FLOAT1:"); debugPrintlnFlt(myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1);

            flag_tx_msg_to_agt = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }


        // do the same as above but for PARAM2
        if (myAgtSharedSettings.request_ap_param_paramname2 != "")      // If not empty then go ahead and REQUEST the param,
                                                                        // as empty indicates the Ground has not populated
                                                                        // this REQUEST_AP_PARAM command.
        {
            debugPrintln("case_tx_to_autopilot() - request_ap_param_param2 - NOT CODED YET");
        }


        /*
         *  Check and action mission data.
         */
        if (flag_got_msg_from_agt_with_mission)     // the mission data was pre validated and flag 
                                                    // would have been set if it looked like we had
                                                    // received a valid mission from the AGT. 
        {
            debugPrintln("case_tx_to_autopilot() - flag_got_msg_from_agt_with_mission = TRUE, so sending mission to AP - NOT CODED YET");
            // xxx Do the mission tx to AP stuff.

                //mission_clear_all_ap();
                //global_mission_dummy_load();
                //global_mission_print();


                //PulsarMavMissionResult_t  pmmr;   // local struct to hold results ready for return to callee.
                //pmmr.pr = NOT_SET;
                //pmmr.mmr = MAV_MISSION_ACCEPTED;
                //pmmr.u32value = 0;
                //pmmr = mission_upload_to_ap();
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.pr:"); Serial.println(pmmr.pr);
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.mmr(UNUSED):"); Serial.println(pmmr.mmr);
                //debugPrint("mavlink_get_MISSION_REQUEST_INT() - pmmr.u32value:"); Serial.println(pmmr.u32value);

                // #ifdef MAVLINK_DEBUG 
                //     debugPrintln("case_tx_to_autopilot() - Periodic TX - do immediate mavlink_receive() to check for response");
                // #endif

                // // do an immediate mavlink_receive() to see if the AP responded to me setting the param above.
                // uint32_t start = millis();  // xxx - need to review how I'm timing this loop...seems clunky. Also need to take any contants and set the as #defines.
                // while ((millis() < (start + 3000)) && (millis() > 4000))  // For 3 seconds, see if we can assemble received msgs and if so process them.
                //     mavlink_receive(); 
        }

        debugPrintln("case_tx_to_autopilot() - Completed for (flag_got_msg_from_agt == true) section");    
    }   // END - if (flag_got_msg_from_agt)


    // Is it time to do a periodic TX to the AutoPilot?
    // Note: If we did a TX to the AP above due to new AP data from the AGT, then this periodic TX 
    //       will get skipped as seconds_since_last_ap_tx will have just been reset.
    if (seconds_since_last_ap_tx > TX_TO_AP_PERIOD_SECONDS)
    {
        debugPrintln("\ncase_tx_to_autopilot() - Starting periodic TX to AP section - YET TO CODE THIS!");
        seconds_since_last_ap_tx = 0;   // reset timer.
        //do_periodic_tx_to_ap();   // xxx - yet to code
        debugPrintln("case_tx_to_autopilot() - Completed periodic TX to AP section");

    }
    
    
}   // END - case_tx_to_autopilot()
