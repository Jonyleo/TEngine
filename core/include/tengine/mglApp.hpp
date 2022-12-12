////////////////////////////////////////////////////////////////////////////////
//
// Application Class
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_APP_HPP
#define MGL_APP_HPP

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.hpp"

namespace mgl
{

    class App;
    class Engine;

    //////////////////////////////////////////////////////////////////////////// App

    class App
    {
    public:
        virtual void initCallback(GLFWwindow *window) {}
        virtual void displayCallback(GLFWwindow *window, double elapsed) {}
        virtual void windowCloseCallback(GLFWwindow *window) {}
        virtual void windowSizeCallback(GLFWwindow *window, int width, int height) {}
        virtual void cursorCallback(GLFWwindow *window, double xpos, double ypos) {}
        virtual void keyCallback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {}
        virtual void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                         int mods) {}
        virtual void scrollCallback(GLFWwindow *window, double xoffset,
                                    double yoffset) {}
        virtual void joystickCallback(int jid, int event) {}
    };

    ///////////////////////////////////////////////////////////////////////// Engine

    class Engine
    {
    public:
        unsigned short WindowWidth, WindowHeight;

        static Engine &getInstance();

        void setApp(App *app);
        App *getApp();
        void setOpenGL(int major, int minor);
        void setWindow(int width, int height, const char *title, int fullscreen,
                       int vsync);
        void init(std::string sceneName);
        void run();

        tengine::Scene& getScene() { return *currentScene; }

    protected:
        virtual ~Engine();

    private:
        Engine();
        App *GlApp;
        int GlMajor, GlMinor;
        bool windowSizeChanged;
        GLFWwindow *Window;
        const char *WindowTitle;
        int Fullscreen;
        int Vsync;
        std::shared_ptr<tengine::Scene> currentScene;

        void setupWindow();
        void setupGLFW();
        void setupGLEW();
        void setupOpenGL();
        void setupCallbacks();

    public:
        Engine(Engine const &) = delete;
        void operator=(Engine const &) = delete;
    };

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_APP_HPP */
