#include "Mesh.h"

Mesh::Mesh(vector<Surface*> inputSurfaces)
{
    this->surfaces = inputSurfaces;
}

void Mesh::operator=(const Mesh otherMesh)
{
    this->surfaces = otherMesh.getSurfaces();
}

/*bool Mesh::operator==(const Surface) const
{
    return ;
}*/

vector<Surface*> Mesh::getSurfaces(void) const
{
    return this->surfaces;
}
