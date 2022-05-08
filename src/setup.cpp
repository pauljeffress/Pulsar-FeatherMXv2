/*
 * setup.ino
 */

#include "global.h"

/*============================
 * setup()
 *
 * Note: setup() code only runs when CPU has done a real powerup (or HW RESET), NOT a wake from sleep.
 ============================*/
void setup()
{
    myCANid = CBP_CANID_FEATHERMX; // Set myCANid based on defines in CBP.h

    setupPins(); // initialise all GPIOs    
    
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);    
    
    disableDebugging(); // Make sure the serial debug messages are disabled until the Serial port is open ( see loop_init() )!
    disableLogging(); // Make sure the serial logging messages (to OLA) are disabled until the Serial port is open ( see loop_init() )! 
    serialSetup();  // Setup all Serial ports   
    enableDebugging(Serial); // THIS LINE IS RIGHT HERE FOR A REASON. Because we re issue Serial.begin() for the console/debug port
                             // just a few lines earlier here in serialSetup(), you need to enable the debug stuff that uses it
                             // after you have done that.  For more info see case_zzz(), and you will see it explicitly does a
                             // Serial.end(); before putting the processor to sleep.
                             // Uncomment this line to enable extra debug messages to Serial    
    
    debugPrintln("================================================");
    debugPrintln("************************************************");
    debugPrintln("================================================");
    debugPrintln("        Pulsar FeatherMx Reset/PowerOn          ");   
    enableLogging(Serial3); // THIS LINE IS RIGHT HERE FOR A REASON. See explanation above for enableDebugging(Serial);

    
    logPrintln("================================================");
    logPrintln("************************************************");
    logPrintln("================================================");
    logPrintln("        Pulsar FeatherMx Reset/PowerOn          "); 
    
    initFmxSettings();        // Initialise the myFmxSettings, from defaults or otherwise.
    zeroFmxSharedSettings();  // Zero out the myFmxSharedSettings, so it is cleanly initialised.
    zeroAgtSharedSettings();  // Zero out the myAgtSharedSettings, so it is cleanly initialised.
    initTFTFeatherInternalSettings();
    initPowerFeatherSettings();
    initMAVLinkSettings();  
    
    RTCSetup(); 
    CANStatus = CANSetup();     
    timerSetup();   
    actuatorsSetup();   
    sensorsSetup(); 
    
    mavlink_unrequest_streaming_params_from_ap(); // I am trying to initially hush the AutoPilot (see my other mavlink stuff for explanation)   
    
    flag_tx_msg_to_agt = false; // when set, tx_to_agt() will send the full standard
                            // Feather to AGT  blob of data to the AGT, and
                            // the AGT will decide what to do with it.  
    main_state = CHECK_POWER;    // Ensure main state machine starts at correct first step.
    
    debugPrintln("setup() - complete.");
} // END - setup()