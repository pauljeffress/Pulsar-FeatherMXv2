/*
 * RTC_fns.cpp
 * 
 * Functions that use the external DS3231 RTC module over I2C.
 * 
 * 
 */

#include "global.h"
#include "RTC_fns.h"

// Globals
RTC_DS3231 rtc;
bool RTC_status = BAD;  
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Functions

/*
 * Attempt to initialise the rtc object, and also check if it has lost power previously.
 * Then set global var RTC_status accordingly.
 */
void RTCSetup()
{
    Serial.println("setupRTC() - Starting.");
    if (! rtc.begin()) 
    {
        Serial.println("setupRTC() - ERROR - Couldn't find RTC");
        RTC_status = BAD;
        delay(5000);
    }
    else
    {
        Serial.println("setupRTC() - RTC found ok");
        if (rtc.lostPower()) 
        {
            Serial.println("setupRTC() - ERROR - RTC lost power!");
            RTC_status = BAD;
            delay(5000);
        }
        else
        {
            Serial.println("setupRTC() - RTC time ok");
            RTCPrintCurrentTime();
            RTC_status = GOOD;
        }
    }
    Serial.println("setupRTC() - Complete.");
}   // END - RTCsetup()

/*
 * Read time from RTC and print it to Serial.
 * 
 */
void RTCPrintCurrentTime()
{
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print(" seconds OR ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println(" days");

}   // END - RTCPrintCurrentTime()


