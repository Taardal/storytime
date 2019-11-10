#include "Layer.h"
#include <sstream>

namespace storytime {

    Layer::Layer(std::string_view name) : name(name) {
    }

    std::string_view Layer::getName() const {
        return name;
    }

    std::string Layer::toString() const {
        std::stringstream ss;
        ss << name << "{}";
        return ss.str();
    }
}