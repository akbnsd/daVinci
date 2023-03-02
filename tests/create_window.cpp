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
#include "GLFW/glfw3.h"


// creates a window and displays green color
// if successful the glfw, glad  work successfully
int main(){

    glfwInit();
    GLFWwindow* win = glfwCreateWindow(720, 480, "DaVinci - create window", NULL, NULL);

    glfwMakeContextCurrent(win);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        LG("error initializing glad\n");
        return 1;
    }

    glClearColor(0.0840f, 0.840f, 0.185f, 1.0f);

    while(!glfwWindowShouldClose(win)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(win);
        glfwWaitEvents();
    }

    return 0;
}
