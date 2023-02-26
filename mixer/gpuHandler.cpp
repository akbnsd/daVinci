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
#include "audio/audio.hpp"
#include "gpuHandler.hpp"


bool IS_GL_READY=false, isGpuHandlerReady=false;
unsigned int shaderId=0, vao=0 , bufs[2], stateVar, scalesVar, weightsVar;
audio::block DEF_NULL_BLOCK;

void getGLReady();
void getGpuHandlerReady();


gpuHandler::gpuHandler(){

    getGpuHandlerReady();

    // setup textures
    glGenTextures(4, datas);
    for(int i=0; i<4; i++){
        int texid = datas[i];
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_1D, texid);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, DEF_BLOCKSIZE, 0, GL_RED, GL_FLOAT, DEF_NULL_BLOCK.data);
        glGenerateMipmap(GL_TEXTURE_1D);
    }

    glUseProgram(shaderId);
    // maps samplers to texture units
    glUniform1i(glGetUniformLocation(shaderId, "TEX0"), 0);
    glUniform1i(glGetUniformLocation(shaderId, "TEX1"), 1);
    glUniform1i(glGetUniformLocation(shaderId, "TEX2"), 2);
    glUniform1i(glGetUniformLocation(shaderId, "TEX3"), 3);


    // setup framebuffer and its texture
    glGenFramebuffers(1, &fbo);

    glGenTextures(1, &fb_tex);
    glBindTexture(GL_TEXTURE_1D, fb_tex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, DEF_BLOCKSIZE, 0, GL_RED, GL_FLOAT, NULL);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_1D, fb_tex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}



void gpuHandler::mix(audio::block& blk)
{   
    int prev[4];
    glGetIntegerv(GL_VIEWPORT, prev);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, DEF_BLOCKSIZE, 1);

    // setup rendering
    glBindVertexArray(vao);
    glUseProgram(shaderId);
    
    // bind textures
    for(int i=0; i<4; i++){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_1D, datas[i]);
    }


    // update uniforms
    glUniform4f(stateVar, 
        (float) offsets[0] / (float) sizes[0],
        (float) offsets[1] / (float) sizes[1],
        (float) offsets[2] / (float) sizes[2],
        (float) offsets[3] / (float) sizes[3]);


    glUniform4f(scalesVar, 
        1.0f / (float) activeCount,
        (float) DEF_BLOCKSIZE / (float) sizes[1],
        (float) DEF_BLOCKSIZE / (float) sizes[2],
        (float) DEF_BLOCKSIZE / (float) sizes[3]);

    glUniform4f(weightsVar, 
        1.0f / (float) activeCount,
        0.0f,
        0.0f,
        0.0f);
    
    // make draw calls
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

    // read data
    glBindTexture(GL_TEXTURE_1D, fb_tex);
    glGetTexImage(GL_TEXTURE_1D, 0, GL_RED, GL_FLOAT, blk.data);

    // set framebuffer to default
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // in debug mode render data to screen
    #ifndef NDEBUG
    glViewport(0, 100, DEF_BLOCKSIZE, 1);
    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
    #endif

    // re-adjust the viewport
    glViewport(prev[0], prev[1], prev[2], prev[3]);
}


void gpuHandler::append(int index, audio::block& blk)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_1D, datas[index]);
    glTextureSubImage1D(GL_TEXTURE_1D, 0, sizes[index], DEF_BLOCKSIZE, GL_RED, GL_FLOAT, blk.data);
    sizes[index] += DEF_BLOCKSIZE;
}

void gpuHandler::clear(int index, audio::block& blk)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_1D, datas[index]);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_R32F, DEF_BLOCKSIZE, 0, GL_RED, GL_FLOAT, blk.data);
    sizes[index] = DEF_BLOCKSIZE;
}

void gpuHandler::setOffSet(int index, int offset)
{
    // if index==-1 add offset to all
    if(index==-1){
        for(int i=0; i <4; i++){
            offsets[i] += offset;
        }
        return;
    }

    offsets[index] += offset;
}

audio::block *gpuHandler::getData(int index, int count, int offset)
{
    audio::block* blk = audio::getBlock();
    glBindTexture(GL_TEXTURE_1D, datas[index]);
    glGetTextureSubImage(GL_TEXTURE_1D, 0, offset, 0, 0, count, 1, 1, GL_RED, GL_FLOAT, DEF_BLOCKSIZE * sizeof(float), blk->data);
    return blk;
}

gpuHandler::~gpuHandler(){

    glDeleteTextures(4, datas);
    glDeleteTextures(1, &fb_tex);
    glDeleteFramebuffers(1, &fbo);
}





// static calls

const char* vertSrc = R"(
#version 330 core

layout(location=0) in vec2 pos;
out float off;

void main(){
    gl_Position = vec4(pos, 1.0f, 1.0f);
    off = pos.x * 0.5f + 0.5f;
}

)";


const char* fragSrc= R"(
#version 330 core

in float off;
out vec4 data;

uniform vec4 param;
uniform vec4 scales;
uniform vec4 weights;
uniform sampler1D TEX0;
uniform sampler1D TEX1;
uniform sampler1D TEX2;
uniform sampler1D TEX3;

void main(){

    data.x  = texture(TEX0, param.x + off * scales.x).x * weights.x;
    data.x += texture(TEX1, param.y + off * scales.y).x * weights.x;
    data.x += texture(TEX2, param.z + off * scales.z).x * weights.x;
    data.x += texture(TEX3, param.w + off * scales.w).x * weights.x;

    data.yz = data.xx;
    data.w = 1.0f;
}

)";




void getGLReady(){
    if(IS_GL_READY) return;

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        LG("error initializing glad\n");
        return;
    }
    IS_GL_READY = true;
}





void getGpuHandlerReady(){

    if(isGpuHandlerReady) return;
    getGLReady();

    // setup vao
    {
        float verData[] = {-1.0f, 0.0f, 1.0f, 0.0f};
        unsigned int eleData[] = {0, 1};
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(2, bufs);
        glBindBuffer(GL_ARRAY_BUFFER, bufs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verData), verData, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(eleData), eleData, GL_STATIC_DRAW);
    }


    // setup shaders
    {
                
        int success;
        char infoLog[512];

        int vert = glCreateShader(GL_VERTEX_SHADER), frag = glCreateShader(GL_FRAGMENT_SHADER);
        
        // vertex Shader
        glShaderSource(vert, 1, &vertSrc, NULL);        stateVar = glGetUniformLocation(shaderId, "param");

        glCompileShader(vert);
        // print compile errors if any
        glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vert, 512, NULL, infoLog);
            LG("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl );
        };


        // fragment Shader
        glShaderSource(frag, 1, &fragSrc, NULL);
        glCompileShader(frag);
        // print compile errors if any
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(frag, 512, NULL, infoLog);
            LG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl );
        };


        shaderId = glCreateProgram();
        glAttachShader(shaderId, vert);
        glAttachShader(shaderId, frag);
        glLinkProgram(shaderId);
        glDetachShader(shaderId, vert);
        glDetachShader(shaderId, frag);
        glDeleteShader(vert);
        glDeleteShader(frag);

        glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
            LG("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl);
        }

        // setup uniforms
        stateVar = glGetUniformLocation(shaderId, "param");
        scalesVar = glGetUniformLocation(shaderId, "scales");
        weightsVar = glGetUniformLocation(shaderId, "weights");
    }

    isGpuHandlerReady = true;
};
