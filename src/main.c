#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

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
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


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
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);


    //Try making a triangle

    // Create a Vertex I think
    //unsigned int VBO;
    //Put that point into a Engine to Generate, Create 1 Vertex at one Point
    //glGenBuffers(1, &VBO);
    //Generate several Buffer for multiple points at once
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    }
    // 4. Main render loop
    // !!!!!!!!!!!!!!!! MAIN LOOP !!!!!!!!!!!!!!!!!!!!!!!! ||
    while (!glfwWindowShouldClose(window)) {

        //Input
        process_input(window);

        //Render Input

        // Clear the screen with a dark teal color
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front/back buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 5. Clean up allocated resources
    (void)glfwTerminate();
    return 0;
}
