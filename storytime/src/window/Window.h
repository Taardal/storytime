#pragma once

#include "graphics/GraphicsContext.h"
#include "events/Event.h"
#include <GLFW/glfw3.h>
#include <functional>

namespace storytime
{
    class Window
    {
    public:
        struct Config
        {
            const char* title;
            int32_t width;
            int32_t height;

            [[nodiscard]] float GetAspectRatio() const;
        };

    private:
        struct GlfwCallbackData
        {
            std::function<void(const Event&)> onEvent;
        };

    private:
        const Config& config;
        GlfwCallbackData glfwCallbackData;
        GLFWwindow* glfwWindow;

    public:
        Window(const Config& config, GraphicsContext* graphicsContext);

        ~Window();

        void SetOnEventListener(const std::function<void(const Event&)>& onEvent);

        [[nodiscard]] float GetTime() const;

        void OnUpdate() const;

    private:
        void InitGlfw() const;

        static void OnGlfwError(int32_t error, const char* description);

        void SetGlfwWindowHints(GraphicsContext* graphicsContext) const;

        [[nodiscard]] GLFWwindow* CreateGlfwWindow() const;

        void SetGlfwCallbacks();

        void SetGlfwWindowCallbacks() const;

        void SetGlfwKeyCallbacks() const;

        void SetGlfwMouseCallbacks() const;

        void DestroyGlfwWindow() const;

        void TerminateGlfw() const;
    };

}

