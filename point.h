#ifndef POINT_H
#define POINT_H

#include <cmath>
#include <vector>

using namespace std;
class Edge;

class Point
{
    private:
        float x,y,z;
        vector<Edge*> edges;

    public:
        Point();
        Point(const Point &point);
        Point(float,float,float);
        Point operator=(const Point pt) ;
        Point operator+(const Point pt) const;
        Point operator-(const Point pt) const;
        Point operator*(float m) const;
        Point operator/(float m) const;
        bool operator==(const Point pt) const;

        void set(float,float,float);
        float getX(void) const;
        float getY(void) const;
        float getZ(void) const;
        float getNorm(void) const;
        float getHeight(void) const;
        vector<Edge*> getEdges(void) const;
        vector<Point*> getNeighbours(void) const;
};

#endif // point_H
