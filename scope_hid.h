#ifndef SCOPE_HID_H_
#define SCOPE_HID_H_

#ifndef ARDUINO

#include <mbed.h>
#include <drivers/usb/include/usb/USBHID.h>

#include "scope.h"

/**
 * The size of the report sent
 * It seems that only the default 64 bytes really works.
 */
#ifndef HID_REPORT_SIZE
#define HID_REPORT_SIZE MAX_HID_REPORT_SIZE
#endif

/**
 * Class to transmit data to a graphical scope on a PC
 * 
 * This class relies on the HIDUsb protocol.
 * On the K64F this is the second micro-USB port, labeled `K64F`.
 * 
 * Sending over HID can be non-blocking and very fast, which is
 * ideal for this application.
 */
class HIDScope : public Scope {

public:

    /**
     * Constructor
     *
     * When you are connecting without blocking, you need to call `connect()`
     * at a later time.
     *
     * @param channels Number of parallel channels
     * @param connect_blocking When true (default), the program halts
     *                         until a USB connection has been established
     */
    HIDScope(size_t channels, bool connect_blocking = true);

    /**
     * Destructor
     */
    ~HIDScope();

    /**
     * Transmit collected buffer
     *
     * Sending is non-blocking using HID.
     *
     * Sending a frame does _not_ clear the buffer. Calling send() repeatedly
     * will send the same values over and over.
     */
    virtual void send() override;

protected:

    USBHID hid;
    HID_REPORT output_report;

};


#endif // ARDUINO

#endif // SCOPE_HID_H_
