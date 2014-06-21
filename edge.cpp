#include "Edge.h"
#include <cmath>

Edge::Edge(Point* A, Point* B)
{
    if (A != B)
    {
        this->points.push_back(A);
        this->points.push_back(B);
    }
}

void Edge::operator=(const Edge otherEdge)
{
    this->points = otherEdge.points;
}

bool Edge::operator==(const Edge otherEdge) const
{
    return (  (*(points[0]) == *(otherEdge.getPoint1()) && (*(points[1]) == *(otherEdge.getPoint2())))
            ||(*(points[0]) == *(otherEdge.getPoint2()) && (*(points[1]) == *(otherEdge.getPoint1())))  );
}

vector<Point*> Edge::getPoints(void) const
{
    return points;
}

void Edge::setPoints(vector<Point*> newPoints)
{
    points = newPoints;
}

Point* Edge::getPoint1(void) const
{
    return points[0];
}

Point* Edge::getPoint2(void) const
{
    return points[1];
}

bool Edge::hasPoint(Point* testedPoint) const
{
    if(*(points[0])==*testedPoint || *(points[1])==*testedPoint)
    {
        return true;
    }
    return false;
}

bool Edge::replace(const Point* oldPoint, Point* newPoint)
{
    for (unsigned short i=0; i<2; i++)
    {
        if (points[i] == oldPoint)
        {
            points[i] = newPoint;
            return true;
        }
    }
    return false;
}

// renvoie vrai si deux arêtes ont un sommet en commun
bool Edge::commonPoint(Edge* otherEdge, Point* commonPoint)
{
    for (unsigned short i=0; i<2; i++)
    {
        for (unsigned short j=0; j<2; j++)
        {
            if (getPoints()[i] == otherEdge->getPoints()[j])
            {
                commonPoint = getPoints()[i];
                return true;
            }
        }
    }
}
