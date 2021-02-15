#include "mbed.h"
#include "drivers/usb/include/usb/USBHID.h"

/**
 * Class to transmit debug data to a graphical scope on a PC
 *
 * The protocol is (per report):
 * The number of channels (1 byte, signed integer): 03
 * Then follow the floats (4 bytes each): 01 01 01 01 ...
 *
 * Data is send over the second USB port, using USBHID. Sending over HID
 * can be non-blocking and very fast, which is ideal for this
 * application.
 */
class Scope {

public:

    /**
     * Constructor
     *
     * When you are connecting without blocking, you need to call `connect()`
     * at a later time.
     *
     * @param channels Number of parallel channels
     * @param connect_blocking When true (default), the program halts
     *                         untill a USB connection has been established
     */
    Scope(size_t channels, bool connect_blocking = true);

    /**
     * Destuctor
     */
    ~Scope();

    /**
     * Update channel value
     *
     * In modern C++, casting to e.g. float can be done
     * implictely, so overloading this method is not
     * necessary.
     */
    void set(size_t channel, float val);

    /**
     * Transmit collected buffer
     *
     * Sending is non-blocking using HID.
     *
     * Sending a frame does _not_ clear the buffer. Calling send() repeatedly
     * will send the same values over and over.
     */
    void send();

protected:
    size_t nchannels;
    float* data;
    const char headers[3] = {0x7f, 0xff, 0xbf};
    USBHID hid;
    HID_REPORT output_report;

};
