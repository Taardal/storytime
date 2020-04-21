#pragma once

#include "graphics/GraphicsContext.h"
#include "graphics/ImGuiRenderer.h"
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
            const char* Title;
            int32_t Width;
            int32_t Height;
            bool Maximized;

            [[nodiscard]] float GetAspectRatio() const;
        };

        struct Size
        {
            int32_t Width;
            int32_t Height;
        };

    private:
        struct GlfwCallbackData
        {
            std::function<void(const Event&)> OnEvent;
        };

    private:
        const Config& config;
        GlfwCallbackData glfwCallbackData;
        GLFWwindow* glfwWindow;

    public:
        Window(const Config& config, GraphicsContext* graphicsContext, ImGuiRenderer* imGuiRenderer);

        ~Window();

        [[nodiscard]] Size GetSize() const;

        [[nodiscard]] float GetTime() const;

        void SetOnEventListener(const std::function<void(const Event&)>& onEvent);

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

