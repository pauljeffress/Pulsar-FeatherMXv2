/*
 * misc_fns.h
 * 
 */

#ifndef MISC_FNS_H
#define MISC_FNS_H

#include <Arduino.h>

/* defines */

#define AGT_ACTIVATE_PIN    14  // OUTPUT - FMX signals to the AGT's power timer to turn on the AGT.
#define AGT_RX_READY_PIN    10  // INPUT - AGT signals to the FMX that it is on and ready to RX.
                                //         Connected to AGT PIN D4
#define AGT_RX_READY_PIN_WAIT_TIME_S  60    // Seconds - how long the FMX will wait for the AGT to
                                            // raise its AGT_RX_READY_PIN
#define AGT_ACK_WAIT_TIME_S  20    // Seconds - how long the FMX will wait for the AGT to ACK.


/* define any enums */


/* define any struct's */


/* extern global variables */



/* function pre defines */

void setupPins();
String my64toString(uint64_t x);
void set_agt_activate_pin();
void clear_agt_activate_pin();
bool wait_agt_rx_ready_pin();
bool wait_agt_ack();
bool send_ack_to_agt();


#endif
// END - misc_fns.h



