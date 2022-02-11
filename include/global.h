/*
 * global.h
 * 
 * Overall header file for this project
 * 
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include "wiring_private.h"    // for "pinPeripheral()". Only needed in PlatformIO, not required in Arduino IDE???
#include <Wire.h>
#include <SPI.h>
#include "Adafruit_GFX.h"                       // For TFT
#include "Adafruit_HX8357.h"                    // For TFT
#include <Fonts/FreeMonoBoldOblique12pt7b.h>    // For TFT
#include <Fonts/FreeSans18pt7b.h>               // For TFT
#include "Adafruit_SHT31.h"         // for my DFRobot weatherproof Temp & Humidity I2C sensor (I'm not using the DFRobot lib)
#include <DFRobot_B_LUX_V30B.h>     // DFRobot Visible Light sensor
#include <OneWire.h>                // required by DallasTemperature library
#include <DallasTemperature.h>      // required for DS18B20 temp sensor

// Below mavlink includes draw from the c_library_v1 library I added to this PlatformIO project.
//#include <common/mavlink.h> // The Mavlink library for the "common" dialect  // Must remove this line now that I have the
                                                                               // ardupilotmega dialect below, as it pulls 
                                                                               // in common itself.
#include <ardupilotmega/mavlink.h> // The Mavlink library for the "ardupilotmega" dialect

#include <TimeLib.h> // The PJRC TimeLib library to help me deal with Unix Epoch time from GPS.

#include <SerialTransfer.h> // Note: This library will complain about SPDR in the SPITransfer.cpp/h files at compile time.
                            //       Its a known problem, search my Evernotes. The solution as I am not using SPI under
                            //       SerialTransfer is to rename two of the source files in the SerilTransfer library so
                            //       they are not compiled.  The two files are;
                            //       * SPITransfer.h and SPITransfer.cpp
                            //       they live in .pio/libdeps/..../SerialTransfer/src/

#include "SharedSettings.h" // header file for my SharedSettings data structures

#include <PulsarCommon.h>

#include "mavlink_fns.h"    // my header file for my own mavlink functions.


/*=======================*/
/* #DEFINES              */
/*=======================*/
#define HOST_IS_FEATHERMX    // used to select what CAN/CBP packets to decode

// DS18B20 sensor - Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 4

// 12v switched power outputs
#define PI_PWR_PIN              23 // The pin the FMX controls the power supply to the Pi on.
#define STROBE_LIGHT_PWR_PIN    24 // The pin the FMX controls the power supply to the Strobe light on.
#define POWER_FEATHER_PWR_PIN   25 // The pin the FMX controls the power supply to the Power Feather on.

/// Various Timers
#define SENSORPERIODSECONDS             120 // seconds - how often should we read the sensors?
#define MAVLINKHEARTBEATPERIODSECONDS   20  // seconds - how often should we send a MAVLink HEARTBEAT to the AutoPilot
#define TX_TO_AP_PERIOD_SECONDS         120  // seconds - how often should we do periodic TX to AutoPilot


/*=======================*/
/* define any enums      */
/*=======================*/

typedef enum PULSAR_MAIN_SM_STATE     // State machine states for the main state machine running in loop()
{
    CHECK_POWER,
    READ_SENSORS,
    TX_TO_CANBUS,
    CHECK_CANBUS,
    HEARTBEAT_TO_AP,
    RX_FROM_AP,
    PROCESS_AP,
    TX_TO_AP,
    RX_FROM_AGT,
    PROCESS_AGT,
    TX_TO_AGT,
    TX_TO_LOGGER,
} PULSAR_MAIN_SM_STATE;

/*=======================*/
/* define any struct's   */
/*=======================*/

/*=======================*/
/* extern my global vars */
/*=======================*/
extern Adafruit_HX8357 tft;
extern Adafruit_SHT31 sht31;
extern DFRobot_B_LUX_V30B myLux;
extern OneWire oneWire;
extern DallasTemperature sensors;
extern Uart Serial2;
extern Uart Serial3;
extern SerialTransfer STdriverF2A;


extern FeatherSharedSettings myFeatherSharedSettings;
extern AgtSharedSettings myAgtSharedSettings;

extern bool flag_do_agt_tx;

extern bool sensor_sht31_status;
extern bool sensor_ambientlight_status;
extern bool sensor_ds18b20_status;

extern bool _printLog;
extern Stream *_logSerial;

extern PULSAR_MAIN_SM_STATE  main_state;

extern GlobalMission_t global_mission;

/*============================*/
/* Function Predefines        */
/*============================*/
void actuatorsSetup();
void sensorsSetup();
void sensorsTest();

void setupPins();

void serialSetup();

void mavlink_fmx_send_heartbeat_to_ap();

void mavlink_request_datastream();
void mavlink_unrequest_datastream();
void mavlink_request_streaming_params_from_ap();
void mavlink_unrequest_streaming_params_from_ap();

void request_one_param_from_ap();
void set_one_param_from_ap(); 
void set_arm_ap();
void set_disarm_ap();
void set_flightmode_ap(float desired_flightmode);

void mavlink_receive();

void case_loop_init();
void case_zzz();
void case_wake();
void case_assess_situation();
void case_read_sensors();
void case_tx_to_CANbus();
void case_write_to_tft();
void case_heartbeat_to_autopilot();
void case_rx_from_autopilot();
void case_process_autopilot();
void case_tx_to_autopilot();
void case_rx_from_agt();
void case_process_agt();
void case_tx_to_agt();
void case_tx_to_logger();
void case_tickle_watchdog();
void case_sleep_yet();

bool sendSharedSettings_to_AGT(void);
void initFeatherSharedSettings(void);
void preptosendFeatherSharedSettings(void);

void enableLogging(Stream &logPort);
void disableLogging(void);
void logPrintStamp(void);
void logPrint(const char *message);
void logPrintln(const char *message);
void logPrintInt(int32_t number);
void logPrintlnInt(int32_t number);
void logPrintFlt(float number);
void logPrintlnFlt(float number);

int16_t int32_to_int16_a(int32_t input_int32);
uint16_t int32_to_int16_b(int32_t input_int32);
uint16_t int32_to_int16_c(int32_t input_int32);

String my64toString(uint64_t x);

void actuatorStrobeOn();
void actuatorStrobeOff();
void actuatorPiOn();
void actuatorPiOff();
void actuatorPowerFeatherOn();
void actuatorPowerFeatherOff();


#endif