# MBED-Scope

Scope for embedded C++ to transmit data for a data-scope.

Designed to transmit data for uScope (<https://bitbucket.org/ctw-bw/uscope>).

## Versions

MBED and Arduino compatible versions both live in the main branch. These compile slightly differently using the `ARDUINO` macro.

Use either `HIDScope` from `scope_hid.h` or `SerialScope` from `scope_serial.h`.

### Legacy

There are older branches where different versions used to live. These were:

| Branch   | Used for                               |
|----------|----------------------------------------|
| main     | Everything (legacy: MBED, UART Serial) |
| mbed_hid | Legacy: MBED, HID USB                  |
| arduino  | Legacy: Arduino, UART Serial           |

## Example

You can use the library like:

```c++
#include <mbed.h>
#include "mbed-scope/scope_hid.h"

HIDScope scope(2); // Create two channels

int main()
{
    while (true) {

        scope.set(0, 3.14f);

        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        scope.set(1, r);

        scope.send();

        ThisThread::sleep_for(1ms);
    }
}
```