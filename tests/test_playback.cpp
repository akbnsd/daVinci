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
#include <iostream>
#include <string>
#include <math.h>

using std::cout, std::cin, std::string;
#define PI 3.1415f

// audio playback using raw portaudio calls
int main()
{
    int err=0;
try{

    #define e(...) if(err = __VA_ARGS__)  throw std::exception() ;

    e(Pa_Initialize());

    PaStream* strm=0;
    float *data = new float[256];

    for(int i=0; i < 256; i++){
        *(data + i) = sin( 2.0f * PI * (float)i / 256.0f);
    }

    e(Pa_OpenDefaultStream(&strm, 0, 1, paFloat32, 44100, 256, NULL, NULL));
    e(Pa_StartStream(strm));

    for(char i =0; Pa_IsStreamActive(strm); i++)
    {
        Pa_WriteStream(strm, data, 256);
        Pa_Sleep(5);
    }

    e(Pa_CloseStream(strm));
    e(Pa_Terminate());

}
catch(std::exception){
    cout << "error occured" << Pa_GetErrorText(err);
    return 0;
}
    
}
