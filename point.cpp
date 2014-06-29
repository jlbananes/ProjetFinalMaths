#include "Point.h"
#include "Edge.h"

Point::Point(void)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

Point::Point(const Point &point)
{
    this->x = point.x;
    this->y = point.y;
    this->z = point.z;
}

Point::Point(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}

Point Point::operator=(const Point pt)
{
    x = pt.x;
    y = pt.y;
    z = pt.z;
    return *this;
}

bool Point::operator==(const Point pt) const
{
    return (x==pt.x && y==pt.y && z==pt.z);
}

Point Point::operator+(const Point pt) const
{
    return Point(x + pt.x, y + pt.y, z + pt.z);
}

Point Point::operator-(const Point pt) const
{
    return Point(x - pt.x, y - pt.y, z - pt.z);
}


Point Point::operator*(float m) const
{
    return Point(x * m, y * m, z * m);
}

Point Point::operator/(float m) const
{
    if (m!=0)
    {
        return Point(x/m, y/m, z/m);
    }
    else
    {
        return Point(0,0,0);
    }
}

float Point::getX(void) const
{
    return x;
}

float Point::getY(void) const
{
    return y;
}

float Point::getZ(void) const
{
    return z;
}

void Point::set(float newX, float newY, float newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}

float Point::getNorm(void) const
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

vector<Edge*> Point::getEdges(void) const
{
    return edges;
}

vector<Point*> Point::getNeighbours(void) const
{
    vector<Point*> neighbours;
    for(unsigned int i=0; i<edges.size(); i++)
    {
        if (edges[i]->getPoint1() != this)
            neighbours.push_back(edges[i]->getPoint1());
        else
            neighbours.push_back(edges[i]->getPoint2());
    }
    return neighbours;
}

float Point::getHeight(void) const
{
    vector<Point*> neighbours = getNeighbours();
    Point pointsAverage;
    for (unsigned int i=0; i<neighbours.size(); i++)
    {
        pointsAverage = pointsAverage + *neighbours[i];
    }
    pointsAverage = pointsAverage / neighbours.size();

    return (*this-pointsAverage).getNorm();
}
