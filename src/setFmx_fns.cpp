/*
 * setFmx_fns.ino
 * 
 * Functions that safely set parameters on this FMX.
 * 
 * 
 */

#include "global.h"

#include "setFmx_fns.h"

/*
 * Bounds check, set & save FMX's TXAGTINT (seconds)
 */
void setFmxTxAgtInt(uint32_t txagtint_s)
{
    if (txagtint_s != FMX_TXAGTINT_S_NC)  // only proceed if we have a new setting that should be applied
    {
        // Bounds check & fix
        if (txagtint_s < FMX_TXAGTINT_S_MIN)    txagtint_s = FMX_TXAGTINT_S_MIN;
        if (txagtint_s > FMX_TXAGTINT_S_MAX)    txagtint_s = FMX_TXAGTINT_S_MAX;

        // Action it
        debugPrint("setFmxTxAgtInt() - setting TXAGTINT=");debugPrintlnInt(txagtint_s);
        myFmxSettings.FMX_TXAGTINT = txagtint_s;    // update the global

        // Note - we don't need to reset any timers just because we have a new value for TXAGTINT,
        // because it is directly evaluated everytime its checked.
    }
    else
    {
        debugPrint("setFmxTxAgtInt() - Not setting TXAGTINT as it = FMX_TXAGTINT_S_NC");
    }
}   // END - setFmxTxAgtInt()


/*
 * Bounds check, set & save FMX's RXAPINT (seconds)
 */
void setFmxRxApInt(uint32_t rxapint_s)
{
    if (rxapint_s != FMX_RXAPINT_S_NC)  // only proceed if we have a new setting that should be applied
    {
        // Bounds check & fix
        if (rxapint_s < FMX_RXAPINT_S_MIN)    rxapint_s = FMX_RXAPINT_S_MIN;
        if (rxapint_s > FMX_RXAPINT_S_MAX)    rxapint_s = FMX_RXAPINT_S_MAX;

        // Action it
        debugPrint("setFmxRxApInt() - setting RXAPINT=");debugPrintlnInt(rxapint_s);
        myFmxSettings.FMX_RXAPINT = rxapint_s;    // update the global

        // Note - we don't need to reset any timers just because we have a new value for RXAPINT,
        // because it is directly evaluated everytime its checked.
    }
    else
    {
        debugPrint("setFmxRxApInt() - Not setting RXAPINT as it = FMX_RXAPINT_S_NC");
    }
}   // END - setFmxRxApInt()