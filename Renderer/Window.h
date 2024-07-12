
#pragma once

#include <string>

#include <glm/glm.hpp>
#include "Common.h"

namespace GaladHen
{
    class IWindowAPI;
    class Input;

    class Window
    {

    public:

        Window(API apiToUse);

        Window(API apiToUse, const std::string& windowName, unsigned int width, unsigned int height);

        // WINDOW ----------------------------------------------------------------------------------------------------------------

        float GetAspectRatio();

        void CloseWindow();

        // void ChangeWindowName(const std::string& windowName);

        // void ChangeWindowSize(unsigned int width, unsigned int height);

        // INPUT -------------------------------------------------------------------------------------------------------------------

        // @brief
        // Set the callback to call when a key is pressed or change status
        void SetKeyboardCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Set the callback to call when a mouse key is pressed or change status
        void SetMouseKeyCallback(void (Input::*callback)(void* sender, unsigned int key, unsigned int action), Input* owner);

        // @brief
        // Set the callback to call when the mouse is moved
        void SetMousePositionCallback(void (Input::* callback)(void* sender, float mouseX, float mouseY), Input* owner);

        // @brief
        // Set the callback to call at window closing
        void SetClosingWindowCallback(void(Input::* callback)(void* sender), Input* owner);

        // @brief
        // Manual call to keyboard callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallKeyboardCallback(unsigned int key, unsigned int action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMouseKeyCallback(unsigned int key, unsigned int action);

        // @brief
        // Manual call to mouse callback on the owner
        // @param key: integer representing the activated key
        // @param action: the action type on the key
        void CallMousePositionCallback(float mouseX, float mouseY);

        // @brief
        // Manual call to closing window callback on the owner
        void CallClosingWindowCallback();

        // RENDERING ------------------------------------------------------------------------------------------------------

        // @brief
        // Perform operations needed at the beginning of a frame
        void BeginFrame();

        // @brief
        // Perform operations needed at the end of a frame
        void EndFrame();

        void SetColorBufferClearColor(glm::vec4 color);

        ~Window();

    protected:

        std::string WindowName;
        unsigned int Width;
        unsigned int Height;

        // pointer to api level window
        IWindowAPI* WinAPI;
        void (Input::*OutKeyboardCallback)(void* sender, unsigned int key, unsigned int action);
        void (Input::*OutMouseKeyCallback)(void* sender, unsigned int key, unsigned int action);
        void (Input::*OutMousePosCallback)(void* sender, float mouseX, float mouseY);
        void(Input::* OutClosingWindowCallback)(void* sender);
        Input* OutKeyboardCallbackOwner;
        Input* OutMouseKeyCallbackOwner;
        Input* OutMousePosCallbackOwner;
        Input* OutClosingWindowCallbackOwner;

        static void KeyboardCallback(Window* owner, unsigned int key, unsigned int action);
        static void MouseKeyCallback(Window* owner, unsigned int key, unsigned int action);
        static void MousePosCallback(Window* owner, float mouseX, float mouseY);
        static void ClosingWindowCallback(Window* owner);

    };
}