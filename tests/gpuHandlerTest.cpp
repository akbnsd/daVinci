// This file is a part of  DaVinci
// Copyright (C) 2022  akshay bansod <aksh.bansod@yahoo.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.



#include "mixer/gpuHandler.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


int main(){

    glfwInit();
    GLFWwindow* win = glfwCreateWindow(720, 480, "DaVinci - gpuHandlerTest", NULL, NULL);
    glfwMakeContextCurrent(win);

    gpuHandler handler;
    audio::block blk, result;

    for(int i=0; i<DEF_BLOCKSIZE; i++){
        blk.data[i] = ((float)i) / (float) DEF_BLOCKSIZE;
    }

    handler.clear(0, blk);
    handler.clear(1, blk);
    handler.clear(2, blk);
    handler.clear(3, blk);



    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        handler.mix(result);
        glfwSwapBuffers(win);
        glfwWaitEvents();
    }

    return 0;
};
