#include <iostream>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if(key == GLFW_KEY_UP && action == GLFW_PRESS)
        glClearColor(1.0, 0.0, 0.0, 0.0);
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        glClearColor(0.0, 1.0, 0.0, 0.0);
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        glClearColor(0.0, 0.0, 1.0, 0.0);
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        glClearColor(1.0, 1.0, 0.0, 0.0);
}


int main(int argc, char const *argv[])
{
    // Handle Initialization failure
    if (!glfwInit()) {
        printf("Unable to initialize GLFW!\n");
        return EXIT_FAILURE;
    }
    glEnable(GL_DEPTH_TEST);
    // Setup
    GLFWwindow* window = glfwCreateWindow(640, 480, "Snek", NULL, NULL);
    if(!window) {
        error_callback(404, "Window or OpenGL context creation failed!");
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
    {
        // RENDER
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        glVertex2f(-0.4f, 0.4f); // top left
        glVertex2f(0.4f, 0.4f); // top right
        glVertex2f(0.4f, -0.4f); // bottom right
        glVertex2f(-0.4f, -0.4f); // bottom left
        glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}
