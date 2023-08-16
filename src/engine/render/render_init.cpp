#include <stdio.h>
#include <iostream>
#include "render_init.h"

using std::cout;
using std::endl;

GLFWwindow *render_init_window (uint32_t width, uint32_t height) {
    GLFWwindow *ret = NULL;
    if(!glfwInit()) {
        cout << "glfwInit has failed!" << endl;
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ret = glfwCreateWindow(width, height, "2d_engine", NULL, NULL);
    if(!ret) {
        glfwTerminate();
        cout << "window has failed!" << endl;
        return NULL;
    }
    glfwMakeContextCurrent(ret);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        cout << "glewInit has failed!" << endl;
        return NULL;
    }
    glfwSwapInterval(1);
    return ret;
}

void render_init_quad (uint32_t *vbo, uint32_t *vao, uint32_t *ebo) {
    //     x,    y, z, u, v
    float vertices[] = { 0.5, 0.5, 0, 0, 0, 0.5, -0.5, 0, 0, 1, -0.5, -0.5, 0, 1, 1, -0.5, 0.5, 0, 1, 0 };
    uint32_t indices[] = { 0, 1, 3, 1, 2, 3 };
    float color[] = { 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // x, y, z
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    // u, v
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void render_begin () {
    glClearColor(0.08, 0.1, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void render_end (GLFWwindow *window) {
    glfwSwapBuffers(window);
}

void render_quad (uint32_t vao) {
    glBindVertexArray(vao);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}