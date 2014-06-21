#include "Point.h"

Point::Point(void)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
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
    return Point(this->x + pt.x, this->y + pt.y, this->z + pt.z);
}

Point Point::operator-(const Point pt) const
{
    return Point(this->x - pt.x, this->y - pt.y, this->z - pt.z);
}


Point Point::operator*(float m) const
{
    return Point(this->x * m, this->y * m, this->z * m);
}

Point Point::operator/(float m) const
{
    if (m!=0)
    {
        return Point(this->x/m, this->y/m, this->z/m);
    }
    else
    {
        return Point(0,0,0);
    }
}

float Point::getX(void) const
{
    return this->x;
}

float Point::getY(void) const
{
    return this->y;
}

float Point::getZ(void) const
{
    return this->z;
}

void Point::set(float newX, float newY, float newZ)
{
    this->x = newX;
    this->y = newY;
    this->z = newZ;
}

float Point::getNorm(void) const
{
    return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}
