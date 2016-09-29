#include "mesh.hpp"
#include "umesh.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <stdint.h>

using std::cerr;
using std::ofstream;
using std::vector;
using std::stringstream;
using std::string;
using std::strcmp;

uint32_t unreal_vertex(double coord[3])
{
    uint32_t result; // allocation for any case

    result =   ( int(coord[0] * 8.0) & 0x7ff ) |
            ( ( int(coord[1] * 8.0) & 0x7ff ) << 11 ) |
            ( ( int(coord[2] * 4.0) & 0x3ff ) << 22 );

    return result;
}

unrealpoly poly_from_face(face other, mesh* base)
{
    unrealpoly res;

    for ( unsigned int i = 0; i < 3; i++ )
        for ( unsigned int j = 0; j < 2; j++ )
            res.mTex[i][j] = base->uv[other.uv[i] - 1][j] * 255;

    for ( unsigned int i = 0; i < 3; i++ )
    {
        if ( other.vertices[i] - 1 >= 0 )
            res.mVertex[i] = other.vertices[i] - 1;

        else
            res.mVertex[i] = base->vertices.size() - other.vertices[i];
    }

    return res;
}

void export_model(anim* outmodel, char* aniv_name, char* data_name, double* oscale)
{
    double scale;

    if ( oscale == NULL )
        scale = 1;

    else
        scale = *oscale;

    cerr << "Writing data file...\n";
    // export data
    unrealpolyh header;

    header.NumPolygons = outmodel->polys.size();
    header.NumVertices = outmodel->frames[0].vertices.size();

    FILE *fdata;
    fdata = fopen(data_name, "wb");

    fwrite(&header, sizeof(unrealpolyh), 1, fdata);
    cerr << "Wrote data header!\n";

    unrealpoly poly;

    for ( unsigned int i = 0; i < outmodel->polys.size(); i++ )
    {
        poly = poly_from_face(outmodel->polys[i], &outmodel->frames[0]);

        fwrite(&poly, sizeof(unrealpoly), 1, fdata);
    }

    cerr << "Wrote data file!\n";

    fclose(fdata);

    // export anim (aniv)
    cerr << "Writing aniv file...\n";

    FILE *faniv;
    faniv = fopen(aniv_name, "wb");

    // -header...
    int16_t numverts = outmodel->frames.size();
    fwrite(&numverts, sizeof(int16_t), 1, faniv);

    int16_t framesize = header.NumVertices * sizeof(unsigned long);
    fwrite(&framesize, sizeof(int16_t), 1, faniv);

    cerr << "Wrote aniv header!\n";

    // -...and vertexes!
    mesh* frame;
    double coord[3];
    uint32_t uvert;

    for ( unsigned int i = 0; i < outmodel->frames.size(); i++ )
    {
        frame = &outmodel->frames[i];

        for ( unsigned int v = 0; v < frame->vertices.size(); v++ )
        {
            for ( unsigned int j = 0; j < 3; j++ )
                coord[j] = frame->vertices[v][j] * scale;

            uvert = unreal_vertex(coord);
            fwrite(&uvert, sizeof(unsigned long), 1, faniv);
        }
    }

    fclose(faniv);

    cerr << "Wrote aniv file! Wrote " << outmodel->frames.size() << " frames and " << frame->vertices.size() << " vertices.\n";
}
