/*
 * main.cpp
 */

#include "global.h" // My main header file for this project itself

/*============================*/
/* Global object declarations */
/*============================*/
// Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST); // Create TFT instance
Adafruit_SHT31 sht31 = Adafruit_SHT31(); // Create SHT31 instance using default i2c addr of 0x44
// Create Ambient Light Sensor instance
// EN = 13, SDA = 5, SCL = 6 - I am not using EN, its actually hard wired high.
DFRobot_B_LUX_V30B myLux(13, 6, 5); // using default i2c addr of 0x4A
// Create DS18B20 temp sensor
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Create Serial2 - a new hw Serial port I'll use to connect to the AGT.
// Note I am calling it "Serial2", I'm not sure, but when I tried naming it something like "UartToAGT" I had problems tx/rx'ing.
// Define --- SERCOM    RX  TX      RX PAD           TX PAD
Uart Serial2(&sercom0, 19, 18, SERCOM_RX_PAD_2, UART_TX_PAD_0);

// Create Serial3 - a new hw Serial port I'll use to connect to the OpenLog Artemis.
// Define --- SERCOM    RX  TX      RX PAD           TX PAD
Uart Serial3(&sercom3, 11, 12, SERCOM_RX_PAD_3, UART_TX_PAD_0);

// SerialTransfer initialisation
SerialTransfer STdriverF2A; // create a SerialTransfer entity for the Feather to AGT connection.

/*============================*/
/* Global Variables           */
/*============================*/

// Action Flags
bool flag_tx_msg_to_agt = false;                    // Set when we know we need to send a message to the FMX.
bool flag_got_msg_from_agt = false;                 // Set in rx_from_agt() IF we received a msg.
bool flag_got_msg_from_agt_with_mission = false;    // Set in case_process_agt_for_ap() if we have a valid mission for teh AP.
bool flag_tx_msg_to_ap = false;                     // Set when we know we need to send a message to the AP.

// Sensor Status Flags
bool sensor_sht31_status = BAD; // Do we think the sensor is available/working or not?
bool sensor_ambientlight_status = BAD;
bool sensor_ds18b20_status = BAD;

// stuff for my printDebug functionality (most code is in debug_fns.cpp)
bool _printLog = false; // Flag to show if message field log printing is enabled.
Stream *_logSerial;     // The stream to send log messages to (if enabled)

// Create and initialise main state machine vars.
PULSAR_MAIN_SM_STATE main_state = CHECK_POWER; // we always start at this state.
