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


#include "pch.hpp"
#include "ui/graphElem.hpp"
#include "math.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ui/imguiRenderer.hpp"
#include "imgui.h"
#include "implot.h"

#define TOPI 2 * 3.1415


graphElem* elemnt;

void imguiScript(){

    ImGui::Begin("win");
    elemnt->render();
    ImGui::End();
};


int main(){ 

    audio::init();
    audio::block* blk = audio::getBlock(), *block;
    for(int i=0;i<DEF_BLOCKSIZE;i++){
        blk->data[i] = sin(TOPI * (float) i / (float) DEF_BLOCKSIZE);
    }


    glfwInit();
    GLFWwindow* win = glfwCreateWindow(720, 480, "DaVinci - graphElemTest", NULL, NULL);

    glfwMakeContextCurrent(win);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        LG("error initializing glad\n");
        return 1;
    }
    
    gpuHandler hand;
    imguiRenderer rend(win, imguiScript);

    graphElem elem(&hand, 0, "demo plot");
    elemnt = &elem;
    
    hand.append(0, *blk);

    glClearColor(0.0840f, 0.840f, 0.185f, 1.0f);

    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        rend.render();
        glfwSwapBuffers(win);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
