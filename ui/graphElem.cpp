
#include "graphElem.hpp"
#include "imgui.h"
#include "implot.h"

float graphElem::xs[DEF_BLOCKSIZE];


graphElem::graphElem(gpuHandler* hand, int index, std::string title) : handler(hand), srcIndex(index), label(title) {
    static bool init=false;
    if(!init){
        for(int i=0; i < DEF_BLOCKSIZE; i++){
            xs[i] = i * 1.0f / (float) DEF_BLOCKSIZE;
        }
        init = true;
    }
    scrollLabel = label + "-scroll";
    plotLabel = label + "-plot";
}


using namespace ImGui;
using namespace ImPlot;

ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;


void graphElem::render(){
    if(!Begin(label.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar)) return;
    BeginPlot(label.c_str(), ImVec2(-1, 0), ImPlotFlags_NoTitle);

    audio::block* blk = handler->getData(srcIndex);
 
    SetupAxes(NULL, NULL, flags, flags | ImPlotAxisFlags_AutoFit);
    SetupAxisLimits(ImAxis_X1, 0, 1, ImGuiCond_Always);
    SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);

    PlotShaded(plotLabel.c_str(), xs, blk->data, DEF_BLOCKSIZE);
    EndPlot();

    toff = ((float)handler->offsets[srcIndex]) / ((float)SAMPLE_RATE);
    
    PushItemWidth(-1);
    if(DragFloat(scrollLabel.c_str(), &toff, 0.001F, 0.0f, FLT_MAX, "%.3f")){
        handler->offsets[srcIndex] = toff * SAMPLE_RATE;
    }

    End();
};

