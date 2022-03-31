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

//namespace gr = Grafica;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n\0";

const char* fragmentShaderPlayerSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);\n"
"}\n\0";


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



struct Controller
{
    bool fillPolygon = true;
    bool showAxis = true;
} controller;

// This function will be executed whenever a key is pressed or released
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_SPACE)
    {
        controller.fillPolygon = not controller.fillPolygon;
    }
    else if (key == GLFW_KEY_LEFT_CONTROL)
    {
        controller.showAxis = not controller.showAxis;
    }
}

void move_player(float *vertices, int x, int y, float speed, float delta) {
    float velocity = speed * delta;
    vertices[0] += velocity * x;
    vertices[3] += velocity * x;
    vertices[6] += velocity * x;
    vertices[9] += velocity * x;
    vertices[1] += velocity * y;
    vertices[4] += velocity * y;
    vertices[7] += velocity * y;
    vertices[10] += velocity * y;
}


int main() {
    Grid g(5, 5, {
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 0, 1, 0, 0,
        1, 0, 0, 0, 1,
        1, 1, 0, 1, 1,
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

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "uwu", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    unsigned int fragmentShaderPlayer = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderPlayer, 1, &fragmentShaderPlayerSource, NULL);
    glCompileShader(fragmentShaderPlayer);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    unsigned int playerShaderProgram = glCreateProgram();
    glAttachShader(playerShaderProgram, vertexShader);
    glAttachShader(playerShaderProgram, fragmentShaderPlayer);
    glLinkProgram(playerShaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const int vertices_n = 4 * 3 * 5 * 5;
    const int indices_n = 2 * 3 * 5 * 5;
    float vertices[vertices_n];
    unsigned int indices[indices_n];
    float left = -0.8;
    float top = 0.8;

    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            if (g.coord(i, j) == 0) continue;
            int offset = (j * 5 + i) * 12;
            float sq_w = 0.3f;
            //top right
            vertices[offset] = left + sq_w * (i + 1);
            vertices[offset + 1] = top - sq_w * j;
            vertices[offset + 2] = 0.0f;
            // bottom right
            vertices[offset + 3] = left + sq_w * (i + 1);
            vertices[offset + 4] = top - sq_w * (j + 1);
            vertices[offset + 5] = 0.0f;
            //bottom left
            vertices[offset + 6] = left + sq_w * i;
            vertices[offset + 7] = top - sq_w * (j + 1);
            vertices[offset + 8] = 0.0f;
            // top left
            vertices[offset + 9] = left + sq_w * i;
            vertices[offset + 10] = top - sq_w * j;
            vertices[offset + 11] = 0.0f;
        }
    }
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 5; i++) {
            if (g.coord(i, j) == 0) continue;
            int offset = (j * 5 + i) * 6;
            int vertex_offset = (j * 5 + i) * 4;
            //top right
            indices[offset] = vertex_offset;
            indices[offset + 1] = vertex_offset + 1;
            indices[offset + 2] = vertex_offset + 3;
            // bottom right
            indices[offset + 3] = vertex_offset + 1;
            indices[offset + 4] = vertex_offset + 2;
            indices[offset + 5] = vertex_offset + 3;
        }
    }
    float verticesPlayer[] = {
         0.05f, -0.45f, 0.0f,  // top right
         0.05f, -0.65f, 0.0f,  // bottom right
        -0.15f, -0.65f, 0.0f,  // bottom left
        -0.15f, -0.45f, 0.0f   // top left 
    };
    unsigned int indicesPlayer[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    unsigned int pVBO, pVAO, pEBO;
    glGenVertexArrays(1, &pVAO);
    glGenBuffers(1, &pVBO);
    glGenBuffers(1, &pEBO);
    
    glBindVertexArray(pVAO);

    glBindBuffer(GL_ARRAY_BUFFER, pVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlayer), verticesPlayer, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPlayer), indicesPlayer, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------

    float t0 = glfwGetTime(), t1, dt;

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        t1 = glfwGetTime();
        dt = t1 - t0;
        t0 = t1;

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6 * 5 * 5, GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0); // no need to unbind it every time 

        move_player(verticesPlayer, 0, 1, 30, dt);

        glUseProgram(playerShaderProgram);
        glBindVertexArray(pVAO);
        //glUniformMatrix4fv(, 1, GL_FALSE, glm::value_ptr());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &pVAO);
    glDeleteBuffers(1, &pVBO);
    glDeleteBuffers(1, &pEBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(playerShaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
