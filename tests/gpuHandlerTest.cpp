#include "mixer/gpuHandler.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


int main(){

    glfwInit();
    GLFWwindow* win = glfwCreateWindow(720, 480, "DaVinci - create window", NULL, NULL);
    glfwMakeContextCurrent(win);

    gpuHandler handler;
    audio::block blk;

    for(int i=0; i<DEF_BLOCKSIZE; i++){
        blk.data[i] = 1.0f; // * ((float)i) / (float) DEF_BLOCKSIZE;
    }

    handler.append(0, blk);
    handler.append(1, blk);
    handler.append(2, blk);
    handler.append(3, blk);



    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        handler.mix();
        glfwSwapBuffers(win);
        glfwWaitEvents();
    }

    return 0;
};
