#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>
#include "Grid.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "root_directory.h"
#include "Enemy.h"
#include "Model.h"
#include "Shader.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "InputController.h"
#include "Player.h"
#include "GameController.h"
#include "SceneGraph.h"
#include "Node3D.h"
#include "MeshNode.h"
#include "Renderer.h"
#include "GridNode.h"


namespace uwu = UWU;

GLFWwindow *window;


void loadResources() {
    ResourceManager::loadShader(uwu::getPath("assets/shaders/sprite_shader.vs").string().c_str(), uwu::getPath("assets/shaders/sprite_shader.fs").string().c_str(), nullptr, "spriteShader");
    ResourceManager::loadShader(uwu::getPath("assets/shaders/model_shader.vs").string().c_str(), uwu::getPath("assets/shaders/model_shader.fs").string().c_str(), nullptr, "modelShader");
    //ResourceManager::loadShader(uwu::getPath("assets/shaders/model_lighting_shader.vs").string().c_str(), uwu::getPath("assets/shaders/model_lighting_shader.fs").string().c_str(), nullptr, "modelShader");
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

void setInputBindings() {
    glfwSetJoystickCallback(InputController::joystick_callback);
    InputController::initialize();
    InputController::setKeyboardBindings({
        { "left", GLFW_KEY_LEFT },
        { "right", GLFW_KEY_RIGHT },
        { "up", GLFW_KEY_UP },
        { "down", GLFW_KEY_DOWN },
        { "a", GLFW_KEY_Z },
        { "b", GLFW_KEY_X }
    });
    InputController::setJoystickBindings({
        { "left", GLFW_GAMEPAD_BUTTON_DPAD_LEFT },
        { "right", GLFW_GAMEPAD_BUTTON_DPAD_RIGHT },
        { "up", GLFW_GAMEPAD_BUTTON_DPAD_UP },
        { "down", GLFW_GAMEPAD_BUTTON_DPAD_DOWN },
        { "a", GLFW_GAMEPAD_BUTTON_A },
        { "b", GLFW_GAMEPAD_BUTTON_B }
    });
    /*InputController::setAxisBindings({
        { "left", { GLFW_GAMEPAD_AXIS_LEFT_X, -1 } },
        { "right", { GLFW_GAMEPAD_AXIS_LEFT_X, 1 } },
        { "up", { GLFW_GAMEPAD_AXIS_LEFT_Y, 1 } },
        { "down", { GLFW_GAMEPAD_AXIS_LEFT_Y, -1 } },
    });*/
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

    g.print();

    if (setUpOpenGL()) return -1;

    loadResources();

    GameController::initialize();
    setInputBindings();

    Sprite gpuwu(std::make_shared<Texture2D>(ResourceManager::getTexture("uwu")));
    gpuwu.transform.translate(16.0f, 620.0f);

    SpriteRenderer renderer2d(std::make_shared<Shader>(ResourceManager::getShader("spriteShader")));
    glm::mat4 spriteProjection = glm::ortho(0.0f, static_cast<float>(GameController::SCR_WIDTH), static_cast<float>(GameController::SCR_HEIGHT), 0.0f, -1.0f, 1.0f);

    //std::vector<direction_t> enemyPath = { LEFT, UP, UP, RIGHT, RIGHT, DOWN, DOWN, LEFT };
    std::vector<direction_t> enemyPath = { LEFT, (direction_t)(WAIT | UP), UP, UP, (direction_t)(WAIT | RIGHT), RIGHT, RIGHT, (direction_t)(WAIT | DOWN), DOWN, DOWN, (direction_t)(WAIT | LEFT), LEFT };
    //std::vector<direction_t> enemyPath = { (direction_t) (WAIT | UP), (direction_t) (DOUBLE | UP), (direction_t)(WAIT | RIGHT), (direction_t)(DOUBLE | RIGHT), (direction_t)(WAIT | DOWN), (direction_t)(DOUBLE | DOWN), (direction_t)(WAIT | LEFT), (direction_t)(DOUBLE | LEFT) };
    Enemy *enemy = new Enemy({2, 6, 0}, enemyPath);
    MeshNode *dcpNode = new MeshNode(std::make_shared<Model>(ResourceManager::getModel("DCP")), "modelShader");
    dcpNode->transform.scale(0.2f);
    dcpNode->transform.rotateX(glm::pi<float>() * 0.5);
    enemy->addChild(dcpNode);

    /*std::vector<direction_t> enemyPath2 = { (direction_t)(WAIT | UP), (direction_t)(WAIT | RIGHT), (direction_t)(WAIT | DOWN), (direction_t)(WAIT | LEFT) };
    Enemy* enemy2 = new Enemy({ 5, 5, 0 }, enemyPath2);
    MeshNode* dcpNode2 = new MeshNode(std::make_shared<Model>(ResourceManager::getModel("DCP")), std::make_shared<Shader>(ResourceManager::getShader("modelShader")));
    dcpNode2->transform.scale(0.2f);
    dcpNode2->transform.rotateX(glm::pi<float>() * 0.5);
    enemy2->addChild(dcpNode2);*/

    Player *player = new Player(0.0f, 3.0f, 0.25f, 90);
    player->camera->setAspectRatio(GameController::SCREEN_RATIO);
    player->camera->makeCurrent();

    GameController::currentGrid = std::make_shared<Grid>(g);

    float t0 = glfwGetTime(), t1, dt;


    // ----------- Build SceneTree -------------------------

    Scene scene;
    GameController::currentScene = &scene;
    Node *root = &scene.root;
    GridNode *gridNode = new GridNode(g);
    root->addChild(player);
    root->addChild(gridNode);
    root->addChild(enemy);
    //GameController::currentScene->root.addChild(enemy2);

    for (int j = 0; j < g.height(); j++) {
        for (int i = 0; i < g.width(); i++) {
            if (g.coord(i, j) == 1) {
                MeshNode *wall = new MeshNode(std::make_shared<Model>(ResourceManager::getModel("greenCube")), "modelShader");
                wall->transform.translate({ i, -j + g.height() - 1, 0.5f });
                wall->transform.scale(0.5f);
                gridNode->addChild(wall, i, -j + g.height() - 1);
            } else {
                MeshNode *floor = new MeshNode(std::make_shared<Model>(ResourceManager::getModel("greenPlane")), "modelShader");
                floor->transform.translate({ i, -j + g.height() - 1, 0.0f });
                floor->transform.scale(0.5f);
                floor->transform.rotateX(glm::radians<float>(90));
                gridNode->addChild(floor, i, -j + g.height() - 1);
            }
        }
    }

    while (!glfwWindowShouldClose(window)) {
        // ----------- time -----------
        t1 = glfwGetTime();
        dt = t1 - t0;
        t0 = t1;

        // ----------- show fps ------------
        bool show_fps = false;
        if (show_fps) {
            std::stringstream ss;
            ss << "uwu enyin [" << (int) (1.0f / dt) << "fps" << (int) (1000 * dt) << " ms]";
            glfwSetWindowTitle(window, ss.str().c_str());
        }

        // ----------- handle input ---------
        glfwPollEvents();
        InputController::pollJoysticks();
        InputController::pollKeys(window);

        // ----------------- update -------------------------

        GameController::update(dt);
        //player.update(dt);
        scene.update(dt);

        // ----------------- draw ---------------------------

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (GameController::fillPolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // 3D

        glm::mat4 player_view = player->camera->getView();
        glm::mat4 player_projection = player->camera->getProjection();

        Renderer::projection = player_projection;
        Renderer::view = player_view;
        /*ResourceManager::getShader("modelShader").setVector3f("lightColor", { 1.0f, 1.0f, 1.0f });
        ResourceManager::getShader("modelShader").setVector3f("lightPos", { 5.0f, 5.0f, 5.0f });
        ResourceManager::getShader("modelShader").setVector3f("viewPos", player.camera->getPos());*/
        
        scene.render();

        Renderer::render();

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
