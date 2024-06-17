#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const int WIDTH = 800, HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void renderScene();

int main() {
    std::cout << "in main" << std::endl;
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    //creating a GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Casting", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //for resizing the window in runtime, width and height is associated with the runtime window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Initialize GLEW
    if(glewInit()!= GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //configure openGL
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {}
void renderScene() {}
