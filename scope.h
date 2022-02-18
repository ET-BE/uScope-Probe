#ifndef SCOPE_H_
#define SCOPE_H_

#include <stddef.h>

#ifndef ARDUINO
#include <cstdint>
#include <cstring>
#endif

/**
 * Class to transmit data to a graphical scope on a PC
 *
 * Implement this abstract class with a transport type.
 */
class Scope {

public:

    /**
     * Constructor
     *
     * @param channels Number of parallel channels
     */
    Scope(size_t channels);

    /**
     * Destructor
     */
    ~Scope();

    /**
     * Update channel value
     */
    void set(size_t channel, float val);

    /**
     * Update multiple channels
     * 
     * `size` defaults to the number of channels available after `channel`.
     *
     * @param buffer Pointer to array of data to send
     * @param channel First channel to fill
     * @param size Number of elements in buffer to copy
     */
    void set(const float* buffer, size_t channel = 0, size_t size = 0);

    /**
     * Transmit collected buffer
     *
     * It is not entirely certain if this tranmission is non-blocking.
     * Regardless, it is fast.
     *
     * Sending a frame does _not_ clear the buffer. Calling send() repeatedly
     * will send the same values over and over.
     */
    virtual void send() = 0;

protected:
    size_t nchannels;
    float* data;

};

#endif