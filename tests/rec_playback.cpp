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
#include "audio/audio.hpp"


// records a 5 second clip and plays it back
int main(){
    audio::init();
    std::cout << "starting recording a 5 sec clip \n";
    audio::startRec();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    audio::stopRec();
    std::cout << "recording stopped\n";


    std::this_thread::sleep_for(std::chrono::milliseconds(2000));


    std::cout << "starting playback\n";
    audio::startPlayback();
    audio::block* blk;
    while (audio::inQue->size())
    {
        blk = audio::inQue->front();
        audio::inQue->pop();

        audio::outQue->push(blk);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "playback stopped\n";
    audio::stopPlayback();
    audio::terminate();
    return 0;
};
