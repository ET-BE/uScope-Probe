#ifndef ARDUINO

#include "scope_hid.h"

// Constructor
HIDScope::HIDScope(size_t channels, bool connect_blocking) : 
    Scope(channels), hid(connect_blocking, HID_REPORT_SIZE, MAX_HID_REPORT_SIZE) {

    size_t channels_max = (HID_REPORT_SIZE  - 1 - sizeof(long)) / sizeof(float);

    if (nchannels > channels_max) {
        nchannels = channels_max; // Error
    }

    output_report.length = HID_REPORT_SIZE;
    for (size_t i = 0; i < output_report.length; i++) {
        output_report.data[i] = 0; // Initialize at zero
    }

    output_report.data[0] = nchannels; // Already set the
    // channel count (not going to change)
}

// Destructor
HIDScope::~HIDScope() {

    hid.disconnect();
}

// Transmit frame
void HIDScope::send() {

    // Send time
    longUnion.l = HIDScope::micros();
    // Copy time into output report (after the nch byte)
    memcpy(&output_report.data[1], longUnion.bytes, sizeof(long));

    // With `.set()` we do not modify this report directly, because the non-
    // blocking send could still be working with it.

    // Copy data into output report (after the time bytes)
    memcpy(&output_report.data[1] + sizeof(long), data, nchannels * sizeof(float));

    // The output_report is continuously updated by the API, 
    // so we can send it directly:
    hid.send_nb(&output_report);
}

#endif
