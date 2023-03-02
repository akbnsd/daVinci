


#include "mixer/gpuHandler.hpp"


class graphElem
{
public:
    graphElem(gpuHandler* hand, int index, std::string);
    void render();

    bool isActive=false;
private:

    gpuHandler* handler;
    int srcIndex;
    std::string scrollLabel,plotLabel,label;
    float toff=0;

    static float xs[DEF_BLOCKSIZE];

};
