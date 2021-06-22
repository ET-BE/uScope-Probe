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
Scope::Scope(size_t channels) {

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

// Transmit frame
void Scope::send() {

    if (!Serial.availableForWrite()) {
        return; // Error, prevent blocking write
    }

    // Send header
    Serial.write(headers, 3);

    // Send channel count
    const char nch[] = {static_cast<char>(nchannels)};
    Serial.write(nch, 1);

    // Send time
    longUnion.l = micros();

    // Flip byte order before sending (that's how uScope expects it)
    char int_bytes[4];
    for (size_t i = 0; i < 4; i++) {
        int_bytes[i] = longUnion.bytes[3 - i];
    }
    Serial.write(int_bytes, 4);

    // Send floats
    for (size_t i = 0; i < nchannels; i++) {
        
        floatUnion.f = data[i];
        Serial.write(floatUnion.bytes, 4);
    }
}
