#ifndef SCOPE_SERIAL_H_
#define SCOPE_SERIAL_H_

#include "scope.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "mbed.h"
#endif

/**
 * Class to transmit debug data to a graphical scope on a PC
 *
 * This class uses regular serial to transmit data. The protocol is:
 * 
 * First the header (3 bytes): 7f ff bf
 * Then the channel count (1 byte): 03
 * Then the microtime (4 bytes, signed integer): 00 01 01 01
 * Then follow the floats (4 bytes each): 01 01 01 01 ...
 */
class SerialScope : public Scope {

public:

    /**
     * Constructor
     *
     * @param channels  Number of parallel channels
     */
#ifdef ARDUINO
    /**
     * @param serial    Serial stream to use (Serial1 by default)
     */
    SerialScope(size_t channels, Stream* serial = &Serial);
#else
    /**
     * @param rx        RX serial pin
     * @param tx        TX serial pin
     */
    SerialScope(size_t channels, PinName rx = USBRX, PinName tx = USBTX);
#endif

    /**
     * Destructor
     */
    virtual ~SerialScope();

    /**
     * Transmit collected buffer
     *
     * It is not entirely certain if this tranmission is non-blocking.
     * Regardless, it is fast.
     *
     * Sending a frame does _not_ clear the buffer. Calling send() repeatedly
     * will send the same values over and over.
     */
    virtual void send() override;

protected:
    char headers[3] = {0x7f, 0xff, 0xbf};

#ifdef ARDUINO
    Stream* serial_ptr;
#else
    BufferedSerial* serial_ptr; // Silly trick so we can use one variable
    BufferedSerial serial;
#endif

};

#endif