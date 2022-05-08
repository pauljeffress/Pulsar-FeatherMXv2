/*
 * case_read_sensors.cpp
 * 
 */

#include "global.h"

void case_read_sensors()
{

    //debugPrintln("case_read_sensors() - executing");

    // is it time to do a routine sensor read?
    if (seconds_since_last_sensors_read > SENSORPERIODSECONDS)
    {
        seconds_since_last_sensors_read = 0;    // Reset this timer.
        
        if (sensor_sht31_status)    // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the SHT31 Temp & Humidity Sensor 
             * ----------------------------------
             */

            // TODO - are we supposed to turn on the SHT31's heater before taking a reading????

            //debugPrintln("case_read_sensors() - SHT31");

            float t = sht31.readTemperature();
            float h = sht31.readHumidity();

            if (!isnan(t))
            { // check if 'is not a number'
                myFmxSettings.FMX_TEMP = t * 100; // * 100 to store as correct units in Pulsar project.
                //debugPrint("case_read_sensors() - Air Temp (degC): ");
                //debugPrintlnFlt(t);
            }
            else
            {
                myFmxSettings.FMX_TEMP = 6666; // set to an obvious failed magic number
                sensor_sht31_status = BAD;  // As soon as we detect an issue, stop using the device.
                debugPrintln("case_read_sensors() - ERROR Air Temp sensor read failed");
            }

            if (!isnan(h))
            { // check if 'is not a number'
                myFmxSettings.FMX_RH = h * 100; // * 100 to store as correct units in Pulsar project.
                //debugPrint("case_read_sensors() - Air Humidity (%): ");
                //debugPrintlnFlt(h);
            }
            else
            {
                myFmxSettings.FMX_RH = 6666; // set to an obvious failed magic number
                debugPrintln("case_read_sensors() - ERROR - Air Humidity sensor read failed");
            }
        }   // END - if (sensor_sht31_status)

        if (sensor_ds18b20_status)    // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the DS18B20 Temp Sensor 
             * ----------------------------------
             */
            //debugPrintln("case_read_sensors() - DS18B20");
            sensors.requestTemperatures();            // Send the command to get temperatures for all DS18B20's on the I2C bus.
            float tempC = sensors.getTempCByIndex(0); // We use the function ByIndex, and as an example get the temperature from the first sensor only.
            if (tempC != DEVICE_DISCONNECTED_C)       // check if its a valid reading
            {
                myFmxSettings.FMX_WATERTEMP = tempC * 100; // * 100 to store as correct units in Pulsar project.
                //debugPrint("case_read_sensors() - Water Temp (degC): ");
                //debugPrintlnFlt(tempC);
            }
            else
            {
                myFmxSettings.FMX_WATERTEMP = 6666; // set to an obvious failed magic number
                sensor_ds18b20_status = BAD;  // As soon as we detect an issue, stop using the device.
                debugPrintln("case_read_sensors() - ERROR - Water Temp sensor read failed");
            }
        }   // END - if (sensor_ds18b20_status)

        if (sensor_ambientlight_status)    // don't try to read the sensor once its marked bad.  The flag will be reset when Feather is reset/power-cycled.
        {
            /*
             * ----------------------------------
             * Operate the Ambient Light Sensor 
             * ----------------------------------
             */
            //debugPrintln("case_read_sensors() - AmbientLight");
            float lux = myLux.lightStrengthLux();
            if (lux != DEVICE_DISCONNECTED_C) // check if its a valid reading
            {
                myFmxSettings.FMX_AMBIENTLIGHT = lux;
                //debugPrint("case_read_sensors() - Ambient Light (Lux): ");
                //debugPrintlnFlt(lux);
            }
            else
            {
                myFmxSettings.FMX_AMBIENTLIGHT = 6666; // set to an obvious failed magic number
                sensor_ambientlight_status = BAD; // As soon as we detect an issue, stop using the device.
                debugPrintln("case_read_sensors() - ERROR - Ambient Light sensor read failed");
            }
        }   // END - if (sensor_ambientlight_status)        
    }
    else  // is it time to read sensors?
    {
        //debugPrintln(" - NOT NOW");
    }

    //debugPrintln("case_read_sensors() - done");
}