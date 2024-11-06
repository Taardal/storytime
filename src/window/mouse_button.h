#pragma once

namespace Storytime {
    typedef i32 MouseButtonCode;

    struct MouseButton {
        static const MouseButtonCode NONE;
        static const MouseButtonCode MOUSE_BUTTON_1;
        static const MouseButtonCode MOUSE_BUTTON_2;
        static const MouseButtonCode MOUSE_BUTTON_3;
        static const MouseButtonCode MOUSE_BUTTON_4;
        static const MouseButtonCode MOUSE_BUTTON_5;
        static const MouseButtonCode MOUSE_BUTTON_6;
        static const MouseButtonCode MOUSE_BUTTON_7;
        static const MouseButtonCode MOUSE_BUTTON_8;
        static const MouseButtonCode MOUSE_BUTTON_LAST;
        static const MouseButtonCode MOUSE_BUTTON_LEFT;
        static const MouseButtonCode MOUSE_BUTTON_RIGHT;
        static const MouseButtonCode MOUSE_BUTTON_MIDDLE;

        static const std::unordered_map<std::string, MouseButtonCode> buttons_by_name;

        static const std::unordered_map<MouseButtonCode, std::string> names_by_button;

        static MouseButtonCode from_name(const std::string& name);

        static std::string to_name(MouseButtonCode button_code);
    };
}
