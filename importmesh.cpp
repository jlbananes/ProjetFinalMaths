/**********************
 * Source :
 * http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
 * http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
 * http://www.gamedev.net/topic/312335-obj-model-loading-in-detail/
 * *******************/

#include "importmesh.h"

importMesh::importMesh()
{
}

bool importMesh::import( const char * path,
                        vector<QVector3D> & out_vertices,
                        vector<QVector2D> & out_uvs,
                        vector<QVector3D> & out_normals)
{
    //Create instance
    vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    vector<QVector3D> temp_vertices;
    vector<QVector2D> temp_uvs;
    vector<QVector3D> temp_normals;

    FILE * file = fopen(path, "r");
    if(file == NULL)
    {
        printf("Error opening file");
        return false;
    }

    while( 1 )
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
                break;

        if(strcmp(lineHeader, "v") == 0 )
        {
            QVector3D vertex;
            //fscanf(file, "%f %f %f\n", (float)vertex.x(), (float)vertex.y(), (float)vertex.z());
            temp_vertices.push_back(vertex);
        }
        else if( strcmp(lineHeader, "vt" ) == 0)
        {
            QVector2D uv;
            //fscanf(file, "%f %f\n", (float)uv.x(), (float)uv.y());
            temp_uvs.push_back(uv);
        }
        else if( strcmp(lineHeader, "vn") == 0)
        {
            QVector3D normal;
            //fscanf(file, "%f %f %f\n", (float)normal.x(), (float)normal.y(), (float)normal.z());
            temp_normals.push_back(normal);
        }
        else if( strcmp(lineHeader, "f") == 0)
        {
            char* vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[2], normalIndex[3];
            //int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            int matches = 0;
            if(matches != 9)
            {
                printf("File can't be read by our simple parser\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else
        {
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    for(unsigned int i = 0; i<vertexIndices.size(); i++)
    {
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        QVector3D vertex = temp_vertices[ vertexIndex -1 ];
        QVector2D uv = temp_uvs[uvIndex -1];
        QVector3D normal = temp_normals[normalIndex-1];

        out_vertices.push_back(vertex);
        out_uvs.push_back(uv);
        out_normals.push_back(normal);
    }

    return true;
}

