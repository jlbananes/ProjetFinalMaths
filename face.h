#ifndef FACE_H
#define FACE_H

#include <vector>
#include "Edge.h"

class Face
{
    private:
        vector<Edge*> edges;
    public:
        Face(Edge*, Edge*, Edge*);
        void operator=(const Face) ;
        bool operator==(const Face) const;
        vector<Edge*> getEdges(void) const;
        Edge* getEdge1(void) const;
        Edge* getEdge2(void) const;
        Edge* getEdge3(void) const;
        bool hasEdge(Edge*) const;
        bool swapEdges(Face* otherFace, const Edge* commonEdge);
};

#endif // FACE_H
