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

    // 4. Main render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Clear the screen with a dark teal color
        glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front/back buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 5. Clean up allocated resources
    glfwTerminate();
    return 0;
}
