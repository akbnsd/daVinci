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
