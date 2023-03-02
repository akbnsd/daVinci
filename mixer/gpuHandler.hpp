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



#ifndef GPU_HANDLER_HPP
#define GPU_HANDLER_HPP

#include  "audio/audio.hpp"

class gpuHandler
{
public:
    gpuHandler();

    void setActive(int index, int state=-1);
    
    void append(int index, audio::block& blk);

    void clearData(int index, audio::block& blk);

    // remove these
    void clearActiveState() { *(unsigned int*)states = 0; };
    void enableAll() { *(unsigned int*)states = 0x01010101; };

    void setOffSet(int index=-1, int offset=DEF_BLOCKSIZE);

    audio::block* getData(int index=-1, int offset=-1, int count=DEF_BLOCKSIZE);

    void mix(audio::block& blk);

    ~gpuHandler();
    
public:
    unsigned int offsets[4] = {0, 0, 0, 0}, activeCount=0;
    bool states[4] = {0, 0, 0, 0};

private:
    unsigned int datas[4] = {0, 0, 0, 0}, sizes[4] = {0, 0, 0, 0}, fbo, fb_tex;
};
#endif // GPU_HANDLER_HPP
