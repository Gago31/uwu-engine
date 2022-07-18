#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>
#include "Renderer2D.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "GameController.h"
#include "Renderer.h"
#include "NodeRegistry.h"
#include "SoundController.h"


// Load resources
#define uwuLoadShader(vertex_path,fragment_path,geometry_path,name) ResourceManager::loadShader(vertex_path, fragment_path, geometry_path, name)
#define uwuLoadTexture(path, alpha, name) ResourceManager::loadTexture(path, alpha, name)
#define uwuLoadModel(path,name) ResourceManager::loadModel(path, name)
#define uwuLoadSoundWAV(path,name) ResourceManager::loadSoundWAV(path, name)

// Set input bindings
//#define uwuSetKeyboardBindings(bindings) InputController::setKeyboardBindings(bindings)
//#define uwuSetJoystickBindings(bindings) InputController::setJoystickBindings(bindings)
//#define uwuSetAxisBindings(bindings) InputController::setAxisBindings(bindings)

namespace uwu {
    GLFWwindow* window;

    int setUpOpenGL() {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------

        window = glfwCreateWindow(GameController::SCR_WIDTH, GameController::SCR_HEIGHT, "uwu enyin", NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        //glfwSetKeyCallback(window, key_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        //glfwSetKeyCallback(window, InputController::key_callback);
        //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        //stbi_set_flip_vertically_on_load(true);

        glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 0;
    }

    void initialize() {
        if (setUpOpenGL()) exit(-1);
        GameController::initialize();
        Renderer2D::initialize(GameController::SCR_WIDTH, GameController::SCR_HEIGHT);
        InputController::initialize();
        NodeRegistry::initialize();
        SoundController::initialize();
        glfwSetJoystickCallback(InputController::joystick_callback);
    }

    void kill() {
        ResourceManager::clear();
        Renderer2D::clear();
        glfwTerminate();
    }

    void setMainScene(Scene& scene) {
        GameController::currentScene = &scene;
    }

    void run() {
        // trigger start event
        GameController::currentScene->start();

        float t0 = glfwGetTime(), t1, dt;
        while (!glfwWindowShouldClose(window)) {
            // ----------- time -----------
            t1 = glfwGetTime();
            dt = t1 - t0;
            t0 = t1;

            // ----------- show fps ------------
            bool show_fps = false;
            if (show_fps) {
                std::stringstream ss;
                ss << "uwu enyin [" << (int)(1.0f / dt) << "fps - " << (int)(1000 * dt) << " ms]";
                glfwSetWindowTitle(window, ss.str().c_str());
            }

            // ----------- handle input ---------
            glfwPollEvents();
            InputController::pollKeys(window);
            InputController::pollJoysticks();

            // ----------------- update -------------------------
            GameController::update(dt);
            GameController::currentScene->update(dt);

            // ----------------- draw ---------------------------

            // 3D
            GameController::currentScene->render();
            Renderer::render();

            // GUI
            Renderer2D::render();

            glfwSwapBuffers(window);
        }
    }

    void setKeyboardBindings(std::map<std::string, int> bindings) {
        InputController::setKeyboardBindings(bindings);
    }

    void setJoystickBindings(std::map<std::string, int> bindings) {
        InputController::setJoystickBindings(bindings);
    }

    void setAxisBindings(std::map<std::string, axis_type> bindings) {
        InputController::setAxisBindings(bindings);
    }

}
