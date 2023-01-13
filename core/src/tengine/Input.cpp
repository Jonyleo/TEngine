#include <iostream>

#include <tengine/Input.hpp>
#include <tengine/mglApp.hpp>

namespace tengine {
    void InputManager::init(GLFWwindow * window) {
        glfwSetCursorPosCallback(window, cursor_pos_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        glfwSetScrollCallback(window, scroll_callback);
    }

    void InputManager::cursor_pos_callback(GLFWwindow *window, double xpos, double ypos){
        InputManager &manager = InputManager::getInstance();

        xpos /= mgl::Engine::getInstance().WindowWidth;
        ypos /= mgl::Engine::getInstance().WindowHeight;

        double moveX = xpos - manager.lastcursorX;
        double moveY = ypos - manager.lastcursorY;

        manager.lastcursorX = manager.cursorX;
        manager.lastcursorY = manager.cursorY;

        manager.cursorX = xpos;
        manager.cursorY = ypos;  
    

        for(auto entry : manager.mouseDragCallbacks) {
            int mouseButton = entry.first;

            if(manager.buttonsPressed.contains(mouseButton)) {
                if(manager.dragState[mouseButton] == 0) {
                    manager.dragState[mouseButton] = 1;
                    moveX = 0;
                    moveY = 0;
                }
                for(auto callback : entry.second) {
                    callback({mouseButton, manager.buttonsPressed[mouseButton], moveX, moveY, false});
                }
                return;
            }
        }

        for(auto callback : manager.mouseMovementCallbacks) {
            callback({moveX, moveY});
        }
    }
    void InputManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
        InputManager &manager = InputManager::getInstance();

        if(!manager.keyCallbacks.contains(key))
            return;

        if(action == GLFW_REPEAT)
            return; // Not handled

        for(auto callback : manager.keyCallbacks[key])
            callback({key, action == GLFW_PRESS, mods});

    }
    void InputManager::mouse_button_callback(GLFWwindow *window, int button, int action, int mods){
        InputManager &manager = InputManager::getInstance();

        if(action == GLFW_PRESS){
            manager.buttonsPressed[button] = mods;

            if(manager.mouseDragCallbacks.contains(button)) {
                manager.dragState[button] = 0;
            }
        }
        else {
            if(manager.dragState.contains(button)) {
                for(auto callback : manager.mouseDragCallbacks[button]) {
                    callback({button, mods, -1, -1, true});
                }
                manager.dragState.erase(button);
            }

            manager.buttonsPressed.erase(button);
        }
            
        if(!manager.mouseButtonCallbacks.contains(button))
            return;

        for(auto callback : manager.mouseButtonCallbacks[button])
            callback({button, action == GLFW_PRESS, mods});
    }
    void InputManager::scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
        InputManager &manager = InputManager::getInstance();

        for(auto callback : manager.scrollWheelCallbacks)
            callback({yoffset});
    }


    void InputManager::registerMouseMovement(Callback<struct MouseMovementInput> callback) {
        mouseMovementCallbacks.push_back(callback);
    }
    void InputManager::registerScrollWheel(Callback<struct ScrollWheelInput> callback) {
        scrollWheelCallbacks.push_back(callback);
    }
    void InputManager::registerMouseButton(int button, Callback<struct MouseButtonInput> callback) {
        if(!mouseButtonCallbacks.contains(button))
            mouseButtonCallbacks[button] = std::vector<Callback<struct MouseButtonInput>>();

        mouseButtonCallbacks[button].push_back(callback);
    }
    void InputManager::registerMouseDrag(int button, Callback<struct MouseDragInput> callback) {
        if(!mouseDragCallbacks.contains(button))
            mouseDragCallbacks[button] = std::vector<Callback<struct MouseDragInput>>();

        mouseDragCallbacks[button].push_back(callback);
        
    }
    void InputManager::registerKey(int key, Callback<struct KeyInput> callback) {
        if(!keyCallbacks.contains(key))
            keyCallbacks[key] = std::vector<Callback<struct KeyInput>>();

        keyCallbacks[key].push_back(callback);
    }
}