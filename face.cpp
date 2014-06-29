#include "Face.h"

Face::Face()
{

}

Face::Face(const Face &face)
{
    this->edges[0] = face.edges[0];
    this->edges[1] = face.edges[1];
    this->edges[2] = face.edges[2];
}

Face::Face(Edge* AB,Edge* BC, Edge* CA)
{
    if (AB != BC && BC != CA)
    {
        this->edges.push_back(AB);
        this->edges.push_back(BC);
        this->edges.push_back(CA);
    }
}

void Face::operator=(const Face otherFace)
{
    this->edges = otherFace.getEdges();
}

bool Face::operator==(const Face otherFace) const
{
    return (   (getEdge1()==otherFace.getEdge1() && getEdge2()==otherFace.getEdge2() && getEdge3()==otherFace.getEdge3())
            || (getEdge1()==otherFace.getEdge1() && getEdge2()==otherFace.getEdge3() && getEdge3()==otherFace.getEdge2())
            || (getEdge1()==otherFace.getEdge2() && getEdge2()==otherFace.getEdge3() && getEdge3()==otherFace.getEdge1())
            || (getEdge1()==otherFace.getEdge2() && getEdge2()==otherFace.getEdge1() && getEdge3()==otherFace.getEdge3())
            || (getEdge1()==otherFace.getEdge3() && getEdge2()==otherFace.getEdge2() && getEdge3()==otherFace.getEdge1())
            || (getEdge1()==otherFace.getEdge3() && getEdge2()==otherFace.getEdge1() && getEdge3()==otherFace.getEdge2()) );
}

vector<Edge*> Face::getEdges(void) const
{
    return edges;
}

Edge* Face::getEdge1(void) const
{
    return this->edges[0];
}

Edge* Face::getEdge2(void) const
{
    return edges[1];
}

Edge* Face::getEdge3(void) const
{
    return edges[2];
}

bool Face::hasEdge(Edge* testedEdge) const
{
    if(*(edges[0])==*testedEdge || *(edges[1])==*testedEdge || *(edges[2])==*testedEdge)
    {
        return true;
    }
    return false;
}

// échange une arête de la face avec une arête d'une autre face (les deux faces partageant l'arête commonEdge)
bool Face::swapEdges(Face* otherFace, const Edge* commonEdge)
{
    for(unsigned short i=0; i<3; i++)
    {
        if (getEdges()[i] != commonEdge)
        {
            for(unsigned short j=0; j<3; j++)
            {
                Point* newCommonPoint;
                if (otherFace->getEdges()[j]!=commonEdge && !(getEdges()[i]->commonPoint(otherFace->getEdges()[j],newCommonPoint)))
                {
                    Edge* temp = otherFace->getEdges()[j];
                    otherFace->getEdges()[j] = getEdges()[i];
                    getEdges()[i] = temp;
                }
            }
        }
    }
    return false;
}
