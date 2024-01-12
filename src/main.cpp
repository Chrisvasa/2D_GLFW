#include <iostream>
#include <GL/gl.h>
#include <GLAD/include/glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char const *argv[])
{
    // Handle Initialization failure
    if (!glfwInit()) {
        printf("Unable to initialize GLFW!\n");
        return EXIT_FAILURE;
    }

    // Using OpenGL 3.3 and the CORE-profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glEnable(GL_DEPTH_TEST);

    // Creates the window, sets width, height, title etc..
    GLFWwindow* window = glfwCreateWindow(640, 480, "Snek", NULL, NULL);
    if(!window) {
        error_callback(404, "Window or OpenGL context creation failed!");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    // We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. 
    // GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        error_callback(400, "Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }
    
    /*
    The first two parameters of glViewport set the location of the lower left corner of the window. 
    The third and fourth parameter set the width and height of the rendering window in pixels, 
    which we set equal to GLFW's window size.
    glViewport(0,0, 640, 480);

    Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates 
     it processed to coordinates on your screen. 
    For example, a processed point of location (-0.5,0.5) would (as its final transformation) 
     be mapped to (200,450) in screen coordinates. 
    Note that processed coordinates in OpenGL are between -1 and 1 so we effectively map 
     from the range (-1 to 1) to (0, 800) and (0, 600). 
    */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clears screen
        glBegin(GL_QUADS);
        glVertex2f(-0.4f, 0.4f); // top left
        glVertex2f(0.6f, 0.64f); // top right
        glVertex2f(0.3f, -0.3f); // bottom right
        glVertex2f(-0.1f, -0.15f); // bottom left
        glEnd();

        /* Swap front and back buffers.
        Will swap the color buffer 
        (a large 2D buffer that contains color values for each pixel in GLFW's window) 
        that is used to render to during this render iteration and show it as output to the screen.
        */
        glfwSwapBuffers(window);

        /* Poll for and process events.
        Checks if any events are triggered (like keyboard input or mouse movement events), 
        updates the window state, and calls the corresponding functions 
        (which we can register via callback methods).
        */
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
