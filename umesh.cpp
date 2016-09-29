#include <stdint.h>

struct unrealpolyh
{
    uint16_t NumPolygons, NumVertices;
    static const uint16_t UnusedS[2];
    static const uint32_t UnusedL[7];
    static const uint8_t UnusedC[12];
};

struct unrealpoly
{
    uint16_t mVertex[3];
    static const uint8_t mType = 1;
    static const uint8_t mColor = 128;
    uint8_t mTex[3][2];
    static const uint8_t mTextureNum = 0;
    static const uint8_t mFlags = 0;
};
