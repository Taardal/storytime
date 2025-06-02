#pragma once

namespace Storytime {
    typedef i32 KeyCode;

    struct Key {
        static const KeyCode NONE;
        static const KeyCode KEY_SPACE;
        static const KeyCode KEY_APOSTROPHE;
        static const KeyCode KEY_COMMA;
        static const KeyCode KEY_MINUS;
        static const KeyCode KEY_PERIOD;
        static const KeyCode KEY_SLASH;
        static const KeyCode KEY_0;
        static const KeyCode KEY_1;
        static const KeyCode KEY_2;
        static const KeyCode KEY_3;
        static const KeyCode KEY_4;
        static const KeyCode KEY_5;
        static const KeyCode KEY_6;
        static const KeyCode KEY_7;
        static const KeyCode KEY_8;
        static const KeyCode KEY_9;
        static const KeyCode KEY_SEMICOLON;
        static const KeyCode KEY_EQUAL;
        static const KeyCode KEY_A;
        static const KeyCode KEY_B;
        static const KeyCode KEY_C;
        static const KeyCode KEY_D;
        static const KeyCode KEY_E;
        static const KeyCode KEY_F;
        static const KeyCode KEY_G;
        static const KeyCode KEY_H;
        static const KeyCode KEY_I;
        static const KeyCode KEY_J;
        static const KeyCode KEY_K;
        static const KeyCode KEY_L;
        static const KeyCode KEY_M;
        static const KeyCode KEY_N;
        static const KeyCode KEY_O;
        static const KeyCode KEY_P;
        static const KeyCode KEY_Q;
        static const KeyCode KEY_R;
        static const KeyCode KEY_S;
        static const KeyCode KEY_T;
        static const KeyCode KEY_U;
        static const KeyCode KEY_V;
        static const KeyCode KEY_W;
        static const KeyCode KEY_X;
        static const KeyCode KEY_Y;
        static const KeyCode KEY_Z;
        static const KeyCode KEY_LEFT_BRACKET;
        static const KeyCode KEY_BACKSLASH;
        static const KeyCode KEY_RIGHT_BRACKET;
        static const KeyCode KEY_GRAVE_ACCENT;
        static const KeyCode KEY_WORLD_1;
        static const KeyCode KEY_WORLD_2;
        static const KeyCode KEY_ESCAPE;
        static const KeyCode KEY_ENTER;
        static const KeyCode KEY_TAB;
        static const KeyCode KEY_BACKSPACE;
        static const KeyCode KEY_INSERT;
        static const KeyCode KEY_DELETE;
        static const KeyCode KEY_RIGHT;
        static const KeyCode KEY_LEFT;
        static const KeyCode KEY_DOWN;
        static const KeyCode KEY_UP;
        static const KeyCode KEY_PAGE_UP;
        static const KeyCode KEY_PAGE_DOWN;
        static const KeyCode KEY_HOME;
        static const KeyCode KEY_END;
        static const KeyCode KEY_CAPS_LOCK;
        static const KeyCode KEY_SCROLL_LOCK;
        static const KeyCode KEY_NUM_LOCK;
        static const KeyCode KEY_PRINT_SCREEN;
        static const KeyCode KEY_PAUSE;
        static const KeyCode KEY_F1;
        static const KeyCode KEY_F2;
        static const KeyCode KEY_F3;
        static const KeyCode KEY_F4;
        static const KeyCode KEY_F5;
        static const KeyCode KEY_F6;
        static const KeyCode KEY_F7;
        static const KeyCode KEY_F8;
        static const KeyCode KEY_F9;
        static const KeyCode KEY_F10;
        static const KeyCode KEY_F11;
        static const KeyCode KEY_F12;
        static const KeyCode KEY_F13;
        static const KeyCode KEY_F14;
        static const KeyCode KEY_F15;
        static const KeyCode KEY_F16;
        static const KeyCode KEY_F17;
        static const KeyCode KEY_F18;
        static const KeyCode KEY_F19;
        static const KeyCode KEY_F20;
        static const KeyCode KEY_F21;
        static const KeyCode KEY_F22;
        static const KeyCode KEY_F23;
        static const KeyCode KEY_F24;
        static const KeyCode KEY_F25;
        static const KeyCode KEY_KP_0;
        static const KeyCode KEY_KP_1;
        static const KeyCode KEY_KP_2;
        static const KeyCode KEY_KP_3;
        static const KeyCode KEY_KP_4;
        static const KeyCode KEY_KP_5;
        static const KeyCode KEY_KP_6;
        static const KeyCode KEY_KP_7;
        static const KeyCode KEY_KP_8;
        static const KeyCode KEY_KP_9;
        static const KeyCode KEY_KP_DECIMAL;
        static const KeyCode KEY_KP_DIVIDE;
        static const KeyCode KEY_KP_MULTIPLY;
        static const KeyCode KEY_KP_SUBTRACT;
        static const KeyCode KEY_KP_ADD;
        static const KeyCode KEY_KP_ENTER;
        static const KeyCode KEY_KP_EQUAL;
        static const KeyCode KEY_LEFT_SHIFT;
        static const KeyCode KEY_LEFT_CONTROL;
        static const KeyCode KEY_LEFT_ALT;
        static const KeyCode KEY_LEFT_SUPER;
        static const KeyCode KEY_RIGHT_SHIFT;
        static const KeyCode KEY_RIGHT_CONTROL;
        static const KeyCode KEY_RIGHT_ALT;
        static const KeyCode KEY_RIGHT_SUPER;
        static const KeyCode KEY_MENU;

        static const std::unordered_map<std::string, KeyCode> keys_by_name;

        static const std::unordered_map<KeyCode, std::string> names_by_key;

        static KeyCode from_name(const std::string& name);

        static std::string to_name(KeyCode key_code);
    };
}

namespace Storytime {
    typedef i32 ModCode;

    struct Mod {
        static const ModCode MOD_SHIFT;
        static const ModCode MOD_CONTROL;
        static const ModCode MOD_ALT;
        static const ModCode MOD_SUPER;
        static const ModCode MOD_CAPS_LOCK;
        static const ModCode MOD_NUM_LOCK;
    };
}

namespace Storytime {
    typedef i32 ActionCode;

    struct Action {
        static const ActionCode ACTION_PRESS;
        static const ActionCode ACTION_RELEASE;
        static const ActionCode ACTION_REPEAT;
    };
}
