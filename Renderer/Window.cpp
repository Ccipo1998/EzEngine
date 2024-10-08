
#include "Window.h"
#include <UI/Page.h>
#include <Renderer/LayerAPI/IWindowAPI.h>
#include <Renderer/LayerAPI/OpenGL/WindowGL.h>

namespace GaladHen
{
    Window::Window(API apiToUse)
        : WindowName("")
        , Width(1280)
        , Height(720)
        , CurrentAPI(apiToUse)
        , WindowInput(Input{})
        , Page(nullptr)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(1280, 720, "");

            // Create UI page
            Page = new UIPage{ "Prova", this};

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    Window::Window(API apiToUse, const std::string& windowName)
        : WindowName(windowName)
        , Width(1280)
        , Height(720)
        , CurrentAPI(apiToUse)
        , WindowInput(Input{})
        , Page(nullptr)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(1280, 720, windowName.data());

            // Create UI page
            Page = new UIPage{ "Prova", this };

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    Window::Window(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height)
        : WindowName(windowName)
        , Width(width)
        , Height(height)
        , CurrentAPI(apiToUse)
        , WindowInput(Input{})
        , Page(nullptr)
    {
        switch (CurrentAPI)
        {
        case API::OpenGL:

            // Create OpenGL window
            WinAPI = new WindowGL(width, height, windowName.data());

            // Create UI page
            Page = new UIPage{ "Prova", this };

            break;

        default:
            break;
        }

        RegisterInputCallbacks();
    }

    float Window::GetAspectRatio()
    {
        return static_cast<float>(Width) / static_cast<float>(Height);
    }

    void Window::CloseWindow()
    {
        WinAPI->CloseWindow();
    }

    API Window::GetWindowAPI() const
    {
        return CurrentAPI;
    }

    IWindowAPI* Window::GetAPILevelWindow()
    {
        return WinAPI;
    }

    void Window::CallKeyboardCallback(KeyboardKey key, KeyAction action)
    {
        Window::KeyboardCallback(this, (int)key, (int)action);
    }

    void Window::CallMouseKeyCallback(MouseKey key, KeyAction action)
    {
        Window::MouseKeyCallback(this, (int)key, (int)action);
    }

    void Window::CallMousePositionCallback(float mouseX, float mouseY)
    {
        Window::MousePosCallback(this, mouseX, mouseY);
    }

    void Window::CallClosingWindowCallback()
    {
        Window::ClosingWindowCallback(this);
    }

    bool Window::IsKeyPressed(KeyboardKey key)
    {
        return WindowInput.Keyboard[(int)key];
    }

    bool Window::IsKeyPressed(MouseKey key)
    {
        return WindowInput.Mouse[(int)key];
    }

    void Window::RegisterInputCallbacks()
    {
        WinAPI->RegisterKeyboardCallback((void (*)(void*, unsigned int, unsigned int)) & Window::KeyboardCallback, this);
        WinAPI->RegisterMouseKeyCallback((void (*)(void*, unsigned int, unsigned int)) & Window::MouseKeyCallback, this);
        //WinAPI->RegisterMousePositionCallback((void (*)(void*, float, float)) & Window::MousePosCallback, this);
        WinAPI->RegisterClosingWindowCallback((void (*)(void*)) & Window::ClosingWindowCallback, this);
    }

    void Window::KeyboardCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->WindowInput.Keyboard[key] = action;
        owner->Page->SendKeyboardCallback(owner, key, action);
    }
    
    void Window::MouseKeyCallback(Window* owner, unsigned int key, unsigned int action)
    {
        owner->WindowInput.Mouse[key] = action;
        owner->Page->SendMouseKeyCallback(owner, key, action);
    }

    void Window::MousePosCallback(Window* owner, float mouseX, float mouseY)
    {
        owner->WindowInput.MouseX = mouseX;
        owner->WindowInput.MouseY = mouseY;
    }

    void Window::ClosingWindowCallback(Window* owner)
    {
        owner->WindowInput.CloseWindowRequested = true;
    }

    void Window::BeginFrame()
    {
        WinAPI->ClearFrontBuffers(true, true, true); // TODO: clear parameters exposed in Window class?

        // update mouse position
        float currentX, currentY;
        GetMousePosition(currentX, currentY);
        WindowInput.LastMouseX = WindowInput.MouseX;
        WindowInput.LastMouseY = WindowInput.MouseY;
        WindowInput.MouseX = currentX;
        WindowInput.MouseY = currentY;

        Page->NewFrame();
        Page->BuildPage();
    }

    void Window::EndFrame()
    {
        Page->Draw();

        WinAPI->SwapBuffers();
        WinAPI->InvokePendingCallbacks();
    }

    void Window::SetColorBufferClearColor(glm::vec4 color)
    {
        WinAPI->SetColorBufferClearColor(color);
    }

    void Window::EnableDepthTest(bool enable)
    {
        WinAPI->EnableDephtTest(enable);
    }

    void Window::GetMousePosition(float& mouseX, float& mouseY)
    {
        WinAPI->GetCursorPosition(mouseX, mouseY);
    }

    void Window::GetMousePositionDelta(float& deltaX, float& deltaY)
    {
        deltaX = WindowInput.MouseX - WindowInput.LastMouseX;
        deltaY = WindowInput.MouseY - WindowInput.LastMouseY;
    }

    bool Window::IsCloseWindowRequested()
    {
        return WindowInput.CloseWindowRequested;
    }

    Window::~Window()
    {
        // TODO

        delete Page;
    }
}
