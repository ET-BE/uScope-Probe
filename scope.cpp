#include "scope.h"

/**
 * Data structure to convert float to bytes
 */
union floatUnion {  
    float f;  
    char bytes[4];  
} floatUnion;

/**
 * Data structure to convert long to bytes
 */
union longUnion {  
    signed long l;  
    char bytes[4];  
} longUnion;

// Constructor
Scope::Scope(size_t channels, Stream* serial) {

    serial_ptr = serial;

    nchannels = channels;
    data = new float(nchannels);
}

// Destructor
Scope::~Scope() {
    if (data) {
        delete[] data;
    }
}

// Set channel value
void Scope::set(size_t channel, float val) {
    if (channel >= nchannels) {
        return; // Error
    }

    data[channel] = val;
}

// Set channel value from list
void Scope::set(const float* buffer, size_t channel, size_t size) {

    if (size == 0) {
        size = nchannels;
    }

    if (channel + size > nchannels) {
        return; // Error
    }

    // Don't write directly into the output report, because
    // it could still be sending (it's non-blocking after all)

    memcpy(&data[channel], buffer, size * sizeof(float));
}

// Transmit frame
void Scope::send() {

    if (!serial_ptr->availableForWrite()) {
        return; // Error, prevent blocking write
    }

    // Send header
    serial_ptr->write(headers, 3);

    // Send channel count
    const char nch[] = {static_cast<char>(nchannels)};
    serial_ptr->write(nch, 1);

    // Send time
    longUnion.l = micros();

    // Flip byte order before sending (that's how uScope expects it)
    char int_bytes[4];
    for (size_t i = 0; i < 4; i++) {
        int_bytes[i] = longUnion.bytes[3 - i];
    }
    serial_ptr->write(int_bytes, 4);

    // Send floats
    for (size_t i = 0; i < nchannels; i++) {
        
        floatUnion.f = data[i];
        serial_ptr->write(floatUnion.bytes, 4);
    }
}
