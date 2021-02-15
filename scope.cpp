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
    hid(connect_blocking, 64, 64) {

    size_t channels_max = MAX_HID_REPORT_SIZE / sizeof(float) - 1;

    if (nchannels > channels_max) {
        nchannels = channels_max; // Error
    }

    hid.device_desc()

    nchannels = channels;
    data = new float(nchannels);

    output_report.length = MAX_HID_REPORT_SIZE; // 64
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

    // Copy data into output report (skipping the first byte)
    memcpy(&output_report.data[1], data, nchannels * sizeof(float));

    // The output_report is continuously updated by the API, 
    // so we can send it directly:
    hid.send_nb(&output_report);
}
