# obj2unr
A brand new mesh converter between Wavefront .obj and Unreal Engine 1 ('98) .3d.
----

**WARNING: Not currently working and *guaranteed* to generate distortion!**

This is (going to be) a converter between Wavefront's .obj file format and Unreal's .3d. More specifically the Unreal Engine 1
from 1998 made by Epic Games, Inc. for Unreal 1 and Unreal Tournament (1999). It is designed with Blender in mind.

## Operational details

**Command-Line:**

    obj2unr <aniv file's name> <data file's name> <list of frames>
    
**List of frames**

The list of frames is a .txt (or any other extension) ASCII/UTF-8 file containing an ordered list of each frame as a relative
or absolute path to the frame's corresponding .obj file. Unreal's mesh format joins all frames in the aniv file and the
polygons in the data file.

## Technical format details

The Wavefront .obj file wasn't made with animation in mind, but rather pleadiness to the human eye in it's ASCII format (which I
find a bit not so useful since it's extremely hard to deduct visually a mesh from the numbers of the vertices and polygons).

The Unreal `_a.3d` (aniv) and `_d.3d` (data) files store important geometrical data about the mesh such as UV coordinates, vertex
coordinates and face's vertex indexes. The headers aren't really complicated either. The skin and classes are stored in
different files though.

These links contain more descriptive info about each format: [Wavefront .obj](https://en.wikipedia.org/wiki/Wavefront_.obj_file#File_format) and [Unreal's mesh format (.3d)](http://paulbourke.net/dataformats/unreal/).

## Technical code information

To achieve animation from Wavefront's static format the program uses the first frame's polygons only, and parse the vertices from all listed Wavefront .obj files, each one as a frame. The texture coordinates (aka UV coordinates; `vt` in Wavefront) are parsed as well. 

Then the polygons are converted into Unreal's `_d.3d` format using a C++ struct:

    struct unrealpoly
    {
        unsigned short mVertex[3];
        static const char mType = 1;
        static const char mColor = 128;
        unsigned char mTex[3][2];
        static const char mTextureNum = 0;
        static const char mFlags = 0;
    };
    
Not before the header (which is also a struct, by the way):

    struct unrealpolyh
    {
        unsigned short NumPolygons, NumVertices;
        static const unsigned short UnusedS[2];
        static const unsigned long UnusedL[7];
        static const unsigned char UnusedC[12];
    };
    
Then the vertices are also converted, frame per frame. The header is a short (number of vertices) followed by another short (number of frames multiplied by the byte size of unsigned long (`sizeOf(unsigned long)` in C++ and `struct.calcsize("L")` in Python)). Then each vertice's 3 XYZ coordinates are converted into a single `unsigned long` using Unreal's algorithm (which involves byte operations):

    unsigned long unreal_vertex(double coord[3])
    {
        unsigned long result; // allocation for any case

        result =   ( int(coord[0] * 8.0) & 0x7ff ) |
                ( ( int(coord[1] * 8.0) & 0x7ff ) << 11 ) |
                ( ( int(coord[2] * 4.0) & 0x3ff ) << 22 );

        return result;
    }
    
## Organizing Projects

It can be easy to do small project-like folder structues:

    > obj2unr.exe
    \ projects
        \ descriptive project name
            > config.txt
            \ out //the following files would be the files obj2unr generates
                > example_a.3d
                > example_d.3d

            \ collada
                > example_01.obj
                > example_02.obj
                [...]

        \ other project
            [...]

        [...]

Being a `>` prefix a file and a `\` prefix a folder level in.

Then do the following command line:

    obj2unr projects\<projname>\out\example_a.3d projects\<projname>\out\example_d.3d projects\<projname>\config.txt

In the future I'll write a small Python application (`.py`) that simply executes the program based on that project structure.

*(c) Gustavo Ramos "Gustavo6046" Rehermann, MIT License.*
