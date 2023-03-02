#include "ui/graphElem.hpp"
#include "ui/imguiRenderer.hpp"
#include "audio/audio.hpp"
#include "mixer/gpuHandler.hpp"

int main(){

    glfwInit();
    audio::init();

    int monitorWidth=0, monitorHeight=0;
    glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), NULL, NULL, &monitorWidth, &monitorHeight);
    GLFWwindow* window =  glfwCreateWindow(monitorWidth, monitorHeight, "Davinci Test", glfwGetPrimaryMonitor(), NULL);

    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        LG("error initializing glad\n");
        return 1;
    }

    gpuHandler hand;
    
    std::cout << "starting recording a 5 sec clip \n";
    audio::startRec();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    audio::stopRec();
    std::cout << "recording stopped\n";

    audio::block* blk;
    for(int i=audio::inQue->size(); i > 0; i--){
        blk = audio::inQue->front();
        audio::inQue->pop();
        hand.append(0, *blk);
        audio::dumpQue->push(blk);
    };

    std::cout << "starting playback\n";
    audio::startPlayback();
    while(true)
    {
        blk = hand.getData(0);
        hand.setOffSet(0);
        audio::outQue->push(blk);
        std::this_thread::sleep_for(std::chrono::milliseconds(98));
    }

    std::cout << "playback stopped\n";
    audio::stopPlayback();
    audio::terminate();
    return 0;

}
