/*
 * setFmx_fns.h
 * 
 */

#ifndef SETFMX_FNS_H
#define SETFMX_FNS_H

#include <Arduino.h>

/* defines */
#define FMX_TXAGTINT_S_NC 0         // value to set it to when no new value needed. NC = No Change.
#define FMX_TXAGTINT_S_MIN 240      // xxx for prod change this to 15m = 900sec
#define FMX_TXAGTINT_S_MAX 86400    // 24hrs = 1440mins = 86,400secs

#define FMX_RXAPINT_S_NC 0         // value to set it to when no new value needed. NC = No Change.
#define FMX_RXAPINT_S_MIN 180      // xxx for prod change this to 5m = 300sec
#define FMX_RXAPINT_S_MAX 3600     // 1hrs = 60mins = 3,600secs

/* define any enums */


/* define any struct's */


/* extern global variables */


/* function pre defines */

void setFmxTxAgtInt(uint32_t txagtint_s);
void setFmxRxApInt(uint32_t rxapint_s);


#endif
// END - setFmx_fns.h



