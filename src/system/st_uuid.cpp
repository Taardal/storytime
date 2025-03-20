#include "st_uuid.h"
#include "st_random.h"

namespace Storytime {
    std::string UUID::create() {
        const std::string hex_chars = "0123456789ABCDEF";
        std::string uuid = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
        for (int i = 0; i < uuid.size(); ++i) {
            if (uuid[i] == 'x') {
                uuid[i] = hex_chars[Random::get_int(0, 15)];
            } else if (uuid[i] == 'y') {
                uuid[i] = hex_chars[Random::get_int(8, 11)];
            }
        }
        return uuid;
    }
}
