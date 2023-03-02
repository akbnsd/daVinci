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
#include "implot.h"

using namespace ImGui;

int xs[] = { 0, 1, 2, 3};
int ys[] = { 0, 1, 2, 3};
float bgColor[4];
double drag[3];


void imguiScript(){

    static bool showDemo=false, implotDemo=false;
    Begin("tester");
    if(Button("show demo")) showDemo = !showDemo;
    SameLine();
    if(Button("show implot demo")) implotDemo = !implotDemo;
    if(ColorPicker4("bg color", bgColor)){
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
    }
    
    if(ImPlot::BeginPlot("data plot")){

        ImPlot::PlotLine<int>("random", xs, ys, 4);
        ImPlot::PlotInfLines<int>("vertical",ys, 1);
        ImPlot::PlotLine<int>("rand", xs, ys, 4);
        ImPlot::EndPlot();  
    };

    End();

    if(showDemo) ShowDemoWindow();
    if(implotDemo) ImPlot::ShowDemoWindow();
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
