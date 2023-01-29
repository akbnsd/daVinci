#include "pch.hpp"
#include "math.h"

#include "audio/audio.hpp"



#define PI 3.1415f

// audio playback using audio context
int main(){
    
    float* data = new float[512];
    float* ptr = data;
    for(int i = 0; i < 512; i++){
        *(ptr++) = sin(4.0f * PI * (float)i / 512.0f);
    }



    bool status = audio::init();
    audio::startPlayback();
    audio::block* blk;
    while (true)
    {
        blk = audio::getBlock();
        
        for(int i=0; i < 512; i++){
            *(blk->data+i) = *(data+i);
        }

        audio::outQue->push(blk);
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    

    return 0;
}
