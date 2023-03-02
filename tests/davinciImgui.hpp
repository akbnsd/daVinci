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
bool record=false, playback=false;
int recordSink=0, playbackSrc;
audio::block* blk;
gpuHandler& hand = *handler;
float timeCount=0;

void imguiScript(){

    static float dt = ImGui::GetIO().DeltaTime;
    timeCount+=dt;

    if(!Begin("DaVinci", NULL,  WINDOWFLAGS)) return;

    if(Button(record ? "stop recording" : "start Recording")){
        record ? audio::stopRec() : audio::startRec();

        record = !record;
    };

    SameLine();
    SliderInt("to", &recordSink, 0, 3);

    if(Button(playback ? "stop playback" : "start playback")){
        playback ? audio::stopPlayback() : audio::startPlayback();
        playback = !playback;
    };

    SameLine();
    SliderInt("from", &playbackSrc, 0, 3);
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


    
    End();


    if(timeCount > 0.07){

        if(playback && audio::outQue->size() <=2){
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
        timeCount = 0;
    };
}



void rederPlots(){

    if(!Begin("plots", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize )) return;

    
    End();
};
