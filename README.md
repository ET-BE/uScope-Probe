# MBED-Scope

Scope for embedded C++ to transmit data for a data-scope.

Designed to transmit data for uScope (<https://bitbucket.org/ctw-bw/uscope>).

## Versions

| Branch   | Used for             |
|----------|----------------------|
| main     | MBED, UART Serial    |
| mbed_hid | MBED, HID USB        |
| arduino  | Arduino, UART Serial |

## Example

You can use the library like:

```c++
#include "mbed.h"
#include "mbed-scope/scope.h"

Scope scope(2); // Create two channels

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