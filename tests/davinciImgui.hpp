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


#define TOPI 6.283f
#include "math.h"
#include "implot.h"
#include "ui/graphElem.hpp"

using namespace ImGui;
using namespace ImPlot;

int WINDOWFLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
bool record=false, playback=false, superposePlay=false, superPoseStates[4] = {0, 0, 0, 0};
int recordSink=0, playbackSrc=0, clearIndex=0,sineDest=0;
float freq=100.0f, speed=1.0f;
gpuHandler& hand = *handler;
audio::block* blk;
float timeCount=0;
graphElem* graphs[4];
graphElem* result;

bool uiInitScript(){
    for(int i=0; i < 4; i++) graphs[i] = new graphElem(handler, i, std::to_string(i));
    result = new graphElem(handler, -1, "superpose");
    return true;
};


void renderPlots(){

    if(!Begin("plots", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize )) return;

    for(int i=0; i < 4; i++){
        graphs[i]->render();
        SameLine();
    }

    memcpy(handler->states, superPoseStates, 4 * sizeof(bool));
    result->render();
    End();
};


void imguiScript(){

    static bool isUiInit=false;
    if(!isUiInit) isUiInit = uiInitScript();

    static float dt = ImGui::GetIO().DeltaTime;
    timeCount+=dt;

    if(!Begin("DaVinci", NULL,  WINDOWFLAGS)) return;

    if(Button(record ? "stop recording" : "start Recording")){
        record ? audio::stopRec() : audio::startRec();

        record = !record;
    };

    SameLine();
    PushItemWidth(-1);
    SliderInt("to", &recordSink, 0, 3);
    if(Button(playback ? "stop playback  " : "start playback ")){
        playback ? audio::stopPlayback() : audio::startPlayback();
        playback = !playback;
    };

    SameLine();PushItemWidth(-1);
    SliderInt("from", &playbackSrc, 0, 3);


    if(Button("Clear")) handler->setToClear(clearIndex);
    SameLine();PushItemWidth(-1);
    SliderInt("clearIndex", &clearIndex, 0, 3);

    Spacing();
    Text("Toggle Streams");
    Checkbox("0", superPoseStates + 0); SameLine();
    Checkbox("1", superPoseStates + 1); SameLine();
    Checkbox("2", superPoseStates + 2); SameLine();
    Checkbox("3", superPoseStates + 3); SameLine();

    if(Button(superposePlay ? "stop superpose  " : "start superpose ")){
        superposePlay ? audio::stopPlayback() : audio::startPlayback();
        superposePlay = !superposePlay;
    };


    Spacing();
    Text("add sine wave");

    PushItemWidth(-1);
    SliderInt("sine to", &sineDest, 0, 3);
    if(Button("add sine wave")){
        audio::block* blk = audio::getBlock();
        for(int i =0; i < DEF_BLOCKSIZE; i++){
            blk->data[i] = sinf(TOPI * freq * i / SAMPLE_RATE);
        }
        handler->clearData(sineDest, *blk);
        audio::dumpQue->push(blk);
    }
    SameLine();
    DragFloat("frequency", &freq, 1.0f, 0.0f, FLT_MAX); SameLine();

    End();

    renderPlots();

    if(playback) handler->setOffSet(playbackSrc, dt * SAMPLE_RATE);
    if(superposePlay) handler->setOffSet(-1, dt * SAMPLE_RATE);

    if(superposePlay && !audio::outQue->size()){
        memcpy(handler->states, superPoseStates, 4 * sizeof(bool));
        blk = handler->getData();
        audio::outQue->push(blk);
        timeCount=0;
    }

    else if(playback && !audio::outQue->size()){
        blk = handler->getData(playbackSrc);
        audio::outQue->push(blk);
        timeCount=0;
    }

    else if(record && audio::inQue->size()){
            handler->offsets[recordSink]= handler->sizes[recordSink];
            blk = audio::inQue->front();
            audio::inQue->pop();
            handler->append(recordSink, *blk);
            audio::dumpQue->push(blk);
    }


}

