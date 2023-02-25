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

    void append(int index, audio::block blk);


    void setOffSet(int index=-1, int offset=DEF_BLOCKSIZE);


    void mix();


    audio::block getMixedData();

    ~gpuHandler();

private:

    struct stateData
    {
        int offsets[4];
        
    };
    unsigned int datas[4], sizes[4], offsets[4], fbo, fb_tex;
};
#endif // GPU_HANDLER_HPP
