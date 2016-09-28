struct unrealpolyh
{
    unsigned short NumPolygons, NumVertices;
    static const unsigned short UnusedS[2];
    static const unsigned long UnusedL[7];
    static const unsigned char UnusedC[12];
};

struct unrealpoly
{
    unsigned short mVertex[3];
    static const char mType = 1;
    static const char mColor = 128;
    unsigned char mTex[3][2];
    static const char mTextureNum = 0;
    static const char mFlags = 0;
};
