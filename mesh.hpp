#ifndef MESH_HPP_INCLUDED
#define MESH_HPP_INCLUDED

#include <vector>
using std::vector;

struct face
{
    int uv[3];
    int vertices[3];
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
