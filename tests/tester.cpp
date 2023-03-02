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


#include "glad/glad.h"
#include "ui/imguiRenderer.hpp"
#include "imgui.h"

using namespace ImGui;

void imguiScript(){

    static bool showDemo=false;
    Begin("tester");
    if(Button("show demo window")) showDemo = !showDemo;

    End();

    if(showDemo) ShowDemoWindow();
};


int main(){
    glfwInit();
    GLFWwindow* win = glfwCreateWindow(720, 480, "DaVinci - tester", NULL, NULL);

    glfwMakeContextCurrent(win);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        LG("error initializing glad\n");
        return 1;
    }

    glClearColor(0.0840f, 0.840f, 0.185f, 1.0f);
    imguiRenderer rend(win, imguiScript);

    rend.setFps(60);
    rend.beginRendering();
    while (rend.isRenderingActive())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
    

}
