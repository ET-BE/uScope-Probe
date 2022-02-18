#include "scope.h"

#ifndef ARDUINO
#include <mbed.h>
#include <chrono>
#endif

FloatUnion floatUnion;
LongUnion longUnion;

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

// Set channel value from list
void Scope::set(const float* buffer, size_t channel, size_t size) {

    if (size == 0) {
        size = nchannels - channel;
    }

    if (channel + size >= nchannels) {
        return; // Error
    }

    for (size_t i = 0; i < size; i++) {
        data[channel + i] = buffer[i];
    }
}

// micros (static method)
long Scope::micros() {

#ifdef ARDUINO
    return micros();
#else
    using namespace std::chrono;
    auto now_ms = time_point_cast<microseconds>(Kernel::Clock::now());
    return now_ms.time_since_epoch().count();
#endif
}
