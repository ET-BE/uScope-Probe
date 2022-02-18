#include "scope_serial.h"

// Constructor
#ifdef ARDUINO
SerialScope::SerialScope(size_t channels, Stream* serial) : 
    Scope(channels) {
    
    serial_ptr = serial;
}
#else
SerialScope::SerialScope(size_t channels, PinName rx, PinName tx) : 
    Scope(channels), serial(tx, rx, 115200) {

    serial_ptr = &serial;
}
#endif

// Destructor
SerialScope::~SerialScope() {

#ifndef ARDUINO
    serial_ptr->close();
#endif
}

// Transmit frame
void SerialScope::send() {

    // Send header
    serial_ptr->write(headers, 3);

    // Send channel count
    const char nch[] = {static_cast<char>(nchannels)};
    serial_ptr->write(nch, 1);

    // Send time
    longUnion.l = SerialScope::micros();

    // Flip byte order before sending (that's how uScope expects it)
    for (size_t i = 3; i >= 0; i--) {
        serial_ptr->write(&longUnion.bytes[i], 1);
    }

    // Send floats
    for (size_t i = 0; i < nchannels; i++) {
        
        floatUnion.f = data[i];
        serial_ptr->write(floatUnion.bytes, 4);
    }
}
