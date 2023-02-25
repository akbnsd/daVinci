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
#include "math.h"

#include "audio/audio.hpp"



#define PI 3.1415f

// audio playback using audio context
int main(){
    
    float* data = new float[DEF_BLOCKSIZE];
    float* ptr = data;
    for(int i = 0; i < DEF_BLOCKSIZE; i++){
        *(ptr++) = sin(210 * PI * (float)i / DEF_BLOCKSIZE);
    }



    bool status = audio::init();
    audio::startPlayback();
    audio::block* blk;
    while (true)
    {
        blk = audio::getBlock();
        
        for(int i=0; i < DEF_BLOCKSIZE; i++){
            *(blk->data+i) = *(data+i);
        }

        audio::outQue->push(blk);
        std::this_thread::sleep_for(std::chrono::milliseconds(46));
    }
    

    return 0;
}
