#pragma once

#include <utility>

namespace storytime {

    class Input {
    public:
        Input();

        ~Input();

        [[nodiscard]] bool isKeyPressed(int keyCode) const;

        [[nodiscard]] bool isMouseButtonPressed(int keyCode) const;

        [[nodiscard]] std::pair<float, float> getMousePosition() const;
    };

}


