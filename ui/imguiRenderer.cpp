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

#include "imguiRenderer.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

const char* imgui_glsl_version = "#version 330 core";


void getImGuiReady(GLFWwindow* win){
    static bool isImguiReady = false;
    if(isImguiReady) return;

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init(imgui_glsl_version);
    isImguiReady=true;
};

imguiRenderer::imguiRenderer(GLFWwindow* win,  void (*func)()) : script(func), window(win)
{
    getImGuiReady(win);
}

void imguiRenderer::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    script();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();
}

void imguiRenderer::beginRendering()
{
    if(renderThread) {
        shouldRender = false;
        renderThread->join();
        delete renderThread;
    }

    renderThread = new std::thread(stat_render, this);
}

void imguiRenderer::stopRendering()
{
    if(!renderThread) return;

    shouldRender = false;
    renderThread->join();
    delete renderThread;
}


void imguiRenderer::setFps(int fps)
{
    frameDelay = std::chrono::milliseconds(1000 / fps);
}

void imguiRenderer::stat_render(imguiRenderer *rend)
{
    glfwMakeContextCurrent(rend->window);
    std::chrono::milliseconds currentDelay;
    auto timePoint = std::chrono::steady_clock().now();
    while(!glfwWindowShouldClose(rend->window) && rend->shouldRender){
        glClear(GL_COLOR_BUFFER_BIT);
        
        rend->render();

        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(rend->frameDelay));
        glfwSwapBuffers(rend->window);

        currentDelay =  rend->frameDelay - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock().now() - timePoint);
        timePoint = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(currentDelay);
    }

    rend->shouldRender=false;
}
