#include "st_mouse_button.h"

namespace Storytime {
    constexpr MouseButtonCode MouseButton::NONE = -1;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT;
    constexpr MouseButtonCode MouseButton::MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE;

    const std::unordered_map<std::string, MouseButtonCode> MouseButton::buttons_by_name = {
        { "BUTTON_1", MouseButton::MOUSE_BUTTON_1 },
        { "BUTTON_2", MouseButton::MOUSE_BUTTON_2 },
        { "BUTTON_3", MouseButton::MOUSE_BUTTON_3 },
        { "BUTTON_4", MouseButton::MOUSE_BUTTON_4 },
        { "BUTTON_5", MouseButton::MOUSE_BUTTON_5 },
        { "BUTTON_6", MouseButton::MOUSE_BUTTON_6 },
        { "BUTTON_7", MouseButton::MOUSE_BUTTON_7 },
        { "BUTTON_8", MouseButton::MOUSE_BUTTON_8 },
        { "BUTTON_LAST", MouseButton::MOUSE_BUTTON_LAST },
        { "BUTTON_LEFT", MouseButton::MOUSE_BUTTON_LEFT },
        { "BUTTON_RIGHT", MouseButton::MOUSE_BUTTON_RIGHT },
        { "BUTTON_MIDDLE", MouseButton::MOUSE_BUTTON_MIDDLE },
    };

    const std::unordered_map<MouseButtonCode, std::string> MouseButton::names_by_button = {
        { MouseButton::MOUSE_BUTTON_1, "BUTTON_1" },
        { MouseButton::MOUSE_BUTTON_2, "BUTTON_2" },
        { MouseButton::MOUSE_BUTTON_3, "BUTTON_3" },
        { MouseButton::MOUSE_BUTTON_4, "BUTTON_4" },
        { MouseButton::MOUSE_BUTTON_5, "BUTTON_5" },
        { MouseButton::MOUSE_BUTTON_6, "BUTTON_6" },
        { MouseButton::MOUSE_BUTTON_7, "BUTTON_7" },
        { MouseButton::MOUSE_BUTTON_8, "BUTTON_8" },
        { MouseButton::MOUSE_BUTTON_LAST, "BUTTON_LAST" },
        { MouseButton::MOUSE_BUTTON_LEFT, "BUTTON_LEFT" },
        { MouseButton::MOUSE_BUTTON_RIGHT, "BUTTON_RIGHT" },
        { MouseButton::MOUSE_BUTTON_MIDDLE, "BUTTON_MIDDLE" },
    };

    MouseButtonCode MouseButton::from_name(const std::string& name) {
        std::string needle = name;
        string_to_upper(needle);
        string_replace_all(needle, " ", "_");
        auto it = buttons_by_name.find(needle);
        if (it == buttons_by_name.end()) {
            return NONE;
        }
        return it->second;
    }

    std::string MouseButton::to_name(MouseButtonCode button_code) {
        auto it = names_by_button.find(button_code);
        if (it == names_by_button.end()) {
            return "";
        }
        return it->second;
    }
}