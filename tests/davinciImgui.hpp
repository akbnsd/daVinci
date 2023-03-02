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



#include "implot.h"
#include "ui/graphElem.hpp"

using namespace ImGui;
using namespace ImPlot;

int WINDOWFLAGS = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
bool record=false, playback=false, superposePlay=false, superPoseStates[4] = {0, 0, 0, 0};
int recordSink=0, playbackSrc=0;
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

    SameLine();
    PushItemWidth(-1);
    SliderInt("from", &playbackSrc, 0, 3);

    // LabelText("Enable / Disable:");
    Checkbox("0", superPoseStates + 0); SameLine();
    Checkbox("1", superPoseStates + 1); SameLine();
    Checkbox("2", superPoseStates + 2); SameLine();
    Checkbox("3", superPoseStates + 3); SameLine();

    if(Button(superposePlay ? "stop superpose  " : "start superpose ")){
        superposePlay ? audio::stopPlayback() : audio::startPlayback();
        superposePlay = !superposePlay;
    };

    End();

    renderPlots();

    if(playback) handler->setOffSet(playbackSrc, dt * SAMPLE_RATE);

    if(superposePlay && audio::outQue->size() <=4){
        memcpy(handler->states, superPoseStates, 4 * sizeof(bool));
        for(int i=0; i < 4; i++){
            blk = handler->getData();
            audio::outQue->push(blk);
            handler->setOffSet();
        }
    }

    else if(playback && audio::outQue->size() <=4){
        for(int i=0; i < 4; i++){
            blk = handler->getData(playbackSrc);
            audio::outQue->push(blk);
            handler->setOffSet(playbackSrc);
        }
    }

    else if(record && audio::inQue->size() >= 3){
        for(int i=audio::inQue->size(); i>0; i--){
            blk = audio::inQue->front();
            audio::inQue->pop();
            handler->append(recordSink, *blk);
        }

        handler->offsets[recordSink]=0;
    }


}

