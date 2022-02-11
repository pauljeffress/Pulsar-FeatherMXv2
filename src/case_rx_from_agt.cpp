/*
 * case_rx_from_agt.cpp
 *
 */

#include "global.h"

void case_rx_from_agt()
{
    // debugPrintln("case_rx_from_agt() - executing");

    // Check if the AGT has sent us anything
    // Use SerialTransfer receive code to get anything from our peer.
    int numbytesexpected = sizeof(myAgtSharedSettings);
    
    // Serial.print("   ...");Serial.println(numbytesexpected);
    if (STdriverF2A.available() > (numbytesexpected - 1))
    {
        STdriverF2A.rxObj(myAgtSharedSettings);

        seconds_since_last_agt_rx = 0; // reset the counter

        debugPrintln(" ");
        debugPrintln("case_rx_from_agt() - FMX received a Datum from the AGT");

        #ifdef RX_FROM_AGT_DEBUG
            Serial.println("case_rx_from_agt() - RRRRRRRRRRRRRRRR");
            Serial.print("case_rx_from_agt() - AGT MAGICNUM=");
            Serial.println(myAgtSharedSettings.MAGICNUM);
            Serial.println();
            Serial.print("case_rx_from_agt() - AGT BATTV=");
            Serial.println(myAgtSharedSettings.BATTV);
            Serial.print("case_rx_from_agt() - AGT PRESS=");
            Serial.println(myAgtSharedSettings.PRESS);
            Serial.print("case_rx_from_agt() - AGT TEMP=");
            Serial.println(myAgtSharedSettings.TEMP);
            Serial.print("case_rx_from_agt() - AGT HUMID=");
            Serial.println(myAgtSharedSettings.HUMID);
            Serial.println();
            Serial.print("case_rx_from_agt() - AGT GPSYEAR=");
            Serial.println(myAgtSharedSettings.GPSYEAR);
            Serial.print("case_rx_from_agt() - AGT GPSMONTH=");
            Serial.println(myAgtSharedSettings.GPSMONTH);
            Serial.print("case_rx_from_agt() - AGT GPSDAY=");
            Serial.println(myAgtSharedSettings.GPSDAY);
            Serial.print("case_rx_from_agt() - AGT GPSHOUR=");
            Serial.println(myAgtSharedSettings.GPSHOUR);
            Serial.print("case_rx_from_agt() - AGT GPSMIN=");
            Serial.println(myAgtSharedSettings.GPSMIN);
            Serial.print("case_rx_from_agt() - AGT GPSSSEC=");
            Serial.println(myAgtSharedSettings.GPSSEC);
            Serial.print("case_rx_from_agt() - AGT GPSMILLIS=");
            Serial.println(myAgtSharedSettings.GPSMILLIS);
            Serial.println();
            Serial.print("case_rx_from_agt() - AGT LAT=");
            Serial.println(myAgtSharedSettings.LAT);
            Serial.print("case_rx_from_agt() - AGT LON=");
            Serial.println(myAgtSharedSettings.LON);
            Serial.print("case_rx_from_agt() - AGT ALT=");
            Serial.println(myAgtSharedSettings.ALT);
            Serial.print("case_rx_from_agt() - AGT SPEED=");
            Serial.println(myAgtSharedSettings.SPEED);
            Serial.print("case_rx_from_agt() - AGT HEAD=");
            Serial.println(myAgtSharedSettings.HEAD);
            Serial.print("case_rx_from_agt() - AGT SATS=");
            Serial.println(myAgtSharedSettings.SATS);
            Serial.print("case_rx_from_agt() - AGT PDOP=");
            Serial.println(myAgtSharedSettings.PDOP);
            Serial.print("case_rx_from_agt() - AGT FIX=");
            Serial.println(myAgtSharedSettings.FIX);
            Serial.println();
        #endif
        main_state = PROCESS_AGT; // now that we have data from the AGT lets process it
    }
    main_state = TX_TO_AGT; // we can skip "process_agt" as we did not receive anything from it.
} // END - case_rx_from_agt()