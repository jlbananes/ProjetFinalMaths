#include "Surface.h"


Surface::Surface(vector<Face*> inputFaces)
{
    this->faces = inputFaces;
}

void Surface::operator=(const Surface otherSurface)
{
    this->faces = otherSurface.getFaces();
}

/*bool Surface::operator==(const Surface) const
{
    return ()
}*/

vector<Face*> Surface::getFaces(void) const
{
    return this->faces;
}
