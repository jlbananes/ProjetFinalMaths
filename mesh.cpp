#include "Mesh.h"
#include <cstddef>

using namespace std;

Mesh::Mesh(void)
{
    surfaces = {NULL};
}

Mesh::Mesh(const Mesh &mesh)
{
    for(unsigned int i = 0; i < mesh.surfaces.size(); ++i)
    {
        this->surfaces[0] = mesh.surfaces[0];
    }
}

Mesh::Mesh(vector<Surface*> inputSurfaces)
{
    surfaces = inputSurfaces;
}

void Mesh::operator=(const Mesh otherMesh)
{
    surfaces = otherMesh.getSurfaces();
}

vector<Surface*> Mesh::getSurfaces(void) const
{
    return surfaces;
}
