#ifndef SURFACE_H
#define SURFACE_H

#include "Face.h"

using namespace std;

class Surface
{
    private:
        vector<Face*> faces;
    public:
        Surface(void);
        Surface(const Surface &surface);
        Surface(vector<Face*>);
        void operator=(const Surface);
        //bool operator==(const Surface) const;
        vector<Face*> getFaces(void) const;
};

#endif // SURFACE_H
