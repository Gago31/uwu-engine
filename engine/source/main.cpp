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
#include "Enemy.h"

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

enum class ProjectionType { Orthographic, Frustum, Perspective };

struct Controller {
    bool fillPolygon = true;
    ProjectionType projectionType = ProjectionType::Perspective;
} controller;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        controller.fillPolygon = not controller.fillPolygon;
    }
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
    constexpr unsigned int SCR_WIDTH = 1280;
    constexpr unsigned int SCR_HEIGHT = 720;
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "uwu enyin", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    gr::Matrix4f projection;
    projection = tr::perspective(60, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1, 100);

    gr::ModelViewProjectionShaderProgram pipeline;
    glUseProgram(pipeline.shaderProgram);
    gr::GPUShape gpuAxis = gr::toGPUShape(pipeline, gr::createAxis(7));
    gr::GPUShape plane = gr::toGPUShape(pipeline, gr::createColorQuad(0.1f, 0.5f, 0.1f));
    gr::GPUShape gpuGreenCube = gr::toGPUShape(pipeline, gr::createColorCube(0, 1, 0));
    gr::GPUShape gpuRedCube = gr::toGPUShape(pipeline, gr::createColorCube(1, 0, 0));


    glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    float t0 = glfwGetTime(), t1, dt;
    float cameraTheta = std::numbers::pi / 2;

    gr::Vector3f playerPos(-5.0f, -1.0f, 0.25f);
    
    std::vector<gr::Vector3f> enemyRoute = {
        { -3.0f, 2.0f, 0.25f },
        { -4.0f, 2.0f, 0.25f },
        { -4.0f, 3.0f, 0.25f },
        { -4.0f, 4.0f, 0.25f },
        { -3.0f, 4.0f, 0.25f },
        { -2.0f, 4.0f, 0.25f },
        { -2.0f, 3.0f, 0.25f },
        { -2.0f, 2.0f, 0.25f }
    };
    gr::GPUShapePtr redCube = std::make_shared<gr::GPUShape>(gpuRedCube);
    gr::SceneGraphNode enemyNode("Enemy", tr::translate(0,0,0));
    enemyNode.childs.push_back(std::make_shared<gr::SceneGraphNode>(gr::SceneGraphNode("Shape", tr::uniformScale(0.5f), redCube)));
    Enemy enemy(std::make_shared<gr::SceneGraphNode>(enemyNode), enemyRoute[0], enemyRoute);

    float MOVEMENT_PERIOD = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (controller.fillPolygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        t1 = glfwGetTime();
        dt = t1 - t0;
        t0 = t1;

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            cameraTheta -= 2 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            cameraTheta += 2 * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            playerPos[0] += 2 * std::sin(cameraTheta) * dt;
            playerPos[1] += 2 * std::cos(cameraTheta) * dt;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            playerPos[0] -= 2 * std::sin(cameraTheta) * dt;
            playerPos[1] -= 2 * std::cos(cameraTheta) * dt;
        }

        gr::Vector3f const viewPos = playerPos;
        gr::Vector3f const eye(
            playerPos[0] + std::sin(cameraTheta), 
            playerPos[1] + std::cos(cameraTheta),
            playerPos[2]);
        gr::Vector3f const at(0, 0, 1);

        gr::Matrix4f view = tr::lookAt(viewPos, eye, at);
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "view"), 1, GL_FALSE, view.data());


        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "projection"), 1, GL_FALSE, projection.data());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::identity().data());
        pipeline.drawCall(gpuAxis, GL_LINES);

        glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE,
            ((gr::Matrix4f) (tr::translate(0.0f, 0.0f, 0.0f) * tr::uniformScale(20))).data());
        pipeline.drawCall(plane);

        for (int i = 0; i < g.width(); i++) {
            for (int j = 0; j < g.height(); j++) {
                if (g.coord(i, j) == 1) {
                    glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::translate(i - g.width() / 2.0f, -j + g.height() / 2.0f, 0.5f).data());
                    pipeline.drawCall(gpuGreenCube);
                }
            }
        }

        // Enemy movement logic
        enemy.update(dt, MOVEMENT_PERIOD);
        /*enemyMovementTime = std::clamp(enemyMovementTime + dt, 0.0f, MOVEMENT_PERIOD);
        float enemyTranslationRatio = enemyMovementTime / MOVEMENT_PERIOD;
        float enemyX = enemyPos[0] + (enemyRoute[enemyNextPos][0] - enemyPos[0]) * enemyTranslationRatio;
        float enemyY = enemyPos[1] + (enemyRoute[enemyNextPos][1] - enemyPos[1]) * enemyTranslationRatio;
        if (enemyMovementTime == MOVEMENT_PERIOD) {
            enemyPos = enemyRoute[enemyNextPos];
            enemyNextPos = (enemyNextPos + 1) % enemyRoute.size();
            enemyMovementTime = 0.0f;
        }*/

        //glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, 
        //    ((gr::Matrix4f)(tr::translate(enemy.getX(), enemy.getY(), 0.25f) * tr::uniformScale(0.5f))).data());
        //pipeline.drawCall(gpuRedCube);
        //enemy.render(pipeline);
        gr::drawSceneGraphNode(enemy.getNode(), pipeline, "model");

        // glClear(GL_DEPTH_BUFFER_BIT);

        // GUI
        // gr::GPUShape gpuRainbowQuad = gr::toGPUShape(pipeline, gr::createRainbowQuad());
        // pipeline.drawCall(gpuRainbowQuad);

        glfwSwapBuffers(window);


    }

    gpuAxis.clear();
    gpuGreenCube.clear();
    gpuRedCube.clear();

    glfwTerminate();
    return 0;
}
