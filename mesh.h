#ifndef MESH_H
#define MESH_H

#include "Surface.h"

using namespace std;

class Mesh
{
    private:
        vector<Surface*> surfaces;
    public:
        Mesh(void);
        Mesh(const Mesh &mesh);
        Mesh(vector<Surface*>);
        void operator=(const Mesh) ;
        //bool operator==(const Surface) const;
        vector<Surface*> getSurfaces(void) const;
};

#endif // MESH_H
