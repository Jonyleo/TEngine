#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <functional>
#include <map>
#include <vector>
#include <set>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace tengine 
{
    class InputManager 
    {
        template <typename I>
        using Callback = std::function<void(I)>;

        private:
            InputManager() {}

            std::vector<Callback<struct MouseMovementInput>> mouseMovementCallbacks;
            std::vector<Callback<struct ScrollWheelInput>> scrollWheelCallbacks;
            std::map<int, std::vector<Callback<struct MouseButtonInput>>> mouseButtonCallbacks;
            std::map<int, std::vector<Callback<struct KeyInput>>> keyCallbacks;
            std::map<int, std::vector<Callback<struct MouseDragInput>>> mouseDragCallbacks;
            std::map<int, int> buttonsPressed;
            std::map<int, int> dragState;

            double cursorX = 0, cursorY = 0;
            double lastcursorX = 0, lastcursorY = 0;

            static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
            static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
            static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
            static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        protected:
            ~InputManager() {}

        public:
            static InputManager &getInstance()
            {
                static InputManager instance;
                return instance;
            }

            void init(GLFWwindow * window);

            void registerMouseMovement(Callback<struct MouseMovementInput>);
            void registerScrollWheel(Callback<struct ScrollWheelInput>);

            void registerMouseButton(int button, Callback<struct MouseButtonInput>);
            void registerMouseDrag(int button, Callback<struct MouseDragInput>);
            void registerKey(int key, Callback<struct KeyInput>);
    };

    struct MouseMovementInput {
        double x, y;
    };

    struct MouseButtonInput {
        int button;
        bool press;
        int mods;
    };

    struct MouseDragInput {
        int button;
        int mods;
        double x, y;
        bool finish;
    };

    struct ScrollWheelInput {
        double delta;
    };

    struct KeyInput {
        int key;
        bool press;
        int mods;
    };




}

#endif