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
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.hpp"
#include "mglCamera.hpp"

namespace mgl
{

    class App;
    class Engine;

    //////////////////////////////////////////////////////////////////////////// App

    class App
    {
    public:
        virtual void initCallback(GLFWwindow *window)                                           {}
        virtual void displayCallback(GLFWwindow *window, double elapsed)                        {}
        virtual void windowCloseCallback(GLFWwindow *window)                                    {}
        virtual void windowSizeCallback(GLFWwindow *window, int width, int height)              {}
        virtual void joystickCallback(int jid, int event)                                       {}
    };

    ///////////////////////////////////////////////////////////////////////// Engine

    class Engine
    {
    public:
        unsigned short WindowWidth, WindowHeight;
        std::shared_ptr<CameraBuffer> cameraBuff;
        std::string nextSceneToLoad;
        bool loadNextScene = false;

        static Engine &getInstance();

        void setApp(App *app);
        App *getApp();
        void setOpenGL(int major, int minor);
        void setWindow(int width, int height, const char *title, int fullscreen, int vsync);
        void swapFullscren();
        void init(std::string sceneName);
        void run();
        void close();
        void loadScene(std::string &path);

        std::shared_ptr<tengine::Scene> &getScene() { return currentScene; }

    protected:
        virtual ~Engine();

    private:
        Engine();
        App *GlApp;
        int GlMajor, GlMinor;
        GLFWwindow *Window;
        const char *WindowTitle;
        int Fullscreen;
        int Vsync;
        std::shared_ptr<tengine::Scene> currentScene;
        bool running;

        void setupWindow();
        void setupGLFW();
        void setupGLEW();
        void setupOpenGL();
        void setupCallbacks();

    public:
        Engine(Engine const &) = delete;
        void operator=(Engine const &) = delete;

        std::shared_ptr<CameraBuffer> getCameraBuff() { return cameraBuff; }
    };

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_APP_HPP */
