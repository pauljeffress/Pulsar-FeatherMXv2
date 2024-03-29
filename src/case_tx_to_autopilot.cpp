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
        oled.println("\ntx_to_autopilot()");
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
         * Check and action SET_AP_PARAM commands from Ground.
         * ===================================================
         */
        if ((myAgtSharedSettings.set_ap_param_paramvaluetype1 != 0) && (myAgtSharedSettings.set_ap_param_paramname1 != ""))      
                                        // If set_ap_param_paramvaluetype == 0 that means this the Ground
                                        // has not populated this request, se we take no action.
                                        // Likewise, we double check myAgtSharedSettings.set_ap_param_paramname1 != ""
                                        // and also abort if it is empty.
        {
             debugPrintln("case_tx_to_autopilot() - setting ap param 1");
             result = mavlink_set_one_param_on_ap(myAgtSharedSettings.set_ap_param_paramname1,
                                     myAgtSharedSettings.set_ap_param_paramvalue_int1,
                                     myAgtSharedSettings.set_ap_param_paramvalue_float1,
                                     myAgtSharedSettings.set_ap_param_paramvaluetype1);  
        
            if (result) // i.e we successfully set the param on the AP and verified it.
            {
                debugPrintln("case_tx_to_autopilot() - setting ap param 1 - SUCCESS");
                myFmxSettings.AP_SET_PARAM1_RESULT = result;   // set response from Boat to Ground accordingly
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - setting ap param 1 - FAILED");
                myFmxSettings.AP_SET_PARAM1_RESULT = result;   // set response from Boat to Ground accordingly
            }
        }

        // do the same as above but for PARAM2
        if ((myAgtSharedSettings.set_ap_param_paramvaluetype2 != 0) && (myAgtSharedSettings.set_ap_param_paramname2 != ""))      
                                        // If set_ap_param_paramvaluetype == 0 that means this the Ground
                                        // has not populated this request, se we take no action.
                                        // Likewise, we double check myAgtSharedSettings.set_ap_param_paramname1 != ""
                                        // and also abort if it is empty.
        {
             debugPrintln("case_tx_to_autopilot() - setting ap param 2");
             result = mavlink_set_one_param_on_ap(myAgtSharedSettings.set_ap_param_paramname2,
                                     myAgtSharedSettings.set_ap_param_paramvalue_int2,
                                     myAgtSharedSettings.set_ap_param_paramvalue_float2,
                                     myAgtSharedSettings.set_ap_param_paramvaluetype2);  
        
            if (result) // i.e we successfully set the param on the AP and verified it.
            {
                debugPrintln("case_tx_to_autopilot() - setting ap param 2 - SUCCESS");
                myFmxSettings.AP_SET_PARAM2_RESULT = result;   // set response from Boat to Ground accordingly
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - setting ap param 2 - FAILED");
                myFmxSettings.AP_SET_PARAM2_RESULT = result;   // set response from Boat to Ground accordingly
            }
        }


        /*
         * Check and action any REQUEST_AP_PARAM commands from Ground.
         * ===========================================================
         */
        int32_t tempInt = 0;
        float   tempFloat = 0;

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
                debugPrintln("case_tx_to_autopilot() - request successful");
                // copy the received results into data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = myAgtSharedSettings.request_ap_param_paramvaluetype1;
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = tempInt;
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = tempFloat;
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - request FAILED");
                // flag the failure into the data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE1 = MAV_REQ_PARAM_FAIL;    // signal to ground that we did not get the param value that was requested.
                myFmxSettings.AP_REQ_PARAM_VALUEINT1 = 66;    // just to ensure there is a value stored, and its obviously a placeholder only
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1 = 66.66;    // just to ensure there is a value stored, and its obviously a placeholder only
            }

            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUETYPE1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUETYPE1);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUEINT1:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUEINT1);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUEFLOAT1:"); debugPrintlnFlt(myFmxSettings.AP_REQ_PARAM_VALUEFLOAT1);

            flag_tx_msg_to_agt = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }


        // do the same as above but for PARAM2
        tempInt = 0;
        tempFloat = 0;
        if (myAgtSharedSettings.request_ap_param_paramname2 != "")      // If not empty then go ahead and REQUEST the param,
                                                                        // as empty indicates the Ground has not populated
                                                                        // this REQUEST_AP_PARAM command.
        {
             debugPrint("case_tx_to_autopilot() - requesting paramname2:");Serial.println(myAgtSharedSettings.request_ap_param_paramname2);
             result = mavlink_get_one_param_from_ap(myAgtSharedSettings.request_ap_param_paramname2,
                                                    &tempInt,
                                                    &tempFloat,
                                                    myAgtSharedSettings.request_ap_param_paramvaluetype2);  
            if (result) // i.e we requested and received the desired param from the  AP
            {
                debugPrintln("case_tx_to_autopilot() - request successful");
                // copy the received results into data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE2 = myAgtSharedSettings.request_ap_param_paramvaluetype2;
                myFmxSettings.AP_REQ_PARAM_VALUEINT2 = tempInt;
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2 = tempFloat;
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - request FAILED");
                // flag the failure into the data structure ready to send back to ground via AGT.
                myFmxSettings.AP_REQ_PARAM_VALUETYPE2 = MAV_REQ_PARAM_FAIL;    // signal to ground that we did not get the param value that was requested.
                myFmxSettings.AP_REQ_PARAM_VALUEINT2 = 66;    // just to ensure there is a value stored, and its obviously a placeholder only
                myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2 = 66.66;    // just to ensure there is a value stored, and its obviously a placeholder only
            }

            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUETYPE2:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUETYPE2);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUEINT2:"); debugPrintlnInt(myFmxSettings.AP_REQ_PARAM_VALUEINT2);
            debugPrint("case_tx_to_autopilot() - AP_REQ_PARAM_VALUEFLOAT2:"); debugPrintlnFlt(myFmxSettings.AP_REQ_PARAM_VALUEFLOAT2);

            flag_tx_msg_to_agt = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }


        /*
         *  Check and action mission data.
         */
        if (flag_got_msg_from_agt_with_mission)     // the mission data was pre validated and flag 
                                                    // would have been set if it looked like we had
                                                    // received a valid mission from the AGT. 
        {
            debugPrintln("case_tx_to_autopilot() - flag_got_msg_from_agt_with_mission = TRUE, so sending mission to AP - xxx NOT CODED YET");
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

            myFmxSettings.AP_MISSION_RESULT = 66; // xxx - set to proper pulsar mav mission result 
            flag_tx_msg_to_agt = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }

        /*
         * Set the AP FlightMode via MAV_CMD_DO_SET_MODE (#176) https://mavlink.io/en/messages/common.html#MAV_CMD_DO_SET_MODE
         * =======================================
         */
        if (myAgtSharedSettings.set_ap_flightmode != NULL_FLIGHTMODE)   // Is ground asking us to change flightmode?
        {
            debugPrintln("case_tx_to_autopilot() - Set Flightmode - xxx NOT FULLY CODED YET!");
            debugPrintln("case_tx_to_autopilot() - Set Flightmode - xxx NEED TO TEST with different ARM/Disarm ETC");
            
            debugPrint("case_tx_to_autopilot() - desired Flightmode:");debugPrintlnInt(myAgtSharedSettings.set_ap_flightmode);
            
            // final bounds check - 0 = MANUAL, 4 = HOLD, 10 = AUTO, 11 = RTL
            if ((myAgtSharedSettings.set_ap_flightmode == 0) ||
                (myAgtSharedSettings.set_ap_flightmode == 4) ||
                (myAgtSharedSettings.set_ap_flightmode == 10) ||
                (myAgtSharedSettings.set_ap_flightmode == 11))
            {
                mavlink_set_flightmode_ap(myAgtSharedSettings.set_ap_flightmode);
                debugPrintln("case_tx_to_autopilot() - Set Flightmode - xxx NEED CODE to look for AP response output");
                // Even though success will be apparent from the  
                // later HEARTBEATS we capture in RX_FROM_AP, we need to check if the AP spits out an error
                // when this fails and if so, catch it and send it back to ground.
            }
            else
                debugPrintln("case_tx_to_autopilot() - Set Flightmode - WARNING - Value out of bounds, taking no action!");

            flag_tx_msg_to_agt = true;  // regardless of the outcome we still want to trigger a send to ground
                                        // as it will have the latest FLIGHTMODE in it, as captured from a HEARTBEAT.
        }


        /*
         * Reboot the AP via MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN command.
         * =======================================
         */
        if (myAgtSharedSettings.set_ap_reboot != 0)     // If anything but zero then go ahead and REBOOT the AP,
                                                        // after "set_ap_reboot" mins.
        {
            debugPrintln("case_tx_to_autopilot() - Reboot AP - xxx NOT FULLY CODED YET!");
            debugPrintln("case_tx_to_autopilot() - Reboot AP - xxx NEED TO TEST WHEN IN AUTO MODE ETC");
            
            debugPrint("case_tx_to_autopilot() - Reboot AP - wait time (mins):");debugPrintlnInt(myAgtSharedSettings.set_ap_reboot);
            
            // final bounds check on allowed times in minutes (to protect a bit against just a random number reaching here!)
            if ((myAgtSharedSettings.set_ap_flightmode == 1) ||
                (myAgtSharedSettings.set_ap_flightmode == 5) ||
                (myAgtSharedSettings.set_ap_flightmode == 10) ||
                (myAgtSharedSettings.set_ap_flightmode == 15) ||
                (myAgtSharedSettings.set_ap_flightmode == 30) ||
                (myAgtSharedSettings.set_ap_flightmode == 60) ||
                (myAgtSharedSettings.set_ap_flightmode == 90) ||
                (myAgtSharedSettings.set_ap_flightmode == 120) ||
                (myAgtSharedSettings.set_ap_flightmode == 180) ||
                (myAgtSharedSettings.set_ap_flightmode == 240)
                )
            {
                delay(1000 * 60 * myAgtSharedSettings.set_ap_reboot);   // wait before issuing the reboot
                debugPrintln("case_tx_to_autopilot() - Reboot AP - executing command now!");
                mavlink_cmd_preflight_reboot_ap();
                
                
                debugPrintln("case_tx_to_autopilot() - Reboot AP - xxx NEED CODE to look for AP reboot output");
                myFmxSettings.AP_REBOOT_RESULT = 33; // xxx - temp mark it was ok, but we need to look for proof!!!
            }
            else
            {
                debugPrintln("case_tx_to_autopilot() - Reboot AP - WARNING - Value out of bounds, taking no action!");
                myFmxSettings.AP_REBOOT_RESULT = 66; // mark that it was a problem.
            }

            flag_tx_msg_to_agt = true;  // regardless of success or failure, flag that we have something to send to AGT/Ground.
        }

        debugPrintln("case_tx_to_autopilot() - Completed for (flag_got_msg_from_agt == true) section");   
        //oled.println("case_tx_to_autopilot() - Complete");
    }   // END - if (flag_got_msg_from_agt)



    // I have commented out the below as I no longer see a need to do this.
    // // Is it time to do a periodic TX to the AutoPilot?
    // // Note: If we did a TX to the AP above due to new AP data from the AGT, then this periodic TX 
    // //       will get skipped as seconds_since_last_ap_tx will have just been reset.
    // if (seconds_since_last_ap_tx > TX_TO_AP_PERIOD_SECONDS)
    // {
    //     debugPrintln("\ncase_tx_to_autopilot() - Starting periodic TX to AP section - xxx YET TO CODE THIS!");
    //     seconds_since_last_ap_tx = 0;   // reset timer.
    //     //do_periodic_tx_to_ap();   // xxx - yet to code
    //     debugPrintln("case_tx_to_autopilot() - Completed periodic TX to AP section");
    // }
    
    
}   // END - case_tx_to_autopilot()
