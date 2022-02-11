/*
 * serial_fns.cpp
 */

#include "global.h" // My main header file for this project itself

// Part of the new hw Serial ports I create on the Feather to talk to the AGT and the Oplen Log Artemis.
// The adafruit document said for SAMD51 I needed to define all 5 interupt handlers for each SERCOM (e.g. SERCOM0, SERCOM1 etc)
void SERCOM0_0_Handler()
{
  Serial2.IrqHandler();
}
void SERCOM0_1_Handler()
{
  Serial2.IrqHandler();
}
void SERCOM0_2_Handler()
{
  Serial2.IrqHandler();
}
void SERCOM0_3_Handler()
{
  Serial2.IrqHandler();
}

void SERCOM3_0_Handler()
{
  Serial3.IrqHandler();
}
void SERCOM3_1_Handler()
{
  Serial3.IrqHandler();
}
void SERCOM3_2_Handler()
{
  Serial3.IrqHandler();
}
void SERCOM3_3_Handler()
{
  Serial3.IrqHandler();
}

void serialSetup()
{
  /*
   * Serial - setup the serial port the is via USB for concole/SerialMonitor etc.
   */
  Serial.begin(115200); // Start the console serial port
  delay(2000); // ensure time for the Serial port to get ready.
  

  /*
   * Serial1 - setup the serial port between Feather and Cube Orange port for MAVLink telemetry
   */  
  Serial1.begin(57600); //RXTX from Telem Radio (Pins RX1 & TX1 on Feather M4)
  
  /*
   * Serial2 - setup the serial port between Feather & AGT for SatComms.
   */
  // Initialise the Serial that connects this Feather to the AGT
  Serial2.begin(57600); // AGT end is using SoftwareSerial, so go slower speed.
  // Reassign pins on the internal SAMD pinmux, to connect to my SERCOMs. They may have defaulted to other peripherals.
  // Assign pins 18 & 19 SERCOM functionality. Must happen after the SerialX.begin(xxxx) command.
  pinPeripheral(18, PIO_SERCOM_ALT);    // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
  pinPeripheral(19, PIO_SERCOM_ALT);    // same as above comment.

  // Initialise SerialTransfer driver for Feather to AGTconnection
  //                Serial Port, debug on, Debug output port, timeout in mS
  STdriverF2A.begin(Serial2,     true,     Serial,            200);

  // prep datum for first use
  //STDatumTX.i1 = 1;
  //STDatumTX.c1 = 'F';
  //STDatumTX.c2 = 't';
  //STDatumTX.c3 = 'o';
  //STDatumTX.c4 = 'A';


  /*
   *  Serial 3 - setup the serial logging port between Feather & OpenLog Artemis for logging.
   */
  // Initialise the Serial that connects this Feather to the OpenLog Artemis
  Serial3.begin(57600); 
  // Reassign pins on the internal SAMD pinmux, to connect to my SERCOMs. They may have defaulted to other peripherals.
  // Assign pins 12 & 11 SERCOM functionality. Must happen after the SerialX.begin(xxxx) command.
  pinPeripheral(12, PIO_SERCOM);    // the 'PIO_SERCOM' should be 'PIO_SERCOM_ALT' if we are trying to use the 'alternate' pins for this.
  pinPeripheral(11, PIO_SERCOM_ALT);    // same as above comment.

}  // END - serialSetup()