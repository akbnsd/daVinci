


#include "mixer/gpuHandler.hpp"


class graphElem
{
public:
    graphElem(gpuHandler& hand, int index, const char* title);

    void render();
private:

    gpuHandler& handler;
    int srcIndex;
    const char* label;
    float toff=0;

    static float xs[DEF_BLOCKSIZE];

};
