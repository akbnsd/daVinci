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



#include "pch.hpp"
#include <portaudio.h>
#include "audioHandler.hpp"

bool init=false;
PaStream* stream = 0;



audioHandler::audioHandler(audioHandler::recordCallback rec) : m_callback{rec} {

    if( !init ){
        init = true;
        int err = 0;

        if(err = Pa_Initialize()) {
            LG(Pa_GetErrorText(err));
            return;
        };

        err = Pa_OpenDefaultStream(&stream, 1, 1, paInt32, 44100, 1024, NULL, NULL);
        if(err) {
            LG(Pa_GetErrorText(err));
            return;
        }

        if( err =Pa_StartStream(stream) ){
            LG(Pa_GetErrorText(err));
            return;
        }

        LG("portaudio stream started successfully\n");
    }
};

void audioHandler::play(int* data, unsigned long size){
    int err = Pa_WriteStream(stream, data, size);
    if(err) LG(Pa_GetErrorText(err));
}



void audioHandler::toggleRecording(int state){}
void audioHandler::setRecordCallback(recordCallback callback){};
