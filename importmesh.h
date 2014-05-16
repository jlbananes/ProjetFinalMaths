#ifndef IMPORTMESH_H
#define IMPORTMESH_H

#include <vector>
#include <stdio.h>
#include <QtOpenGL>

using namespace std;

class importMesh
{
public:
    importMesh();
    bool import( const char * path,
                 vector<QVector3D> & out_vertices,
                 vector<QVector2D> & out_uvs,
                 vector<QVector3D> & out_normals);
};

#endif // IMPORTMESH_H
