#include <iostream>
#include <set>
#include <string>

#include <tengine/mglConventions.hpp>
#include <tengine/mglApp.hpp>
#include <tengine/mglError.hpp>

#include <tengine/Resources.hpp>
#include <tengine/Scene.hpp>
#include <tengine/Input.hpp>
#include <tengine/Transform.hpp>
#include <tengine/MeshInstance.hpp>

namespace mgl
{

    /////////////////////////////////////////////////////////////// STATIC CALLBACKS

    static void window_close_callback(GLFWwindow *window) {
        Engine::getInstance().getApp()->windowCloseCallback(window);
    }

    static void window_size_callback(GLFWwindow *window, int width, int height) {
        Engine::getInstance().getApp()->windowSizeCallback(window, width, height);
    }

    static void glfw_error_callback(int error, const char *description) {
        std::cerr << "GLFW Error: " << description << std::endl;
    }

    static void joystick_callback(int jid, int event) {
        Engine::getInstance().getApp()->joystickCallback(jid, event);
    }

    ////////////////////////////////////////////////////////////////////////// SETUP

    Engine::Engine(void)
    {
        GlApp = 0;
        Window = 0;
        WindowWidth = 640, WindowHeight = 480;
        GlMajor = 3, GlMinor = 3;
        Fullscreen = 0, Vsync = 0;
        WindowTitle = "Tangram Project";
        running = true;
    }

    Engine::~Engine(void) {}

    Engine &Engine::getInstance(void)
    {
        static Engine instance;
        return instance;
    }

    App *Engine::getApp(void)                       { return GlApp; }

    void Engine::setApp(App *app)                   { GlApp = app; }

    void Engine::setOpenGL(int major, int minor)
    {
        GlMajor = major;
        GlMinor = minor;
    }

    void Engine::setWindow(int width, int height, const char *title, int fullscreen, int vsync)
    {
        WindowWidth = width;
        WindowHeight = height;
        WindowTitle = title;
        Fullscreen = fullscreen;
        Vsync = vsync;
    }

    /////////////////////////////////////////////////////////////////////////// INIT

    void Engine::setupWindow()
    {
        GLFWmonitor *monitor = Fullscreen ? glfwGetPrimaryMonitor() : 0;
        Window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, monitor, 0);
        if (!Window)
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        glfwMakeContextCurrent(Window);
        glfwSwapInterval(Vsync);
    }

    void Engine::setupCallbacks()
    {
        glfwSetJoystickCallback(joystick_callback);
        glfwSetWindowCloseCallback(Window, window_close_callback);
        glfwSetWindowSizeCallback(Window, window_size_callback);
    }

    void Engine::setupGLFW()
    {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            exit(EXIT_FAILURE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GlMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GlMinor);

#ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        setupWindow();
        setupCallbacks();
    }

    void Engine::setupGLEW()
    {
        glewExperimental = GL_TRUE;

        // Allow extension entry points to be loaded even if the extension isn't
        // present in the driver's extensions string.
        GLenum result = glewInit();
        if (result != GLEW_OK)
        {
            std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void displayInfo()
    {
        std::cerr << "OpenGL Renderer: " << glGetString(GL_RENDERER) << " ("
                  << glGetString(GL_VENDOR) << ")" << std::endl;
        std::cerr << "OpenGL version " << glGetString(GL_VERSION) << std::endl;
        std::cerr << "GLSL version " << glGetString(GL_SHADING_LANGUAGE_VERSION)
                  << std::endl;
        std::cout << "GLEW version " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "GLFW version " << glfwGetVersionString() << std::endl;
        std::cout << GLM_VERSION_MESSAGE << std::endl;
    }

    void Engine::setupOpenGL()
    {
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glDepthRange(0.0, 1.0);
        glClearDepth(1.0);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glViewport(0, 0, WindowWidth, WindowHeight);

        
	    cameraBuff = std::make_shared<mgl::CameraBuffer>(mgl::CAMERA_BLOCK_BINDING_POINT);
    }

    void Engine::init(std::string sceneName)
    {
        setupGLFW();
        setupGLEW();
        setupOpenGL();

        tengine::Component::registerComponent("transform", &tengine::Transform::loadTransform);
        tengine::Component::registerComponent("meshInstance", &tengine::MeshInstance::loadMeshInstance);

        tengine::InputManager::getInstance().init(Window);

        GlApp->initCallback(Window);

        currentScene = tengine::ResourceManager::getInstance().load<tengine::Scene>(sceneName);
        currentScene->getRoot().init();

#ifdef DEBUG
        displayInfo();
        setupDebugOutput();
#endif
    }

    //////////////////////////////////////////////////////////////////////////// RUN

    void Engine::run()
    {
        double last_time = glfwGetTime();
        while (!glfwWindowShouldClose(Window) && running)
        {
            if(loadNextScene) {
                tengine::ResourceManager::getInstance().clear<tengine::Scene>(nextSceneToLoad);
                currentScene = tengine::ResourceManager::getInstance().load<tengine::Scene>(nextSceneToLoad);
                currentScene->getRoot().init();
                loadNextScene = false;
            }

            double time = glfwGetTime();
            double elapsed_time = time - last_time;
            last_time = time;
            currentScene->update(elapsed_time);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            currentScene->draw();
            currentScene->finish();
            glfwSwapBuffers(Window);
            glfwPollEvents();
        }
        glfwDestroyWindow(Window);
        glfwTerminate();
    }

    void Engine::close() {
        running = false;
    }

    void Engine::swapFullscren() {}

    
    void Engine::loadScene(std::string &path) {
        nextSceneToLoad = path;
        loadNextScene = true;
    }


    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
