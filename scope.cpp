#include "scope.h"
#include <chrono>

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
Scope::Scope(
    size_t channels,
    PinName rx,
    PinName tx) : serial(tx, rx, 115200) {

    nchannels = channels;
    data = new float(nchannels);
    timer.start();
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

// Transmit frame
void Scope::send() {

    // Send header
    serial.write(headers, 3);

    // Send channel count
    const char nch[] = {static_cast<char>(nchannels)};
    serial.write(nch, 1);

    // Send time
    longUnion.l = std::chrono::duration_cast<std::chrono::microseconds>(timer.elapsed_time()).count();
    // Flip byte order before sending (that's how uScope expects it)
    std::reverse(longUnion.bytes, longUnion.bytes + 4);
    serial.write(longUnion.bytes, 4);

    // Send floats
    for (size_t i = 0; i < nchannels; i++) {
        
        floatUnion.f = data[i];
        serial.write(floatUnion.bytes, 4);
    }
}
