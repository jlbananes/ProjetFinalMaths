#ifndef POINT_H
#define POINT_H

#include <cmath>

using namespace std;

class Point
{
    private:
        float x,y,z;

    public:
        Point();
        Point(float,float,float);
        Point operator=(const Point pt) ;
        Point operator+(const Point pt) const;
        Point operator-(const Point pt) const;
        Point operator*(float m) const;
        Point operator/(float m) const;
        bool operator==(const Point pt) const;
        float getX(void) const;
        float getY(void) const;
        float getZ(void) const;
        void set(float,float,float);
        float getNorm(void) const;
};

#endif // point_H
