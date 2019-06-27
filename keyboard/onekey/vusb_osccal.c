// V-USB with ATtiny45 / ATtiny85 without a crystal
// http://codeandlife.com/2012/02/22/v-usb-with-attiny45-attiny85-without-a-crystal/
#include <avr/io.h>
#include <usbdrv.h>


#define abs(x) ((x) > 0 ? (x) : (-x))

// Called by V-USB after device reset
void hadUsbReset() {
    int frameLength, targetLength = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
    int bestDeviation = 9999;
    uint8_t trialCal, bestCal, step, region;

    // do a binary search in regions 0-127 and 128-255 to get optimum OSCCAL
    for(region = 0; region <= 1; region++) {
        frameLength = 0;
        trialCal = (region == 0) ? 0 : 128;
        
        for(step = 64; step > 0; step >>= 1) { 
            if(frameLength < targetLength) // true for initial iteration
                trialCal += step; // frequency too low
            else
                trialCal -= step; // frequency too high
                
            OSCCAL = trialCal;
            frameLength = usbMeasureFrameLength();
            
            if(abs(frameLength-targetLength) < bestDeviation) {
                bestCal = trialCal; // new optimum found
                bestDeviation = abs(frameLength -targetLength);
            }
        }
    }

    OSCCAL = bestCal;
}
