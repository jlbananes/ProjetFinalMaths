#ifndef EDGE_H
#define EDGE_H

#include "Point.h"

using namespace std;

class Edge
{
    private:
        vector<Point*> points;
    public:
        Edge();
        Edge(const Edge &edge);
        Edge(Point*, Point*);
        void operator=(const Edge) ;
        bool operator==(const Edge) const;
        vector<Point *> getPoints(void) const;
        Point* getPoint1(void) const;
        Point* getPoint2(void) const;
        void setPoints(vector<Point*>);
        bool hasPoint(Point*) const;
        bool commonPoint(Edge*, Point*);
        bool replace(const Point*, Point *);
};

#endif // EDGE_H
