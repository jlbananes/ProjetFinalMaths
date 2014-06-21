#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include "Face.h"

class Surface
{
    private:
        vector<Face*> faces;
    public:
        Surface(vector<Face*>);
        void operator=(const Surface);
        //bool operator==(const Surface) const;
        vector<Face*> getFaces(void) const;
};

#endif // SURFACE_H
