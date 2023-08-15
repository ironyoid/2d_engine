#pragma once
#include <SDL_render.h>
#include "SDL.h"
#include "physics.h"
#include <iostream>
#include "sdl2.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <stdint.h>

SDL_Window *render_init_window(uint32_t width, uint32_t height);
void render_init_quad(uint32_t *vbo, uint32_t *vao, uint32_t *ebo);
void render_quad(uint32_t vao);
void render_begin(void);
void render_end(SDL_Window *window);