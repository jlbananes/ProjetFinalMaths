#include "Surface.h"
#include <cstddef>

Surface::Surface(void)
{
    faces = {NULL};
}

Surface::Surface(const Surface &surface)
{
    for(unsigned int i = 0; i < surface.faces.size(); ++i)
    {
        this->faces[i] = surface.faces[i];
    }
}

Surface::Surface(vector<Face*> inputFaces)
{
    faces = inputFaces;
}

void Surface::operator=(const Surface otherSurface)
{
    faces = otherSurface.getFaces();
}

/*bool Surface::operator==(const Surface) const
{
    return ()
}*/

vector<Face*> Surface::getFaces(void) const
{
    return faces;
}
