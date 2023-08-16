#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GLEW/glew.h>

GLFWwindow *render_init_window (uint32_t width, uint32_t height);
void render_init_quad (uint32_t *vbo, uint32_t *vao, uint32_t *ebo);
void render_begin (void);
void render_end (GLFWwindow *window);
void render_quad (uint32_t vao);
