#include "scope.h"
#include <chrono>

using namespace std::chrono;

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
Scope::Scope(size_t channels, bool connect_blocking) : 
    hid(connect_blocking, HID_REPORT_SIZE, MAX_HID_REPORT_SIZE) {

    size_t channels_max = (HID_REPORT_SIZE  - 1 - sizeof(long)) / sizeof(float);

    if (nchannels > channels_max) {
        nchannels = channels_max; // Error
    }

    nchannels = channels;
    data = new float(nchannels);

    output_report.length = HID_REPORT_SIZE;
    for (size_t i = 0; i < output_report.length; i++) {
        output_report.data[i] = 0; // Initialize at zero
    }

    output_report.data[0] = nchannels; // Already set the
    // channel count (not going to change)
}

// Destructor
Scope::~Scope() {
    if (data) {
        delete[] data;
    }
    hid.disconnect();
}

// Set channel value
void Scope::set(size_t channel, float val) {
    if (channel >= nchannels) {
        return; // Error
    }

    // Don't write directly into the output report,
    // because it could be sending (non-blocking after
    // all)

    data[channel] = val;
}

// Transmit frame
void Scope::send() {

    // Send time
    auto now_us = time_point_cast<microseconds>(Kernel::Clock::now());
    longUnion.l = now_us.time_since_epoch().count();

    uint8_t* cursor = output_report.data; // Start at the first byte of the list

    cursor += 1; // Skip number-of-channels byte

    // Copy time into output report (after the nch byte)
    memcpy(cursor, longUnion.bytes, sizeof(long));

    cursor += sizeof(long);

    // Copy data into output report (after the time bytes)
    memcpy(cursor, data, nchannels * sizeof(float));

    // The output_report is continuously updated by the API, 
    // so we can send it directly:
    hid.send_nb(&output_report);
}
