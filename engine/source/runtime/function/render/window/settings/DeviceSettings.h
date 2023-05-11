#include <stdint.h>
namespace EasyEngine {
    struct DeviceSettings{
        uint8_t contextVersionMajor = 3;
        uint8_t contextVersionMinor = 2;
        uint8_t samples = 4;
    };
}