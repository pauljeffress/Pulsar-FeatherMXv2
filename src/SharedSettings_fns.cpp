/*
 * SharedSettings_fns.cpp
 *
 * This file may be similar on both the Feather and AGT but is slighhtly custom per side.
 */

#include "global.h" // My main header file for this project itself

bool sendSharedSettings_to_AGT(void)    // xxx - do we need to return anything here, if not get rid of the bool.
{

    debugPrintln("sendSharedSettings_to_AGT() - starting");

    bool success = false;

    preptosendFeatherSharedSettings();    // gets myFeatherSharedSettings fresh and ready to TX to AGT. 

    debugPrintln("sendSharedSettings_to_AGT() - FMX about to SerialTransfer sendDatum() to AGT");
    // Send the Datum to peer
    uint8_t numbytessent = STdriverF2A.sendDatum(myFeatherSharedSettings);
    debugPrint("sendSharedSettings_to_AGT() - FMX to AGT transfer finished - numbytessent:");
    Serial.println(numbytessent);  //debugPrintlnInt(numbytessent);

    return(success);
}   // END - sendSharedSettings_to_AGT


void initFeatherSharedSettings(void) // Initialises the myFeatherSharedSettings in RAM & is also called
                                     // to update myFeatherSharedSettings just before we do a send 
                                     // over to the AGT.
{
  // myFeatherSharedSettings.MAGICNUM = 0;   // do not reset this, the SharedSetting is the source of truth!!!!
  
  // initialise based on the correct source of truth. This is the same for both initial boot up AND when we need to refresh!
  
  myFeatherSharedSettings.FMX_BATT_V = myFeatherMxSettings.FMX_BATT_V;
  myFeatherSharedSettings.FMX_TEMP = myFeatherMxSettings.FMX_TEMP;
  myFeatherSharedSettings.FMX_RH = myFeatherMxSettings.FMX_RH;
  myFeatherSharedSettings.FMX_UPTIME_S = seconds(); // xxx - should this be coming from myFeatherMxSettings.FMX_UPTIME_S ???
  myFeatherSharedSettings.FMX_PRESS = myFeatherMxSettings.FMX_PRESS;
  myFeatherSharedSettings.FMX_WATERTEMP = myFeatherMxSettings.FMX_WATERTEMP;
  myFeatherSharedSettings.FMX_AMBIENTLIGHT = myFeatherMxSettings.FMX_AMBIENTLIGHT;
  myFeatherSharedSettings.FMX_LAST_AP_HEARTBEAT_S = myFeatherMxSettings.FMX_LAST_AP_HEARTBEAT_S;
  
  myFeatherSharedSettings.PF_BATT_V = myPowerFeatherSettings.PF_BATT_V;
  myFeatherSharedSettings.PF_TEMP = myPowerFeatherSettings.PF_TEMP;
  myFeatherSharedSettings.PF_RH = myPowerFeatherSettings.PF_RH;
  myFeatherSharedSettings.PF_UPTIME_S = myPowerFeatherSettings.PF_UPTIME_S;
  
  myFeatherSharedSettings.PF_CHARGER1_PID  = myPowerFeatherSettings.PF_CHARGER1_PID ;
  myFeatherSharedSettings.PF_CHARGER1_FW   = myPowerFeatherSettings.PF_CHARGER1_FW  ;
  myFeatherSharedSettings.PF_CHARGER1_V    = myPowerFeatherSettings.PF_CHARGER1_V   ;
  myFeatherSharedSettings.PF_CHARGER1_I    = myPowerFeatherSettings.PF_CHARGER1_I   ;
  myFeatherSharedSettings.PF_CHARGER1_VPV  = myPowerFeatherSettings.PF_CHARGER1_VPV ;
  myFeatherSharedSettings.PF_CHARGER1_PPV  = myPowerFeatherSettings.PF_CHARGER1_PPV ;
  myFeatherSharedSettings.PF_CHARGER1_CS   = myPowerFeatherSettings.PF_CHARGER1_CS  ;
  myFeatherSharedSettings.PF_CHARGER1_ERR  = myPowerFeatherSettings.PF_CHARGER1_ERR ;
  myFeatherSharedSettings.PF_CHARGER1_LOAD = myPowerFeatherSettings.PF_CHARGER1_LOAD;
  myFeatherSharedSettings.PF_CHARGER1_IL   = myPowerFeatherSettings.PF_CHARGER1_IL  ;
  myFeatherSharedSettings.PF_CHARGER1_H19  = myPowerFeatherSettings.PF_CHARGER1_H19 ;
  myFeatherSharedSettings.PF_CHARGER1_H20  = myPowerFeatherSettings.PF_CHARGER1_H20 ;
  myFeatherSharedSettings.PF_CHARGER1_H21  = myPowerFeatherSettings.PF_CHARGER1_H21 ;
  myFeatherSharedSettings.PF_CHARGER1_H22  = myPowerFeatherSettings.PF_CHARGER1_H22 ;
  myFeatherSharedSettings.PF_CHARGER1_H23  = myPowerFeatherSettings.PF_CHARGER1_H23 ;
  myFeatherSharedSettings.PF_CHARGER1_HSDS = myPowerFeatherSettings.PF_CHARGER1_HSDS;
  myFeatherSharedSettings.PF_CHARGER1_MPPT = myPowerFeatherSettings.PF_CHARGER1_MPPT;

  myFeatherSharedSettings.PF_CHARGER2_PID  = myPowerFeatherSettings.PF_CHARGER2_PID ;
  myFeatherSharedSettings.PF_CHARGER2_FW   = myPowerFeatherSettings.PF_CHARGER2_FW  ;
  myFeatherSharedSettings.PF_CHARGER2_V    = myPowerFeatherSettings.PF_CHARGER2_V   ;
  myFeatherSharedSettings.PF_CHARGER2_I    = myPowerFeatherSettings.PF_CHARGER2_I   ;
  myFeatherSharedSettings.PF_CHARGER2_VPV  = myPowerFeatherSettings.PF_CHARGER2_VPV ;
  myFeatherSharedSettings.PF_CHARGER2_PPV  = myPowerFeatherSettings.PF_CHARGER2_PPV ;
  myFeatherSharedSettings.PF_CHARGER2_CS   = myPowerFeatherSettings.PF_CHARGER2_CS  ;
  myFeatherSharedSettings.PF_CHARGER2_ERR  = myPowerFeatherSettings.PF_CHARGER2_ERR ;
  myFeatherSharedSettings.PF_CHARGER2_LOAD = myPowerFeatherSettings.PF_CHARGER2_LOAD;
  myFeatherSharedSettings.PF_CHARGER2_IL   = myPowerFeatherSettings.PF_CHARGER2_IL  ;
  myFeatherSharedSettings.PF_CHARGER2_H19  = myPowerFeatherSettings.PF_CHARGER2_H19 ;
  myFeatherSharedSettings.PF_CHARGER2_H20  = myPowerFeatherSettings.PF_CHARGER2_H20 ;
  myFeatherSharedSettings.PF_CHARGER2_H21  = myPowerFeatherSettings.PF_CHARGER2_H21 ;
  myFeatherSharedSettings.PF_CHARGER2_H22  = myPowerFeatherSettings.PF_CHARGER2_H22 ;
  myFeatherSharedSettings.PF_CHARGER2_H23  = myPowerFeatherSettings.PF_CHARGER2_H23 ;
  myFeatherSharedSettings.PF_CHARGER2_HSDS = myPowerFeatherSettings.PF_CHARGER2_HSDS;
  myFeatherSharedSettings.PF_CHARGER2_MPPT = myPowerFeatherSettings.PF_CHARGER2_MPPT;

  myFeatherSharedSettings.AP_BASEMODE             = myFeatherMxSettings.AP_BASEMODE;
  myFeatherSharedSettings.AP_CUSTOMMODE           = myFeatherMxSettings.AP_CUSTOMMODE;
  myFeatherSharedSettings.AP_SYSTEMSTATUS         = myFeatherMxSettings.AP_SYSTEMSTATUS;

  myFeatherSharedSettings.AP_POSITIONTIMESTAMP    = myFeatherMxSettings.AP_POSITIONTIMESTAMP;
  myFeatherSharedSettings.AP_LAT                  = myFeatherMxSettings.AP_LAT;
  myFeatherSharedSettings.AP_LON                  = myFeatherMxSettings.AP_LON;
  myFeatherSharedSettings.AP_VX                   = myFeatherMxSettings.AP_VX;
  myFeatherSharedSettings.AP_VY                   = myFeatherMxSettings.AP_VY;
  myFeatherSharedSettings.AP_HDG                  = myFeatherMxSettings.AP_HDG;

  myFeatherSharedSettings.AP_SATS                 = myFeatherMxSettings.AP_SATS;
  myFeatherSharedSettings.AP_VEL                  = myFeatherMxSettings.AP_VEL;
  myFeatherSharedSettings.AP_COG                  = myFeatherMxSettings.AP_COG;

  myFeatherSharedSettings.AP_VCC                  = myFeatherMxSettings.AP_VCC;
  myFeatherSharedSettings.AP_VSERVO               = myFeatherMxSettings.AP_VSERVO;
  myFeatherSharedSettings.AP_POWERFLAGS           = myFeatherMxSettings.AP_POWERFLAGS;

  myFeatherSharedSettings.AP_SENSORSPRESENT       = myFeatherMxSettings.AP_SENSORSPRESENT;
  myFeatherSharedSettings.AP_SENSORSENABLED       = myFeatherMxSettings.AP_SENSORSENABLED;
  myFeatherSharedSettings.AP_SENSORSHEALTH        = myFeatherMxSettings.AP_SENSORSHEALTH;
  myFeatherSharedSettings.AP_LOAD                 = myFeatherMxSettings.AP_LOAD;

  myFeatherSharedSettings.AP_TIMEUNIXUSEC         = myFeatherMxSettings.AP_TIMEUNIXUSEC;
  myFeatherSharedSettings.AP_TIMEBOOTMS           = myFeatherMxSettings.AP_TIMEBOOTMS;

  myFeatherSharedSettings.AP_NAVBEARING           = myFeatherMxSettings.AP_NAVBEARING;
  myFeatherSharedSettings.AP_TARGETBEARING        = myFeatherMxSettings.AP_TARGETBEARING;
  myFeatherSharedSettings.AP_WPDIST               = myFeatherMxSettings.AP_WPDIST;

  myFeatherSharedSettings.AP_VOLTAGES[0]               = myFeatherMxSettings.AP_VOLTAGES[0];
  myFeatherSharedSettings.AP_CURRENTBATTERY         = myFeatherMxSettings.AP_CURRENTBATTERY;

  myFeatherSharedSettings.AP_VENDORID               = myFeatherMxSettings.AP_VENDORID;
  myFeatherSharedSettings.AP_PRODUCTID              = myFeatherMxSettings.AP_PRODUCTID;

  //debugPrintln("initFeatherSharedSettings: RAM settings initialised");
}   // END - initFeatherSharedSettings()


void preptosendFeatherSharedSettings(void) // gets myFeatherSharedSettings fresh (mainly from myFeatherMxSettings) and ready to TX to AGT.
{
  // refresh all parameters in myFeatherSharedSettings.
  initFeatherSharedSettings();

  // update the MAGICNUM in myFeatherSharedSettings.
  myFeatherSharedSettings.FMX_MAGICNUM++;
  if (myFeatherSharedSettings.FMX_MAGICNUM > 240) // roll the counter back to 0 at 240.
    myFeatherSharedSettings.FMX_MAGICNUM = 0;

  //debugPrintln("preptosendFeatherSharedSettings: DONE");
}   // END - preptosendFeatherSharedSettings()




