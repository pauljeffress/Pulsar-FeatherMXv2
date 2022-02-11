/*
 * sensor_fns.cpp
 */

#include "global.h" // My main header file for this project itself

void sensorsSetup()
{
  debugPrintln("sensorsSetup() - executing");
  
  // Setup the SHT31 sensor
  debugPrintln("sensorsSetup() - SHT31 initialising");
  if (!sht31.begin(0x44)) // Set to 0x45 for alternate i2c addr
  {
    // xxx - Even when the SHT31 is not connected, the begin still seems to respond true 
    // and hence this code here does not get run! Whats worse is that if the sensor is disconnected
    // and I guess if it is connected but failing, when we get to the sht31.isHeaterEnabled() further down
    // the sketch hangs!!!!!!!    
    debugPrintln("sensorsSetup() - ERROR - Failed");
    sensor_sht31_status = BAD;
  }
  else
  {
    debugPrintln("sensorsSetup() - OK");
    sensor_sht31_status = GOOD;
    if (sht31.isHeaterEnabled())
      debugPrintln("sensorsSetup() - Heater ENABLED");
    else
      debugPrintln("sensorsSetup() - Heater DISABLED");
  }
  

  // setup ambient light sensor dome
  debugPrintln("sensorsSetup() - AmbientLight initialising");
  float lux;
  // xxx myLux.begin();    // TODO - there is a potential dead end inside this begin code "while(lightStrengthLux()<=0);" in that library!!
  // float lux = myLux.lightStrengthLux();
  if (lux == -1) // the read failed
  {
    debugPrintln("sensorsSetup() - ERROR - Failed");
    sensor_ambientlight_status = BAD;
  }
  else  // the read succeeded
  {
    debugPrintln("sensorsSetup() - OK");
    sensor_ambientlight_status = GOOD;
  }




  // Start up the DallasTemperature library
  debugPrintln("sensorsSetup() - DS18B20 Temp initialising");
  sensors.begin();
  // locate devices on the bus
  uint8_t devcount = sensors.getDeviceCount();
  if (devcount < 1 ) // the dev count failed
  {
    debugPrintln("sensorsSetup() - ERROR - Failed");
    sensor_ds18b20_status = BAD;
  }
  else  // the read succeeded
  {
    debugPrintln("sensorsSetup() - OK");
    sensor_ds18b20_status = GOOD;
  }  

  // report parasite power requirements
  if (sensors.isParasitePowerMode())
    debugPrintln("sensorsSetup() - Parasite Power ENABLED");
  else
    debugPrintln("sensorsSetup() - Parasite Power DISABLED");
}

