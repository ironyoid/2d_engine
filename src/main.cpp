#include <stdio.h>
#include "render_init.h"

#define WINDOW_WIDTH  (640U)
#define WINDOW_HEIGHT (480U)

void frameBufferResizeCallback (GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main (void) {
    uint32_t vbo, vao, ebo;
    GLFWwindow *window = render_init_window(WINDOW_WIDTH, WINDOW_HEIGHT);
    render_init_quad(&vbo, &vao, &ebo);

    while(!glfwWindowShouldClose(window)) {
        // Resize the viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        render_begin();
        render_quad(vao);
        render_end(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}