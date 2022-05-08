/*
 * misc_fns.ino
 *
 * Assorted functions
 *
 *
 */

#include "global.h"

void setupPins()
{
    debugPrintln("setupPins()");

    pinMode(LED_BUILTIN, OUTPUT); // Make the LED pin an output

    // setup the 12v switched power output GPIO control pins.
    pinMode(PI_PWR_PIN, OUTPUT);
    digitalWrite(PI_PWR_PIN, LOW);
    pinMode(STROBE_LIGHT_PWR_PIN, OUTPUT);
    digitalWrite(STROBE_LIGHT_PWR_PIN, LOW);
    pinMode(POWER_FEATHER_PWR_PIN, OUTPUT);
    digitalWrite(POWER_FEATHER_PWR_PIN, LOW);

    // AGT control/monitoring pins
    pinMode(AGT_ACTIVATE_PIN, OUTPUT);
    digitalWrite(AGT_ACTIVATE_PIN, LOW);
    pinMode(AGT_RX_READY_PIN, INPUT);
}

// Because Arduino print can't handle uint64_t I found this here https://forum.arduino.cc/t/printing-uint64_t/364646
// and it works well.
// Use it as follows;
//    Serial.print(my64toString(<your uint64_t variable>));
String my64toString(uint64_t x)
{
    boolean flag = false; // For preventing string return like this 0000123, with a lot of zeros in front.
    String str = "";      // Start with an empty string.
    uint64_t y = 10000000000000000000;
    int res;
    if (x == 0) // if x = 0 and this is not testet, then function return a empty string.
    {
        str = "0";
        return str; // or return "0";
    }
    while (y > 0)
    {
        res = (int)(x / y);
        if (res > 0) // Wait for res > 0, then start adding to string.
            flag = true;
        if (flag == true)
            str = str + String(res);
        x = x - (y * (uint64_t)res); // Subtract res times * y from x
        y = y / 10;                  // Reducer y with 10
    }
    return str;
}

/*
 *  Functions to control the signal/PIN to powerup the AGT
 */
void set_agt_activate_pin()
{
    digitalWrite(AGT_ACTIVATE_PIN, HIGH);
}

void clear_agt_activate_pin()
{
    digitalWrite(AGT_ACTIVATE_PIN, LOW);
}

bool wait_agt_rx_ready_pin()
{
    bool result = false;   // track success/failure of waiting to see agt_rx_ready_pin come HIGH.
    bool timedOut = false; // track when we have timed out.
    uint32_t start = 0;

    debugPrintln("wait_agt_rx_ready_pin() - waiting to see if AGT_RX_READY_PIN goes HIGH or else timeout");

    start = millis(); // take a millis() timestamp when we start.

    while (!timedOut && !result) // do this until we either succeed or we time out.
    {
        result = digitalRead(AGT_RX_READY_PIN); // check status of AGT's PIN
        if (result)
            debugPrintln("wait_agt_rx_ready_pin() - SUCCESS - AGT_RX_READY_PIN went HIGH");

        // Check if we have timed out, which will be evaluated at next pass through the while()
        if ((millis() - start) >= (AGT_RX_READY_PIN_WAIT_TIME_S * 1000))
        {
            timedOut = true;
            debugPrintln("wait_agt_rx_ready_pin() - WARNING - we timed out before seeing AGT_RX_READY_PIN go HIGH");
        }
    } // END - while()

    if (timedOut)
        return (false);
    else
        return (result);
} // END - wait_agt_rx_ready_pin()

/*
 *  Called to send an ACK msg back to the AGT when the FMX has received a msg from the AGT.
 *
 */
bool send_ack_to_agt()
{
    debugPrint("send_ack_to_agt() - acki'ing the AGT for rcvd AGT msg with AGT_MAGICNUM:");debugPrintlnInt(myAgtSharedSettings.AGT_MAGICNUM);

    bool success = false;

    // // Even though we are about to send an ACK, we always include the latest data we have as well.
    // // This approach makes it easier on the receiveing side because the receiver can process the message
    // // quite happily even if it is not looking for the ACK fields. It helps make the whole exchange a bit
    // // more robust.
    // preptosendFmxSharedSettings(); // gets myFmxSharedSettings fresh and ready to TX to AGT
    // Do additional message prep for this ACK message.

    zeroFmxSharedSettings();            // prepare a zero'd struct, as all we want in it is the two ACK related fields.

    setFmxSharedSettingNextMagicnum();  // All msgs we send must have a fresh MAGICNUM.

    myFmxSharedSettings.FMX_ACK_FOR_AGT = 1;                                 // 1 = ACK, 0 = nil.
    myFmxSharedSettings.FMX_ACK_MAGICNUM = myAgtSharedSettings.AGT_MAGICNUM; // Copy the MAGICNUM from the most recently received msg from AGT
                                                                             // as that is the message we are ACK'ing.

    // Send the Datum to peer
    debugPrint("case_ack_to_agt() - Sending ACK Datum to AGT. My MAGICNUM=");debugPrintlnInt(myFmxSharedSettings.FMX_MAGICNUM);
    uint8_t numbytessent = STdriverF2A.sendDatum(myFmxSharedSettings);
    debugPrint("case_ack_to_agt() - FMX to AGT transfer finished - numbytessent:");debugPrintlnInt(numbytessent);  //debugPrintlnInt(numbytessent);

    seconds_since_last_agt_tx = 0; // reset this counter as we have sent....even though AGT may not have received.

    return (success);
} // END - send_ack_to_fmx()

/*
 *  wait_agt_ack()
 *
 *  We wait for a set period of time for an ACK msg from the AGT, else we timeout.
 *  We only use the ACK/MAGICNUM contents of the msg, no other fields are even looked at.
 */
bool wait_agt_ack()
{
    bool result = false;   // track success/failure of waiting to see agt_rx_ready_pin come HIGH.
    bool timedOut = false; // track when we have timed out.
    uint32_t start = 0;

    debugPrintln("wait_agt_ack() - waiting for AGT to ACK our TX or else timeout");

    start = millis(); // take a millis() timestamp when we start.

    while (!timedOut && !result) // do this until we either succeed or we time out.
    {
        // Check if the AGT has sent us anything
        // Use SerialTransfer receive code to get anything from our peer.
        int numbytesexpected = sizeof(myAgtSharedSettings);
        // Serial.print("   ...");Serial.println(numbytesexpected);
        if (STdriverF2A.available() > (numbytesexpected - 1)) // is a full ST packet available?
        {
            STdriverF2A.rxObj(myAgtSharedSettings); // read in the ST packet and store it.
            seconds_since_last_agt_rx = 0;          // reset the counter as we have successfully received a packet from the AGT
            debugPrintln("wait_agt_ack() - FMX received a Datum from the AGT");

            if ((myAgtSharedSettings.AGT_ACK_FOR_FMX == 1)                                     // 1 = ACK, 0 = nil.
                && (myAgtSharedSettings.AGT_ACK_MAGICNUM == myFmxSharedSettings.FMX_MAGICNUM)) // is the ACK for the packet we expected it for?
            {
                result = true;
                debugPrintln("wait_agt_ack() - SUCCESS - It's an ACK for the correct MagicNum packet");
            }
            else
            {
                result = false;
                debugPrintln("wait_agt_ack() - WARNING - It's either not an ACK or not for the correct MagicNum packet");
            }
        }

        // Check if we have timed out, which will be evaluated at next pass through the while()
        if ((millis() - start) >= (AGT_ACK_WAIT_TIME_S * 1000))
        {
            timedOut = true;
            debugPrintln("wait_agt_ack() - WARNING - we timed out");
        }
    } // END - while()

    if (timedOut)
        return (false);
    else
        return (result);
} // END - wait_agt_ack()
