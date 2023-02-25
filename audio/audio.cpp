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

#include "portaudio.h"

#include "audio.hpp"

std::queue<audio::block*> *audio::ques, *audio::outQue, *audio::inQue, *audio::dumpQue;


bool paErr=0;
PaStream* outStream = 0, *inStream=0;


// std::queue<audio::block*> *audio::ques=0, &outQue=0, &inQue=0, &dumpQue=0;


int playBackCallback(
    const void *input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    // check for data
    if(!audio::outQue->size()) return paContinue;

    // fetch block
    audio::block* blk = audio::outQue->front();
    audio::outQue->pop();

    float* data = blk->data;
    
    float* out = (float*) output;
    for(int i =0 ; i < frameCount; i++){
        *(out++) = *(data++);
    };

    audio::dumpQue->push(blk);
    return paContinue;
};



int recCallback(
    const void *input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    audio::block* blk = audio::getBlock();
    audio::inQue->push(blk);

    float* data = blk->data, *inp = (float*)input;
    for(int i=0; i < frameCount; i++){
        *(data++) = *(inp++);
    }

    return paContinue;
};


bool audio::init(){
    // initialize portaudio
    if(paErr = Pa_Initialize()) return !paErr;

    // create input and output streams 
    Pa_OpenDefaultStream(&outStream, 0, 1, paFloat32, 44100, DEF_BLOCKSIZE, playBackCallback, ques);
    Pa_OpenDefaultStream(&inStream, 1, 0, paFloat32, 44100, DEF_BLOCKSIZE, recCallback, ques);

    // initialize handlers
    ques = new std::queue<audio::block*>[3];
    outQue = &ques[0];
    inQue = &ques[1];
    dumpQue = &ques[2];
    return !paErr;
};


bool audio::startPlayback(){
    return !Pa_StartStream(outStream);
};

bool audio::stopPlayback(){
    return Pa_StopStream(outStream);
};

bool audio::startRec(){
    return !Pa_StartStream(inStream);
};


bool audio::stopRec(){
    return !Pa_StopStream(inStream);
};



audio::block* audio::getBlock(){
    if(dumpQue->size()){
        audio::block* blk = dumpQue->front();
        dumpQue->pop();
        return blk;
    }

    return new audio::block();
}


bool audio::terminate(){
    Pa_CloseStream(inStream);
    Pa_CloseStream(outStream);

    Pa_Terminate();
    return true;
};
