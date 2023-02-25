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
