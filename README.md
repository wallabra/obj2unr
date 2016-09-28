# obj2unr
A brand new mesh converter between Wavefront .obj and Unreal Engine 1 ('98) .3d.
----

**WARNING: Not currently working and "guaranteed" to generate distortion!**

This is (going to be) a converter between Wavefront's .obj file format and Unreal's .3d. More specifically the Unreal Engine 1
from 1998 made by Epic Games, Inc. for Unreal 1 and Unreal Tournament (1999). It is designed with Blender in mind.

## Operational details

**Command-Line:**

    obj2unr <aniv file's name> <data file's name> <list of frames>
    
** List of frames **

The list of frames is a .txt (or any other extension) ASCII/UTF-8 file containing an ordered list of each frame as a relative
or absolute path to the frame's corresponding .obj file. Unreal's mesh format joins all frames in the aniv file and the
polygons in the data file.

## Technical format details

The Wavefront .obj file wasn't made with animation in mind, but rather pleadiness to the human eye in it's ASCII format (which I
find a bit not so unuseful since it's extremely hard to deduct visually a mesh from the numbers of the vertices and polygons).

The Unreal _a.3d (aniv) and _d.3d (data) files store important geometrical data about the mesh such as UV coordinates, vertex
coordinates and face's vertex indexes. The headers aren't really complicated either. The skin and classes are stored in
different files though.

These links contain more descriptive info about each format: [Wavefront .obj](https://en.wikipedia.org/wiki/Wavefront_.obj_file#File_format) and [Unreal's mesh format (.3d)](http://paulbourke.net/dataformats/unreal/).

(c) Gustavo Ramos "Gustavo6046" Rehermann, MIT License.
