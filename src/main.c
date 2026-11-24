#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cglm/cglm.h>

// Callback to adjust the OpenGL viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void)window;
    glViewport(0, 0, width, height);
}

// Handle keyboard input (closes window when ESC is pressed)
void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

}

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
" ourColor = aColor;\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
" FragColor = vec4(ourColor, 1.0);\n"
"}\n"
;

unsigned int compileShader(unsigned int type, const char* source);

int main(void) {
    // 1. Initialize GLFW and configure OpenGL 3.3 Core Profile
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2. Create the window and make its OpenGL context current
    GLFWwindow* window = glfwCreateWindow(800, 600, "Boids Simulation", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //May tell the program that it is 2d
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. Load all OpenGL function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwTerminate();
        return -1;
    }
    // !!!!!!!!!!!!!!! Handle Stuff before compiling !!!!!!!!!!!!!!!!!!!!!!!!!! 
    // Shader Stuff ||
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);

    //Fragment Shader ||
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Finish Compiling Shader ||
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Try making a triangle
float vertices[] = {
    // positions        // colors
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
};

    // Create a Vertex I think
    unsigned int VBO;
    unsigned int VAO;
    //Put that point into a Engine to Generate, Create 1 Vertex at one Point
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //Generate several Buffer for multiple points at once
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Vertex points, no stride because it starts with vertext then color then vertext etc...
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute list given vertices and color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
    // 4. Main render loop
    // !!!!!!!!!!!!!!!! MAIN LOOP !!!!!!!!!!!!!!!!!!!!!!!! ||
    while (!glfwWindowShouldClose(window)) {
        //Input
        process_input(window);

        //Render Input

        // Clear the screen with a dark teal color
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front/back buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //De-allocate all   resources after run(Is option but good practice for less errors)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);


    // 5. Clean up allocated resources
    (void)glfwTerminate();
    return 0;
}

unsigned int compileShader(unsigned int type, const char* source){
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return shader;
}
