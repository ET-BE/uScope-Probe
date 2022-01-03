#include "mbed.h"
#include "drivers/usb/include/usb/USBHID.h"

/**
 * The size of the report sent
 * It seems that only the default 64 bytes really works.
 */
#define HID_REPORT_SIZE MAX_HID_REPORT_SIZE

/**
 * Class to transmit debug data to a graphical scope on a PC
 *
 * The protocol is (per report):
 * The number of channels (1 byte, signed integer): 03
 * The current runtime in micro seconds
 * (4 bytes, unsigned integer): 00 00 00 01
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
     * Update multiple channels for arrays
     *
     * @param buffer Pointer to array of data to send
     * @param channel First channel to fill (defaults to 0)
     * @param size Number of elements in buffer to copy (defaults to number of channels)
     */
    void set(const float* buffer, size_t channel = 0, size_t size = 0);

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
    USBHID hid;
    HID_REPORT output_report;

};
