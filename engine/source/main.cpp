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

namespace gr = Grafica;
namespace tr = Grafica::Transformations;

enum class ProjectionType { Orthographic, Frustum, Perspective };

struct Controller
{
    bool fillPolygon = true;
    ProjectionType projectionType = ProjectionType::Perspective;
} controller;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        controller.fillPolygon = not controller.fillPolygon;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        controller.projectionType = ProjectionType::Orthographic;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        controller.projectionType = ProjectionType::Frustum;
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        controller.projectionType = ProjectionType::Perspective;
    }
}


int main() {
    Grid g(10, 10, {
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
    int w = g.width();
    int h = g.height();

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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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


    glClearColor(0.0f, 0.6f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    float t0 = glfwGetTime(), t1, dt;
    float cameraTheta = std::numbers::pi / 4;

    gr::Vector3f playerPos(g.width() * 2.0f, g.height() / 2.0f, 0.25f);

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
                    glUniformMatrix4fv(glGetUniformLocation(pipeline.shaderProgram, "model"), 1, GL_FALSE, tr::translate(i - g.width() / 2.0f, j - g.height() / 2.0f, 0.5f).data());
                    pipeline.drawCall(gpuGreenCube);
                }
            }
        }

        glfwSwapBuffers(window);


    }

    gpuAxis.clear();
    gpuGreenCube.clear();

    glfwTerminate();
    return 0;
}



//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 800;
//
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
//"}\n\0";
//
//const char* fragmentShaderPlayerSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);\n"
//"}\n\0";


//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}



//struct Controller
//{
//    bool fillPolygon = true;
//    bool showAxis = true;
//} controller;
//
//// This function will be executed whenever a key is pressed or released
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (action != GLFW_PRESS)
//        return;
//
//    if (key == GLFW_KEY_ESCAPE)
//    {
//        glfwSetWindowShouldClose(window, true);
//    }
//    else if (key == GLFW_KEY_SPACE)
//    {
//        controller.fillPolygon = not controller.fillPolygon;
//    }
//    else if (key == GLFW_KEY_LEFT_CONTROL)
//    {
//        controller.showAxis = not controller.showAxis;
//    }
//}

//void move_player(float *vertices, int x, int y, float speed, float delta) {
//    float velocity = speed * delta;
//    vertices[0] += velocity * x;
//    vertices[3] += velocity * x;
//    vertices[6] += velocity * x;
//    vertices[9] += velocity * x;
//    vertices[1] += velocity * y;
//    vertices[4] += velocity * y;
//    vertices[7] += velocity * y;
//    vertices[10] += velocity * y;
//}
