#ifndef IMPORTMESH_H
#define IMPORTMESH_H

#include <vector>
#include <iostream> //std::cout
#include <string>   //std::string
#include <sstream>  //std::stringstream
#include <fstream>  //std::fstream
#include <istream>  //std::istream

#include <QtOpenGL>
#include "Mesh.h"

using namespace std;

class importMesh
{
public:
    importMesh();
    void test();
    bool import(const char * path,
                 //Mesh &out_mesh,
                 vector<QVector3D> & out_vertices,
                 vector<QVector2D> & out_uvs,
                 vector<QVector3D> & out_normals,
                 vector<unsigned int> & _vertexIndices,
                 vector<unsigned int> & _uvIndices,
                 vector<unsigned int> & _normalIndices );
};

#endif // IMPORTMESH_H
