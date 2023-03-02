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

#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "pch.hpp"
#define DEF_BLOCKSIZE 4410
#define SAMPLE_RATE   44100

class audio
{
public:

    // data structure to pass data to device
    struct block
    {
        float data[DEF_BLOCKSIZE];
        int seq = 0;
    };
    
    // initialize audio context
    static bool  init();

    // start / stop playback
    static bool  startPlayback();
    static bool  stopPlayback();

    // start / stop recording
    static bool  startRec();
    static bool  stopRec();

    // get blank blocks of data to write data into
    static block*  getBlock();

    // terminate audio context
    static bool  terminate();

    // pointers to handle audio streams
    static std::queue<block*> *ques, *outQue, *inQue, *dumpQue;
};
#endif //  AUDIO_HPP
