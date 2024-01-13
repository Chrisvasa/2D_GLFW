
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

// OpenGL Shading Language
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
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // Handle Initialization failure
    if (!glfwInit()) {
        printf("Unable to initialize GLFW!\n");
        return EXIT_FAILURE;
    }
    // Using OpenGL 3.3 and the CORE-profile.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // Creates the window, sets width, height, title etc..
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2D Game", NULL, NULL);
    if (window == NULL) {
        error_callback(404, "Window or OpenGL context creation failed!");
        glfwTerminate();
        return - 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific. 
    // GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        error_callback(400, "Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }

    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    /*
    The glShaderSource function takes the shader object to compile to as its first argument. 
    The second argument specifies how many strings we're passing as source code, which is only one. 
    The third parameter is the actual source code of the vertex shader and we can leave the 4th parameter to NULL. 
    */
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
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    /*
    The glCreateProgram function creates a program and returns the ID reference to the newly created program object. 
    Now we need to attach the previously compiled shaders to the program object and then link them with glLinkProgram: 
    */
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
    //    X,     Y,    Z coordinates.
         0.0f,  0.25f,  0.0f,  // top
       -0.15f, -0.25f,  0.0f,  // Bottom left
         //0.0f, -0.15f,  0.0f,  // Bottom mid
        0.15f, -0.25f,  0.0f,  // Bottom right

        0.30f,  0.25f,  0.0f,  // top
        0.15f, -0.25f,  0.0f,  // Bottom left
        0.45f, -0.25f,  0.0f,  // Bottom right

        0.15f,  0.75f,  0.0f,  // top
        0.0f,   0.25f,   0.0f,  // Bottom left
        0.30f,  0.25f,  0.0f  // Bottom right
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 3, 2 // second triangle
    };

    /*
    With the vertex data defined we'd like to send it as input to the first process of the graphics pipeline: the vertex shader.
    This is done by creating memory on the GPU where we store the vertex data,
      configure how OpenGL should interpret the memory and specify how to send the data to the graphics card.
    The vertex shader then processes as much vertices as we tell it to from its memory.

    We manage this memory via so called vertex buffer objects (VBO) that can store a large number of vertices in the GPU's memory.
    The advantage of using those buffer objects is that we can send large batches of data all at once to the graphics card,
      and keep it there if there's enough memory left, without having to send data one vertex at a time.
    Sending data to the graphics card from the CPU is relatively slow,
      so wherever we can we try to send as much data as possible at once.
    Once the data is in the graphics card's memory the vertex shader has almost instant access to the vertices
      making it extremely fast
    */
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s).
    glBindVertexArray(VAO);
    /*
    glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
    The first argument is the type of the buffer we want to copy data into: the vertex buffer object currently
      bound to the GL_ARRAY_BUFFER target.
    The second argument specifies the size of the data (in bytes) we want to pass to the buffer;
      a simple sizeof of the vertex data suffices.
    The third parameter is the actual data we want to send.
    The fourth parameter specifies how we want the graphics card to manage the given data. This can take 3 forms:
        - GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        - GL_STATIC_DRAW: the data is set only once and used many times.
        - GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STATIC_DRAW);
    // 1. then set the vertex attribute pointers
    /*
    The first parameter specifies which vertex attribute we want to configure.
      Remember that we specified the location of the position vertex attribute in the vertex shader with layout (location = 0).
      This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
    The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
    The third argument specifies the type of the data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
    The next argument specifies if we want the data to be normalized.
      If we're inputting integer data types (int, byte) and we've set this to GL_TRUE,
       the integer data is normalized to 0 (or -1 for signed data) and 1 when converted to float.
      This is not relevant for us so we'll leave this at GL_FALSE.
    The fifth argument is known as the stride and tells us the space between consecutive vertex attributes.
      Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.
      Note that since we know that the array is tightly packed (there is no space between the next vertex attribute value)
        we could've also specified the stride as 0 to let OpenGL determine the stride
        (this only works when values are tightly packed).
      Whenever we have more vertex attributes we have to carefully define the spacing between each vertex attribute
       but we'll get to see more examples of that later on.
    The last parameter is of type void* and thus requires that weird cast.
     This is the offset of where the position data begins in the buffer.
     Since the position data is at the start of the data array this value is just 0.
     We will explore this parameter in more detail later on
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 2. use our shader program when we want to render an object
    //glUseProgram(shaderProgram);
    // 3. now draw the object
    // someOpenGLFunctionThatDrawsOurTriangle();
    /*
    We have to repeat this process every time we want to draw an object. 
    It may not look like that much, but imagine if we have over 5 vertex attributes and perhaps 100s of different objects 
     (which is not uncommon). 
    Binding the appropriate buffer objects and configuring all vertex attributes for each of those objects quickly 
     becomes a cumbersome process. 
    What if there was some way we could store all these state configurations into an object and simply bind this object to 
     restore its state? 
    */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ..:: Drawing code (in render loop) ::..
    // 
    glBindVertexArray(0);
    // 4. draw the object
    /*glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);*/
    //glBindVertexArray(0);
    // someOpenGLFunctionThatDrawsOurTriangle();

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

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT); // Clears screen
        //glClearColor(0.0f, 0.4f, 0.0f, 1.0f);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        glClearColor(0.4f, 0.0, 0.0, 0.0);
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        glClearColor(0.0, 0.4f, 0.0, 0.0);
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
        glClearColor(0.0, 0.0, 0.4f, 0.0);
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
        glClearColor(0.4f, 0.4f, 0.0, 0.0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
