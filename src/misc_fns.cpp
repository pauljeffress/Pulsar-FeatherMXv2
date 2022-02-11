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

}

// Beacuse Arduino print can't handle uint64_t I found this here https://forum.arduino.cc/t/printing-uint64_t/364646
// and it works well.
// Use it as follows;
//    Serial.print(my64toString(<your uint64_t variable>));
String my64toString(uint64_t x)  
{
     boolean flag = false; // For preventing string return like this 0000123, with a lot of zeros in front.
     String str = "";      // Start with an empty string.
     uint64_t y = 10000000000000000000;
     int res;
     if (x == 0)  // if x = 0 and this is not testet, then function return a empty string.
     {
           str = "0";
           return str;  // or return "0";
     }    
     while (y > 0)
     {                
            res = (int)(x / y);
            if (res > 0)  // Wait for res > 0, then start adding to string.
                flag = true;
            if (flag == true)
                str = str + String(res);
            x = x - (y * (uint64_t)res);  // Subtract res times * y from x
            y = y / 10;                   // Reducer y with 10    
     }
     return str;
}