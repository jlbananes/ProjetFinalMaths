#ifndef UTILS_H
#define UTILS_H

#include "Mesh.h"

namespace Utils
{
    void CenterCircumsribedSphere(const Face*, Point&);
    bool commonEdge(const Face*,const Face*,Edge*);
    bool DelaunayCriterium(const Face*,const Face*,const Edge*);
}

#endif // UTILS_H
;
