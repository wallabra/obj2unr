#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cstring>

#include "mesh.hpp"
#include "export.cpp"
#include "ssplit.cpp"

using std::cout;
using std::cin;
using std::vector;
using std::string;
using std::pair;
using std::ifstream;
using std::stringstream;
using std::strcmp;

struct stats
{
    unsigned int parsedlines;
    unsigned int parsedverts;
    unsigned int parseduvs;
    unsigned int parsedfaces;
    unsigned int totallines;
};

bool parseObjLine(string line, mesh* out, stats* out_stats)
{
    face newface;
    vector<double> curd;
    vector<string> pieces;
    string cur = "";

    // Split lines with space delimiter
    for ( unsigned int i = 0; i < line.size(); i++ )
    {
        if ( line[i] == ' ' )
        {
            pieces.push_back(cur);
            cur = "";
        }

        else
            cur += line[i];
    }

    if ( cur != "" )
        pieces.push_back(cur);

    // Check if vertex; if so, append to mesh vertex list
    if ( line.find("v ") == 0 )
    {
        if ( pieces.size() < 4 )
            return false;

        for ( unsigned int i = 0; i < 3; i++ )
            curd.push_back(strtod(pieces[i + 1].c_str(), 0));

        out->vertices.push_back(curd);

        out_stats->parsedverts++;
        return true;
    }

    // Check if texture UV vertex too
    else if ( line.find("vt") == 0 )
    {
        if ( pieces.size() < 3 )
            return false;

        for ( unsigned int i = 0; i < 2; i++ )
            curd.push_back(strtod(pieces[i + 1].c_str(), 0));

        out->uv.push_back(curd);

        out_stats->parseduvs++;
        return true;
    }

    //...and for faces
    else if ( line.find("f") == 0 )
    {
        if ( pieces.size() < 4 )
            return false;

        vector< vector<int> > indexes;
        vector<string> indexlist;
        string curr = "";

        for ( unsigned int i = 1; i < pieces.size(); i++ )
        {
            indexlist.clear();
            curr = "";

            for ( unsigned int j = 0; j < pieces[i].size(); j++ )
            {
                if ( pieces[i][j] == '/' )
                {
                    indexlist.push_back(curr);
                    curr = "";
                }

                else
                    curr += pieces[i][j];
            }

            if ( curr != "" )
            {
                indexlist.push_back(curr);
            }

            vector<int> new_index;

            new_index.push_back(atoi(indexlist[0].c_str()));
            new_index.push_back(atoi(indexlist[1].c_str()));

            indexes.push_back(new_index);
        }

        for ( int i = 0; i < 3; i++ )
        {
            newface.vertices[i] = indexes[i][0];
            newface.uv[i] = indexes[i][1];
        }

        out->faces.push_back(newface);

        out_stats->parsedfaces++;
        return true;
    }

    return false;
}

int main(int argc, char* argv[])
{
    anim animation;
    mesh model;
    stats statistic;

    // Title :3
    cerr << "===================================\nOBJ2UNR: Wavefront Converter for Unreal Engine 1\n\n   - by Gustavo6046 (Gustavo R. Rehermann)\n===================================\n\n";

    if ( argc < 4 )
    {
        if ( argc > 1 && strcmp(argv[1], "-txt") == 0 )
        {
            cerr << "The framelist (text file of any extension passed through stdin) must contain the relative or absolute path of each .obj file used as frames for the model.\ne.g.:\n\na.obj\nb.obj\nc.obj\n\nwould import three frames:\n\n1 -> a.obj\n2 -> b.obj\n3 -> c.obj\n\nin the order they have been put in the .txt project. You can use third-party tools to automate the task of creating the .txt file :^)\n     -Gustavo\n";

            return 0;
        }

        cerr << "Syntax: obj2unr *aniv name* *data name* *name of .txt file; see `obj2unr -txt` for more*\n";

        return 4;
    }

    string fnam;
    string line;

    ifstream otxt;
    otxt.open(argv[3]);
    std::getline(otxt, fnam);

    ifstream currfile;
    currfile.open(fnam.c_str());

    for (; std::getline(otxt, fnam) && currfile; )
    {
        // clean statistics
        statistic.parsedlines = 0;
        statistic.parsedverts = 0;
        statistic.parseduvs = 0;
        statistic.parsedfaces = 0;
        statistic.totallines = 0;

        // parse each of these FREAKING LINES
        for (; std::getline(currfile, line);  )
        {
            if ( parseObjLine(line, &model, &statistic) )
                statistic.parsedlines++;

            statistic.totallines++;
        }

        // add frame to animation
        animation.frames.push_back(model);

        if ( animation.frames.size() < 2 )
            animation.polys = model.faces;

        // log statistics
        cerr << "Imported model '" << fnam << "': Parsed " << statistic.totallines << " lines (" << statistic.totallines - statistic.parsedlines << " lines skipped), with " << statistic.parsedverts << " vertices, " << statistic.parseduvs << " UV coordinates and " << statistic.parsedfaces << " faces.\n";

        // open new file
        currfile.close();
        currfile.open(fnam.c_str());
    }

    currfile.close();

    if ( animation.frames.size() <= 0 )
    {
        cerr << "Error: Invalid model imported! (No imported frames detected!)";

        return 1;
    }

    if ( animation.polys.size() <= 0 )
    {
        cerr << "Error: Invalid model imported! (No polygons!)";

        return 2;
    }

    if ( animation.frames[0].vertices.size() <= 0 )
    {
        cerr << "Error: Invalid model imported! (No vertices in first frame!)";

        return 3;
    }

    for ( unsigned int i = 0; i < animation.polys.size(); i++ )
    {
        for ( unsigned int j = 0; j < 3; j++ )
            if ( animation.polys[i].vertices[j] > animation.frames[0].vertices.size() )
            {
                cerr << "Error: Invalid model imported! (Face's vertex index out of range!)";

                return 5;
            }

        for ( unsigned int j = 0; j < 2; j++ )
            if ( animation.polys[i].uv[j] > animation.frames[0].uv.size() )
            {
                cerr << "Error: Invalid model imported! (Face's UV index out of range!)";

                return 6;
            }
    }

    double real_scale;
    real_scale = 16;

    export_model(&animation, argv[1], argv[2], &real_scale);
    cerr << "Done converting model!";
}
