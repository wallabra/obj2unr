#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <vector>
using std::vector;

struct face
{
    unsigned short uv[3];
    unsigned short vertices[3];
};

struct mesh
{
    vector<face> faces;
    vector< vector<double> > vertices;
    vector< vector<double> > uv;
};

struct anim
{
    vector<face> polys;
    vector<mesh> frames;
};

#endif // MESH_HPP_INCLUDED
