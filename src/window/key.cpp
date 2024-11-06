#include "key.h"

namespace Storytime {
    constexpr KeyCode Key::NONE = -1;
    constexpr KeyCode Key::KEY_SPACE = GLFW_KEY_SPACE;
    constexpr KeyCode Key::KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
    constexpr KeyCode Key::KEY_COMMA = GLFW_KEY_COMMA;
    constexpr KeyCode Key::KEY_MINUS = GLFW_KEY_MINUS;
    constexpr KeyCode Key::KEY_PERIOD = GLFW_KEY_PERIOD;
    constexpr KeyCode Key::KEY_SLASH = GLFW_KEY_SLASH;
    constexpr KeyCode Key::KEY_0 = GLFW_KEY_0;
    constexpr KeyCode Key::KEY_1 = GLFW_KEY_1;
    constexpr KeyCode Key::KEY_2 = GLFW_KEY_2;
    constexpr KeyCode Key::KEY_3 = GLFW_KEY_3;
    constexpr KeyCode Key::KEY_4 = GLFW_KEY_4;
    constexpr KeyCode Key::KEY_5 = GLFW_KEY_5;
    constexpr KeyCode Key::KEY_6 = GLFW_KEY_6;
    constexpr KeyCode Key::KEY_7 = GLFW_KEY_7;
    constexpr KeyCode Key::KEY_8 = GLFW_KEY_8;
    constexpr KeyCode Key::KEY_9 = GLFW_KEY_9;
    constexpr KeyCode Key::KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
    constexpr KeyCode Key::KEY_EQUAL = GLFW_KEY_EQUAL;
    constexpr KeyCode Key::KEY_A = GLFW_KEY_A;
    constexpr KeyCode Key::KEY_B = GLFW_KEY_B;
    constexpr KeyCode Key::KEY_C = GLFW_KEY_C;
    constexpr KeyCode Key::KEY_D = GLFW_KEY_D;
    constexpr KeyCode Key::KEY_E = GLFW_KEY_E;
    constexpr KeyCode Key::KEY_F = GLFW_KEY_F;
    constexpr KeyCode Key::KEY_G = GLFW_KEY_G;
    constexpr KeyCode Key::KEY_H = GLFW_KEY_H;
    constexpr KeyCode Key::KEY_I = GLFW_KEY_I;
    constexpr KeyCode Key::KEY_J = GLFW_KEY_J;
    constexpr KeyCode Key::KEY_K = GLFW_KEY_K;
    constexpr KeyCode Key::KEY_L = GLFW_KEY_L;
    constexpr KeyCode Key::KEY_M = GLFW_KEY_M;
    constexpr KeyCode Key::KEY_N = GLFW_KEY_N;
    constexpr KeyCode Key::KEY_O = GLFW_KEY_O;
    constexpr KeyCode Key::KEY_P = GLFW_KEY_P;
    constexpr KeyCode Key::KEY_Q = GLFW_KEY_Q;
    constexpr KeyCode Key::KEY_R = GLFW_KEY_R;
    constexpr KeyCode Key::KEY_S = GLFW_KEY_S;
    constexpr KeyCode Key::KEY_T = GLFW_KEY_T;
    constexpr KeyCode Key::KEY_U = GLFW_KEY_U;
    constexpr KeyCode Key::KEY_V = GLFW_KEY_V;
    constexpr KeyCode Key::KEY_W = GLFW_KEY_W;
    constexpr KeyCode Key::KEY_X = GLFW_KEY_X;
    constexpr KeyCode Key::KEY_Y = GLFW_KEY_Y;
    constexpr KeyCode Key::KEY_Z = GLFW_KEY_Z;
    constexpr KeyCode Key::KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
    constexpr KeyCode Key::KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
    constexpr KeyCode Key::KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
    constexpr KeyCode Key::KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
    constexpr KeyCode Key::KEY_WORLD_1 = GLFW_KEY_WORLD_1;
    constexpr KeyCode Key::KEY_WORLD_2 = GLFW_KEY_WORLD_2;
    constexpr KeyCode Key::KEY_ESCAPE = GLFW_KEY_ESCAPE;
    constexpr KeyCode Key::KEY_ENTER = GLFW_KEY_ENTER;
    constexpr KeyCode Key::KEY_TAB = GLFW_KEY_TAB;
    constexpr KeyCode Key::KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
    constexpr KeyCode Key::KEY_INSERT = GLFW_KEY_INSERT;
    constexpr KeyCode Key::KEY_DELETE = GLFW_KEY_DELETE;
    constexpr KeyCode Key::KEY_RIGHT = GLFW_KEY_RIGHT;
    constexpr KeyCode Key::KEY_LEFT = GLFW_KEY_LEFT;
    constexpr KeyCode Key::KEY_DOWN = GLFW_KEY_DOWN;
    constexpr KeyCode Key::KEY_UP = GLFW_KEY_UP;
    constexpr KeyCode Key::KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
    constexpr KeyCode Key::KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
    constexpr KeyCode Key::KEY_HOME = GLFW_KEY_HOME;
    constexpr KeyCode Key::KEY_END = GLFW_KEY_END;
    constexpr KeyCode Key::KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
    constexpr KeyCode Key::KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
    constexpr KeyCode Key::KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
    constexpr KeyCode Key::KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
    constexpr KeyCode Key::KEY_PAUSE = GLFW_KEY_PAUSE;
    constexpr KeyCode Key::KEY_F1 = GLFW_KEY_F1;
    constexpr KeyCode Key::KEY_F2 = GLFW_KEY_F2;
    constexpr KeyCode Key::KEY_F3 = GLFW_KEY_F3;
    constexpr KeyCode Key::KEY_F4 = GLFW_KEY_F4;
    constexpr KeyCode Key::KEY_F5 = GLFW_KEY_F5;
    constexpr KeyCode Key::KEY_F6 = GLFW_KEY_F6;
    constexpr KeyCode Key::KEY_F7 = GLFW_KEY_F7;
    constexpr KeyCode Key::KEY_F8 = GLFW_KEY_F8;
    constexpr KeyCode Key::KEY_F9 = GLFW_KEY_F9;
    constexpr KeyCode Key::KEY_F10 = GLFW_KEY_F10;
    constexpr KeyCode Key::KEY_F11 = GLFW_KEY_F11;
    constexpr KeyCode Key::KEY_F12 = GLFW_KEY_F12;
    constexpr KeyCode Key::KEY_F13 = GLFW_KEY_F13;
    constexpr KeyCode Key::KEY_F14 = GLFW_KEY_F14;
    constexpr KeyCode Key::KEY_F15 = GLFW_KEY_F15;
    constexpr KeyCode Key::KEY_F16 = GLFW_KEY_F16;
    constexpr KeyCode Key::KEY_F17 = GLFW_KEY_F17;
    constexpr KeyCode Key::KEY_F18 = GLFW_KEY_F18;
    constexpr KeyCode Key::KEY_F19 = GLFW_KEY_F19;
    constexpr KeyCode Key::KEY_F20 = GLFW_KEY_F20;
    constexpr KeyCode Key::KEY_F21 = GLFW_KEY_F21;
    constexpr KeyCode Key::KEY_F22 = GLFW_KEY_F22;
    constexpr KeyCode Key::KEY_F23 = GLFW_KEY_F23;
    constexpr KeyCode Key::KEY_F24 = GLFW_KEY_F24;
    constexpr KeyCode Key::KEY_F25 = GLFW_KEY_F25;
    constexpr KeyCode Key::KEY_KP_0 = GLFW_KEY_KP_0;
    constexpr KeyCode Key::KEY_KP_1 = GLFW_KEY_KP_1;
    constexpr KeyCode Key::KEY_KP_2 = GLFW_KEY_KP_2;
    constexpr KeyCode Key::KEY_KP_3 = GLFW_KEY_KP_3;
    constexpr KeyCode Key::KEY_KP_4 = GLFW_KEY_KP_4;
    constexpr KeyCode Key::KEY_KP_5 = GLFW_KEY_KP_5;
    constexpr KeyCode Key::KEY_KP_6 = GLFW_KEY_KP_6;
    constexpr KeyCode Key::KEY_KP_7 = GLFW_KEY_KP_7;
    constexpr KeyCode Key::KEY_KP_8 = GLFW_KEY_KP_8;
    constexpr KeyCode Key::KEY_KP_9 = GLFW_KEY_KP_9;
    constexpr KeyCode Key::KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
    constexpr KeyCode Key::KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
    constexpr KeyCode Key::KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
    constexpr KeyCode Key::KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
    constexpr KeyCode Key::KEY_KP_ADD = GLFW_KEY_KP_ADD;
    constexpr KeyCode Key::KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
    constexpr KeyCode Key::KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
    constexpr KeyCode Key::KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
    constexpr KeyCode Key::KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
    constexpr KeyCode Key::KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
    constexpr KeyCode Key::KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
    constexpr KeyCode Key::KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
    constexpr KeyCode Key::KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
    constexpr KeyCode Key::KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
    constexpr KeyCode Key::KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
    constexpr KeyCode Key::KEY_MENU = GLFW_KEY_MENU;

    const std::unordered_map<std::string, KeyCode> Key::keys_by_name = {
            { "SPACE", KEY_SPACE },
            { "APOSTROPHE", KEY_APOSTROPHE },
            { "COMMA", KEY_COMMA },
            { "MINUS", KEY_MINUS },
            { "PERIOD", KEY_PERIOD },
            { "SLASH", KEY_SLASH },
            { "0", KEY_0 },
            { "1", KEY_1 },
            { "2", KEY_2 },
            { "3", KEY_3 },
            { "4", KEY_4 },
            { "5", KEY_5 },
            { "6", KEY_6 },
            { "7", KEY_7 },
            { "8", KEY_8 },
            { "9", KEY_9 },
            { "SEMICOLON", KEY_SEMICOLON },
            { "EQUAL", KEY_EQUAL },
            { "A", KEY_A },
            { "B", KEY_B },
            { "C", KEY_C },
            { "D", KEY_D },
            { "E", KEY_E },
            { "F", KEY_F },
            { "G", KEY_G },
            { "H", KEY_H },
            { "I", KEY_I },
            { "J", KEY_J },
            { "K", KEY_K },
            { "L", KEY_L },
            { "M", KEY_M },
            { "N", KEY_N },
            { "O", KEY_O },
            { "P", KEY_P },
            { "Q", KEY_Q },
            { "R", KEY_R },
            { "S", KEY_S },
            { "T", KEY_T },
            { "U", KEY_U },
            { "V", KEY_V },
            { "W", KEY_W },
            { "X", KEY_X },
            { "Y", KEY_Y },
            { "Z", KEY_Z },
            { "LEFT_BRACKET", KEY_LEFT_BRACKET },
            { "BACKSLASH", KEY_BACKSLASH },
            { "RIGHT_BRACKET", KEY_RIGHT_BRACKET },
            { "GRAVE_ACCENT", KEY_GRAVE_ACCENT },
            { "WORLD_1", KEY_WORLD_1 },
            { "WORLD_2", KEY_WORLD_2 },
            { "ESCAPE", KEY_ESCAPE },
            { "ENTER", KEY_ENTER },
            { "TAB", KEY_TAB },
            { "BACKSPACE", KEY_BACKSPACE },
            { "INSERT", KEY_INSERT },
            { "DELETE", KEY_DELETE },
            { "RIGHT", KEY_RIGHT },
            { "LEFT", KEY_LEFT },
            { "DOWN", KEY_DOWN },
            { "UP", KEY_UP },
            { "PAGE_UP", KEY_PAGE_UP },
            { "PAGE_DOWN", KEY_PAGE_DOWN },
            { "HOME", KEY_HOME },
            { "END", KEY_END },
            { "CAPS_LOCK", KEY_CAPS_LOCK },
            { "SCROLL_LOCK", KEY_SCROLL_LOCK },
            { "NUM_LOCK", KEY_NUM_LOCK },
    };

    const std::unordered_map<KeyCode, std::string> Key::names_by_key = {
            { KEY_SPACE, "SPACE" },
            { KEY_APOSTROPHE, "APOSTROPHE" },
            { KEY_COMMA, "COMMA" },
            { KEY_MINUS, "MINUS" },
            { KEY_PERIOD, "PERIOD" },
            { KEY_SLASH, "SLASH" },
            { KEY_0, "0" },
            { KEY_1, "1" },
            { KEY_2, "2" },
            { KEY_3, "3" },
            { KEY_4, "4" },
            { KEY_5, "5" },
            { KEY_6, "6" },
            { KEY_7, "7" },
            { KEY_8, "8" },
            { KEY_9, "9" },
            { KEY_SEMICOLON, "SEMICOLON" },
            { KEY_EQUAL, "EQUAL" },
            { KEY_A, "A" },
            { KEY_B, "B" },
            { KEY_C, "C" },
            { KEY_D, "D" },
            { KEY_E, "E" },
            { KEY_F, "F" },
            { KEY_G, "G" },
            { KEY_H, "H" },
            { KEY_I, "I" },
            { KEY_J, "J" },
            { KEY_K, "K" },
            { KEY_L, "L" },
            { KEY_M, "M" },
            { KEY_N, "N" },
            { KEY_O, "O" },
            { KEY_P, "P" },
            { KEY_Q, "Q" },
            { KEY_R, "R" },
            { KEY_S, "S" },
            { KEY_T, "T" },
            { KEY_U, "U" },
            { KEY_V, "V" },
            { KEY_W, "W" },
            { KEY_X, "X" },
            { KEY_Y, "Y" },
            { KEY_Z, "Z" },
            { KEY_LEFT_BRACKET, "LEFT_BRACKET" },
            { KEY_BACKSLASH, "BACKSLASH" },
            { KEY_RIGHT_BRACKET, "RIGHT_BRACKET" },
            { KEY_GRAVE_ACCENT, "GRAVE_ACCENT" },
            { KEY_WORLD_1, "WORLD_1" },
            { KEY_WORLD_2, "WORLD_2" },
            { KEY_ESCAPE, "ESCAPE" },
            { KEY_ENTER, "ENTER" },
            { KEY_TAB, "TAB" },
            { KEY_BACKSPACE, "BACKSPACE" },
            { KEY_INSERT, "INSERT" },
            { KEY_DELETE, "DELETE" },
            { KEY_RIGHT, "RIGHT" },
            { KEY_LEFT, "LEFT" },
            { KEY_DOWN, "DOWN" },
            { KEY_UP, "UP" },
            { KEY_PAGE_UP, "PAGE_UP" },
            { KEY_PAGE_DOWN, "PAGE_DOWN" },
            { KEY_HOME, "HOME" },
            { KEY_END, "END" },
            { KEY_CAPS_LOCK, "CAPS_LOCK" },
            { KEY_SCROLL_LOCK, "SCROLL_LOCK" },
            { KEY_NUM_LOCK, "NUM_LOCK" },
    };

    KeyCode Key::from_name(const std::string& name) {
        std::string needle = name;
        string_to_upper(needle);
        string_replace_all(needle, " ", "_");
        auto it = keys_by_name.find(needle);
        if (it == keys_by_name.end()) {
            return NONE;
        }
        return it->second;
    }

    std::string Key::to_name(KeyCode key_code) {
        auto it = names_by_key.find(key_code);
        if (it == names_by_key.end()) {
            return "";
        }
        return it->second;
    }
}
