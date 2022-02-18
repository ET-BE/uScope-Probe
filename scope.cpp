#include "scope.h"

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

    for (size_t i = 0; i++; i < size) {
        data[channel + i] = buffer[i];
    }
}
