#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include <numbers>
#include <ciso646>
#include "Grid.h"
#include <grafica/shape.h>
#include <grafica/basic_shapes.h>
#include <grafica/load_shaders.h>
#include <grafica/easy_shaders.h>
#include <grafica/gpu_shape.h>
#include <grafica/transformations.h>
#include <grafica/scene_graph.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "root_directory.h"
#include "Enemy.h"
#include "Model.h"
#include "Shader.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Controller.h"
#include "Player.h"


namespace gr = Grafica;
namespace tr = Grafica::Transformations;
namespace uwu = UWU;

GLFWwindow *window;

enum class ProjectionType { Orthographic, Frustum, Perspective };

struct Controller {
    bool fillPolygon = true;
    ProjectionType projectionType = ProjectionType::Perspective;
    unsigned int SCR_WIDTH = 1280;
    unsigned int SCR_HEIGHT = 720;
    //unsigned int SCR_WIDTH = 1920;
    //unsigned int SCR_HEIGHT = 1080;
    float SCREEN_RATIO = float(SCR_WIDTH) / float(SCR_HEIGHT);
} controller;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        controller.fillPolygon = not controller.fillPolygon;
    }
}

void loadResources() {
    ResourceManager::loadShader(uwu::getPath("assets/shaders/sprite_shader.vs").string().c_str(), uwu::getPath("assets/shaders/sprite_shader.fs").string().c_str(), nullptr, "spriteShader");
    ResourceManager::loadShader(uwu::getPath("assets/shaders/model_shader.vs").string().c_str(), uwu::getPath("assets/shaders/model_shader.fs").string().c_str(), nullptr, "modelShader");
    //ResourceManager::loadShader(uwu::getPath("assets/shaders/mesh_shader.vs").string().c_str(), uwu::getPath("assets/shaders/mesh_shader.fs").string().c_str(), nullptr, "meshShader");
    ResourceManager::loadTexture(uwu::getPath("assets/imgs/uwu.png").string().c_str(), true, "uwu");
    ResourceManager::loadModel(uwu::getPath("assets/models/dcp.obj").string().c_str(), "DCP");
    ResourceManager::loadModel(uwu::getPath("assets/models/greenCube.obj").string().c_str(), "greenCube");
    ResourceManager::loadModel(uwu::getPath("assets/models/greenPlane.obj").string().c_str(), "greenPlane");
}

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
    //constexpr unsigned int SCR_WIDTH = 1280;
    //constexpr unsigned int SCR_HEIGHT = 720;
    ////constexpr unsigned int SCR_WIDTH = 1920;
    ////constexpr unsigned int SCR_HEIGHT = 1080;
    //constexpr float SCREEN_RATIO = float(SCR_WIDTH) / float(SCR_HEIGHT);

    window = glfwCreateWindow(controller.SCR_WIDTH, controller.SCR_HEIGHT, "uwu enyin", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetKeyCallback(window, InputController::key_callback);
    glfwSetJoystickCallback(InputController::joystick_callback);
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main() {
    int w = 10;
    int h = 10;

    Grid g(w, h, {
        1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        1, 0, 0, 0, 1, 0, 0, 1, 1, 1,
        1, 0, 1, 0, 0, 0, 1, 0, 0, 0,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 1,
        1, 1, 0, 1, 1, 1, 1, 0, 0, 1,
        1, 1, 0, 1, 1, 0, 1, 1, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
        1, 1, 0, 1, 0, 1, 0, 0, 0, 1,
        1, 1, 0, 1, 0, 0, 1, 0, 0, 1,
        1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
        });

    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            std::cout << g.coord(i, j) << ' ';
        }
        std::cout << std::endl;
    }

    InputController::initialize();
    InputController::setKeyboardBindings({
        { "left", GLFW_KEY_LEFT },
        { "right", GLFW_KEY_RIGHT },
        { "up", GLFW_KEY_UP },
        { "down", GLFW_KEY_DOWN }
    });
    InputController::setJoystickBindings({
        { "left", GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
        { "right", GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
        { "up", GLFW_GAMEPAD_BUTTON_DPAD_UP },
        { "down", GLFW_GAMEPAD_BUTTON_DPAD_DOWN }
    });
    InputController::setAxisBindings({
        { "left", { GLFW_GAMEPAD_AXIS_LEFT_X, -1 } },
        { "right", { GLFW_GAMEPAD_AXIS_LEFT_X, 1 } },
        { "up", { GLFW_GAMEPAD_AXIS_LEFT_Y, 1 } },
        { "down", { GLFW_GAMEPAD_AXIS_LEFT_Y, -1 } },
    });

//    // glfw: initialize and configure
//    float MOVEMENT_PERIOD = 1.0f;
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    constexpr unsigned int SCR_WIDTH = 1280;
//    constexpr unsigned int SCR_HEIGHT = 720;
//    //constexpr unsigned int SCR_WIDTH = 1920;
//    //constexpr unsigned int SCR_HEIGHT = 1080;
//    constexpr float SCREEN_RATIO = float(SCR_WIDTH) / float(SCR_HEIGHT);
//    
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "uwu enyin", NULL, NULL);
//    if (window == NULL) {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    //glfwSetKeyCallback(window, key_callback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//    glfwSetKeyCallback(window, InputController::key_callback);
//    glfwSetJoystickCallback(InputController::joystick_callback);
//    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    //stbi_set_flip_vertically_on_load(true);
//
//    glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (setUpOpenGL() == -1) {
        return -1;
    }

    float MOVEMENT_PERIOD = 1.0f;

    loadResources();

    Sprite gpuwu(std::make_shared<Texture2D>(ResourceManager::getTexture("uwu")));
    gpuwu.transform.translate(16.0f, 620.0f);

    SpriteRenderer renderer2d(std::make_shared<Shader>(ResourceManager::getShader("spriteShader")));
    glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<float>(controller.SCR_WIDTH), static_cast<float>(controller.SCR_HEIGHT), 0.0f, -1.0f, 1.0f);

    //std::vector<direction_t> enemyPath = { LEFT, UP, UP, RIGHT, RIGHT, DOWN, DOWN, LEFT };
    std::vector<direction_t> enemyPath = { LEFT, (direction_t)(WAIT | UP), UP, UP, (direction_t)(WAIT | RIGHT), RIGHT, RIGHT, (direction_t)(WAIT | DOWN), DOWN, DOWN, (direction_t)(WAIT | LEFT), LEFT };
    //std::vector<direction_t> enemyPath = { (direction_t) (WAIT | UP), (direction_t) (DOUBLE | UP), (direction_t)(WAIT | RIGHT), (direction_t)(DOUBLE | RIGHT), (direction_t)(WAIT | DOWN), (direction_t)(DOUBLE | DOWN), (direction_t)(WAIT | LEFT), (direction_t)(DOUBLE | LEFT) };
    Enemy enemy(std::make_shared<Model>(ResourceManager::getModel("DCP")), std::make_shared<Shader>(ResourceManager::getShader("modelShader")), {-3, 2, 0}, enemyPath);
    enemy.transform.scale(0.2f);
    enemy.transform.rotateX(glm::pi<float>() * 0.5);

    Player player(-5.0f, -1.0f, 0.25f);
    player.camera->setAspectRatio(controller.SCREEN_RATIO);


    float t0 = glfwGetTime(), t1, dt;
    float cameraTheta = std::numbers::pi / 2;


    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (controller.fillPolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        t1 = glfwGetTime();
        dt = t1 - t0;
        t0 = t1;

        player.update(dt);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // walls

        glm::mat4 player_view = player.camera->getView();
        glm::mat4 player_projection = player.camera->getProjection();

        ResourceManager::getShader("modelShader").use();
        ResourceManager::getShader("modelShader").setMatrix4f("projection", player_projection);
        ResourceManager::getShader("modelShader").setMatrix4f("view", player_view);
        
        glm::mat4 plane_transform = glm::mat4(1.0f);
        plane_transform = glm::scale(plane_transform, { 20.0f, 20.0f, 1.0f });
        plane_transform = glm::rotate(plane_transform, glm::radians<float>(90), { 1.0f, 0.0f, 0.0f });
        ResourceManager::getShader("modelShader").setMatrix4f("model", plane_transform);
        ResourceManager::getModel("greenPlane").Draw(std::make_shared<Shader>(ResourceManager::getShader("modelShader")));

        // walls
        for (int i = 0; i < g.width(); i++) {
            for (int j = 0; j < g.height(); j++) {
                if (g.coord(i, j) == 1) {
                    glm::mat4 cubeTransform(1.0f);
                    cubeTransform = glm::translate(cubeTransform, { i - g.width() / 2.0f, -j + g.height() / 2.0f, 0.5f });
                    cubeTransform = glm::scale(cubeTransform, glm::vec3(0.5f));
                    ResourceManager::getShader("modelShader").setMatrix4f("model", cubeTransform);
                    ResourceManager::getModel("greenCube").Draw(std::make_shared<Shader>(ResourceManager::getShader("modelShader")));
                }
            }
        }

        // dcp

        // enemy
        enemy.update(dt, MOVEMENT_PERIOD);
        enemy.render();

        // GUI
        glClear(GL_DEPTH_BUFFER_BIT);

        ResourceManager::getShader("spriteShader").use();
        ResourceManager::getShader("spriteShader").setMatrix4f("projection", spriteProjection);
        //ResourceManager::getShader("spriteShader").setInt("image", gpuwu.texture->ID);
        renderer2d.DrawSprite(std::make_shared<Sprite>(gpuwu));


        glfwSwapBuffers(window);
    }

    ResourceManager::clear();

    glfwTerminate();
    return 0;
}
