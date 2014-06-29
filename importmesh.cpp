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

void importMesh::test()
{

    string str("1/2/3 a/b/c\n4/5/6 d/e/f\n7/8/9");
    ifstream istr(str);

    char* cstr = new char[str.length()+1];
    strcpy(cstr,str.c_str());

    char* line = strtok(cstr,"\n");
    while(line!=0)
    {
        cout << line << endl;
        line = strtok(NULL, "\n");
    }

    stringstream ss(str);
    string s1, s2;
    ss >> s1 >> s2;
    while (s1.find('/')!=std::string::npos)
    { s1.at(s1.find('/')) = ' '; }
    while (s2.find('/')!=std::string::npos)
    { s2.at(s2.find('/')) = ' '; }
    stringstream ss1, ss2;
    ss1.str(s1);
    ss2.str(s2);
    int u, d, t;
    char a, b, c;
    ss1 >> u >> d >> t;
    ss2 >> a >> b >> c;
    cout << u << " " << d << " " << t << endl;
    cout << a << " " << b << " " << c << endl;

}


bool importMesh::import( const char * path,
                        //Mesh &out_mesh,
                        vector<QVector3D> & out_vertices,
                        vector<QVector2D> & out_uvs,
                        vector<QVector3D> & out_normals,
                        vector<unsigned int> & _vertexIndices,
                        vector<unsigned int> & _uvIndices,
                        vector<unsigned int> & _normalIndices )
{

    fstream file;
    string fullFile;
    string line;

    Surface* currentSurface = NULL;
    Face* currentFace = NULL;

    unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

    try
    {
        file.open(path, fstream::in);
    }
    catch(exception e)
    {
        cerr << "ouverture impossible" << e.what() << endl;
        return false;
    }

    if (file)
    {
        file.seekg (0, file.end);
        int length = file.tellg();
        file.seekg (0, file.beg);

        char * buffer = new char [length];

        file.read (buffer,length);

        file.close();

        fullFile = buffer;

        delete[] buffer;
    }
    else
    {
        return false;
    }

    char* cstr = new char[fullFile.length()+1];
    strcpy(cstr,fullFile.c_str());

    char* ln = strtok(cstr,"\n");
    while(ln!=0)
    {
        line = ln;

        if(line.compare(0,1,"v") == 0 )
        {
            stringstream ss(line.substr(2));
            float x, y, z;
            ss >> x >> y >> z;
            QVector3D vertex(x,y,z);
            out_vertices.push_back(vertex);
        }
        else if(line.compare(0,2, "vt" ) == 0)
        {
            stringstream ss(line.substr(3));
            float u, v;
            ss >> u >> v;
            QVector2D uv(u, v);
            out_uvs.push_back(uv);
        }
        else if(line.compare(0,1, "s") == 0)
        {
            /*if(line.compare(2,1, "0") == 0 || line.compare(2,3, "off") == 0)
            {

            }*/
            Surface newSurface;
            //out_mesh.addSurface();
            currentSurface = &newSurface;
        }
        else if(line.compare(0,2, "vn") == 0)
        {
            stringstream ss(line.substr(3));
            float x, y, z;
            ss >> x >> y >> z;
            QVector3D normal(x, y, z);
            out_normals.push_back(normal);
        }
        else if(line.compare(0,1, "f") == 0)
        {
            stringstream ss(line.substr(2));
            string sp1, sp2, sp3;
            ss >> sp1 >> sp2 >> sp3;

            while (sp1.find('/')!=std::string::npos)
            { sp1.at(sp1.find('/')) = ' '; }
            while (sp2.find('/')!=std::string::npos)
            { sp2.at(sp2.find('/')) = ' '; }
            while (sp3.find('/')!=std::string::npos)
            { sp3.at(sp3.find('/')) = ' '; }

            stringstream ssp1, ssp2, ssp3;
            ssp1.str(sp1);
            ssp2.str(sp2);
            ssp3.str(sp3);

            unsigned int vp1, up1, np1, vp2, up2, np2, vp3, up3, np3;
            ssp1 >> vp1 >> up1 >> np1;
            ssp2 >> vp2 >> up2 >> np2;
            ssp3 >> vp3 >> up3 >> np3;

            vertexIndex[0] = vp1; vertexIndex[1] = vp2; vertexIndex[2] = vp3;
            uvIndex[0] = up1; uvIndex[1] = up2; uvIndex[2] = up3;
            normalIndex[0] = np1; normalIndex[1] = np2; normalIndex[2] = np3;

            _vertexIndices.push_back(vertexIndex[0]);
            _vertexIndices.push_back(vertexIndex[1]);
            _vertexIndices.push_back(vertexIndex[2]);
            _uvIndices.push_back(uvIndex[0]);
            _uvIndices.push_back(uvIndex[1]);
            _uvIndices.push_back(uvIndex[2]);
            _normalIndices.push_back(normalIndex[0]);
            _normalIndices.push_back(normalIndex[1]);
            _normalIndices.push_back(normalIndex[2]);
        }
        ln = strtok(NULL, "\n");
        line.clear();
    }

    delete[] cstr;
    return true;
//*/
}

