/*
 * SharedSettings.h
 * 
 * header file for my sharedSettings data structures
 * that are used to communicate data over serial link between the FeatherMx and AGT.
 * 
 * Should be identical on FeatherMx and on AGT. As they need to know the structure of each others messages.
 */

#ifndef SHAREDSETTINGS_H
#define SHAREDSETTINGS_H

#include "Arduino.h"    // helps with the "types" used here.

//----------------------------------------------------
// Define the struct for all of my Feather originated settings, that I want to be able to share with the AGT.
// It is based heavily on myFeatherMxSettings.
// Whilst alot of it is duplicated from myFeatherMxSettings, I want a clean copy timestamped and ready to share with AGT.
// It is only ever written to by the FeatherMx, and then shared with the AGT.

typedef struct // FeatherSharedSettings
{
    uint8_t FMX_MAGICNUM;           // sequence number incremented just before sending to AGT. Wraps at 240.
    
    // The following are from the FMX itself
    uint16_t    FMX_BATT_V;             // The battery (Batt socket LiPo) voltage in V * 100
    int16_t     FMX_TEMP;               // The air temperature in degrees C * 100
    int16_t     FMX_RH;                 // The relative humidity in %RH * 100
    uint32_t    FMX_UPTIME_S;           // Uptime in seconds
    uint16_t    FMX_PRESS;              // presure in mbar
    int16_t     FMX_WATERTEMP;          // The water temperature in degrees C * 100
    int16_t     FMX_AMBIENTLIGHT;       // Ambient light reading in lux
    uint32_t    FMX_LAST_AP_HEARTBEAT_S; // Seconds - How many seconds ago the FMX last received a MAVLKink HEARTBEAT from the AP.
    
    // PowerFeather data - received via CAN (all of this can be copied directly from PowerFeatherSettings.h)
    uint16_t    PF_BATT_V;         // The battery (Batt socket LiPo) voltage in V * 100
    int16_t     PF_TEMP;           // The air temperature in degrees C * 100
    int16_t     PF_RH;             // The relative humidity in %RH * 100
    uint32_t    PF_UPTIME_S;       // Uptime in seconds
    // CHARGER 1
    // storage for values read from charger
    uint16_t    PF_CHARGER1_PID;                // Victron Product ID (stored as 0xXXXX)
    uint16_t    PF_CHARGER1_FW;                 // Firmware version (16 bit)
    //char        PF_CHARGER1_SER[12];            // Serial number (string of 11 alpha + /0)    // Not using, too awkward and not needed.
    uint16_t    PF_CHARGER1_V;                  // Main or channel 1 battery voltage (mV)
    int16_t     PF_CHARGER1_I;                  // Main or channel 1 battery current (mA)
    uint16_t    PF_CHARGER1_VPV;                // Panel voltage (mV)
    uint16_t    PF_CHARGER1_PPV;                // Panel power (W)
    uint8_t     PF_CHARGER1_CS;                 // State of operation (enum)
    uint8_t     PF_CHARGER1_ERR;                // Error code (enum)
    uint8_t     PF_CHARGER1_LOAD;               // Load output state (ON/OFF)
    int16_t     PF_CHARGER1_IL;                 // Load current (mA)
    uint16_t    PF_CHARGER1_H19;                // Yield total, user resettable counter (0.01kWh)
    uint16_t    PF_CHARGER1_H20;                // Yield today (0.01kWh)
    uint16_t    PF_CHARGER1_H21;                // Maximum power today (W)
    uint16_t    PF_CHARGER1_H22;                // Yield yesterday (0.01kWh)
    uint16_t    PF_CHARGER1_H23;                // Maximum power yesterday (W)
    uint16_t    PF_CHARGER1_HSDS;               // Day sequence number (0..364)
    uint8_t     PF_CHARGER1_MPPT;               // Tracker operation mode (enum)
    // CHARGER 2
    // storage for values read from charger
    uint16_t    PF_CHARGER2_PID;                // Victron Product ID (stored as 0xXXXX)
    uint16_t    PF_CHARGER2_FW;                 // Firmware version (16 bit)
    //char        PF_CHARGER2_SER[12];            // Serial number (string of 11 alpha + /0)    // Not using, too awkward and not needed.
    uint16_t    PF_CHARGER2_V;                  // Main or channel 1 battery voltage (mV)
    int16_t     PF_CHARGER2_I;                  // Main or channel 1 battery current (mA)
    uint16_t    PF_CHARGER2_VPV;                // Panel voltage (mV)
    uint16_t    PF_CHARGER2_PPV;                // Panel power (W)
    uint8_t     PF_CHARGER2_CS;                 // State of operation (enum)
    uint8_t     PF_CHARGER2_ERR;                // Error code (enum)
    uint8_t     PF_CHARGER2_LOAD;               // Load output state (ON/OFF)
    int16_t     PF_CHARGER2_IL;                 // Load current (mA)
    uint16_t    PF_CHARGER2_H19;                // Yield total, user resettable counter (0.01kWh)
    uint16_t    PF_CHARGER2_H20;                // Yield today (0.01kWh)
    uint16_t    PF_CHARGER2_H21;                // Maximum power today (W)
    uint16_t    PF_CHARGER2_H22;                // Yield yesterday (0.01kWh)
    uint16_t    PF_CHARGER2_H23;                // Maximum power yesterday (W)
    uint16_t    PF_CHARGER2_HSDS;               // Day sequence number (0..364)
    uint8_t     PF_CHARGER2_MPPT;               // Tracker operation mode (enum)


    // the following relate to MAVLINK_MSG_ID_HEARTBEAT (#0) packets
    uint8_t     AP_BASEMODE;         // System mode bitmap.
    uint32_t    AP_CUSTOMMODE;      // A bitfield for use for autopilot-specific flags
    uint8_t     AP_SYSTEMSTATUS;    //  System status flag.
    
    // the following relate to AP data via MAVLINK_MSG_ID_GLOBAL_POSITION_INT ( #33 )
    uint32_t    AP_POSITIONTIMESTAMP;       //  Timestamp (time since system boot).
    int32_t     AP_LAT;     //  Latitude, expressed
    int32_t     AP_LON;     //  Longitude, expressed
    int16_t     AP_VX;      //  Ground X Speed (Latitude, positive north)
    int16_t     AP_VY;      //  Ground Y Speed (Longitude, positive east)
    uint16_t    AP_HDG;     //  Vehicle heading (yaw angle), 0.0..359.99 degrees. If unknown, set to: UINT16_MAX

    // the following relate to AP data via MAVLINK_MSG_ID_GPS_RAW_INT ( #24 )
    uint8_t     AP_SATS;     //  Number of satellites visible. If unknown, set to UINT8_MAX
    uint16_t    AP_VEL;      //  GPS ground speed. If unknown, set to: UINT16_MAX
    uint16_t    AP_COG;      //  Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX

    // the following relate to AP data via MAVLINK_MSG_ID_POWER_STATUS ( #125 )
    uint16_t    AP_VCC;          //  5V rail voltage.
    uint16_t    AP_VSERVO;       //  Servo rail voltage.
    uint16_t    AP_POWERFLAGS;   //  Bitmap of power supply status flags.

    // the following relate to AP data via MAVLINK_MSG_ID_SYS_STATUS ( #1 )
    uint32_t    AP_SENSORSPRESENT;       //  Bitmap showing which onboard controllers and sensors are present. Value of 0: not present. Value of 1: present.
    uint32_t    AP_SENSORSENABLED;       //  Bitmap showing which onboard controllers and sensors are enabled: Value of 0: not enabled. Value of 1: enabled.
    uint32_t    AP_SENSORSHEALTH;        //  Bitmap showing which onboard controllers and sensors have an error (or are operational). Value of 0: error. Value of 1: healthy.
    uint16_t    AP_LOAD;                 //  Maximum usage in percent of the mainloop time. Values: [0-1000] - should always be below 1000

    // the following relate to AP data via MAVLINK_MSG_ID_SYSTEM_TIME ( #2 )
    uint64_t    AP_TIMEUNIXUSEC;     //  Timestamp (UNIX epoch time). The system time is the time of the master clock, typically the computer clock of the main onboard computer.
    uint32_t    AP_TIMEBOOTMS;       //  Timestamp (time since system boot).

    // the following relate to AP data via MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT ( #62 )
    int16_t     AP_NAVBEARING;       //  Nav controller - Current desired heading
    int16_t     AP_TARGETBEARING;    //  Nav controller - Bearing to current waypoint/target
    uint16_t    AP_WPDIST;           //  Nav controller - Distance to active waypoint

    // the following relate to AP data via MAVLINK_MSG_ID_BATTERY_STATUS ( #147 )
    uint16_t        AP_VOLTAGES[10];             //  Battery voltage of cells 1 to 10 (see voltages_ext for cells 11-14). Cells in this field above the valid cell count for this battery should have the UINT16_MAX value. If individual cell voltages are unknown or not measured for this battery, then the overall battery voltage should be filled in cell 0, with all others set to UINT16_MAX. If the voltage of the battery is greater than (UINT16_MAX - 1), then cell 0 should be set to (UINT16_MAX - 1), and cell 1 to the remaining voltage. This can be extended to multiple cells if the total voltage is greater than 2 * (UINT16_MAX - 1).
    int16_t         AP_CURRENTBATTERY;       //  Battery current, -1: autopilot does not measure the current

    // the following relate to AP data via MAVLINK_MSG_ID_AUTOPILOT_VERSION ( #148 )
    uint16_t    AP_VENDORID;     // ID of the board vendor
    uint16_t    AP_PRODUCTID;    // ID of the product



} FeatherSharedSettings;
//----------------------------------------------------


//----------------------------------------------------
// Define the struct for all of my AGT originated settings, that I want to be able to share with the FEATHER.
// It is based heavily on mytrackerSettings.
// Whilst alot of it is duplicated from mytrackerSettings, I want a clean copy timestamped and ready to share with FEATHER.
// It is only ever written to by the AGT, and then shared with the FEATHER.

typedef struct  // AgtSharedSettings
{
    uint8_t MAGICNUM;    // sequence number incremented just before sending to Feather. Wraps at 240.
    
    // The following are populated by the sensors directly connected to the AGT
    uint16_t BATTV;         // The battery (bus) voltage in V * 10^-2
    uint16_t PRESS;         // The pressure in mbar
    int16_t  TEMP;          // The temperature in degrees C * 10^-2
    uint16_t HUMID;         // The humidity in %RH * 10^-2

    // the time provided by the GPS when the GPS position was taken.
    uint16_t GPSYEAR;     // UTC year
    uint8_t  GPSMONTH;    // UTC month
    uint8_t  GPSDAY;      // UTC day
    uint8_t  GPSHOUR;     // UTC hour
    uint8_t  GPSMIN;      // UTC minute
    uint8_t  GPSSEC;      // UTC seconds
    uint16_t GPSMILLIS;      // UTC milliseconds

    // the following are populated by the AGT's onboard GPS
    int32_t LAT;           // Latitude in Degrees * 10^-7
    int32_t LON;           // Latitude in Degrees * 10^-7
    int32_t ALT;           // Altitude above MSL in mm
    int32_t SPEED;         // Ground speed in mm/s
    int32_t HEAD;          // The heading in Degrees * 10^-7
    uint8_t SATS;          // The number of satellites (space vehicles) used in the solution
    uint16_t PDOP;         // The Positional Dilution of Precision in cm
    uint8_t FIX;           // The gps fix type as defined in the u-blox PVT message
} AgtSharedSettings;
//----------------------------------------------------

#endif