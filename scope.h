#include "mbed.h"

/**
 * Class to transmit debug data to a graphical scope on a PC
 *
 * The protocol is:
 * First the header (3 bytes): 7f ff bf
 * Then the channel count (1 byte): 03
 * Then the microtime (4 bytes, signed integer): 00 01 01 01
 * Then follow the floats (4 bytes each): 01 01 01 01 ...
 */
class Scope {

public:

    /**
     * Constructor
     *
     * @param channels Number of parallel channels
     */
    Scope(size_t channels, PinName rx = USBRX, PinName tx = USBTX);

    /**
     * Destuctor
     */
    ~Scope();

    /**
     * Update channel value
     */
    void set(size_t channel, float val);

    /**
     * Transmit collected buffer
     *
     * It is not entirely certain if this tranmission is non-blocking.
     * Regardless, it is fast.
     *
     * Sending a frame does _not_ clear the buffer. Calling send() repeatedly
     * will send the same values over and over.
     */
    void send();

protected:
    size_t nchannels;
    float* data;
    char headers[3] = {0x7f, 0xff, 0xbf};
    BufferedSerial serial;
    time_t start_time;

};
