#pragma once

#include "graphics/GraphicsContext.h"
#include "events/Event.h"
#include <GLFW/glfw3.h>
#include <functional>

namespace storytime {

    class Window {
    public:
        struct Config {
            const char* title;
            int width;
            int height;

            [[nodiscard]] float getAspectRatio() const;
        };

    private:
        struct GlfwCallbackData {
            std::function<void(const Event&)> onEvent;
        };

    private:
        const Config& config;
        GlfwCallbackData glfwCallbackData;
        GLFWwindow* glfwWindow;

    public:
        Window(const Config& config, GraphicsContext* graphicsContext);

        ~Window();

        [[nodiscard]] const Config& getConfig() const;

        void setOnEventListener(const std::function<void(const Event&)>& onEvent);

        [[nodiscard]] double getTime() const;

        void onUpdate() const;

    private:
        void initGlfw() const;

        static void onGlfwError(int error, const char* description);

        void setGlfwWindowHints(GraphicsContext* graphicsContext) const;

        [[nodiscard]] GLFWwindow* createGlfwWindow() const;

        void setGlfwCallbacks();

        void setGlfwWindowCallbacks() const;

        void setGlfwKeyCallbacks() const;

        void setGlfwMouseCallbacks() const;

        void destroyGlfwWindow() const;

        void terminateGlfw() const;
    };

}

