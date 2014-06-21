#include <iostream>
#include "Retopology.h"

using namespace std;
using namespace Utils;

namespace Retopology
{
    void TestFunction(void)
    {
        Point A(-10,0,5);
        Point B(7,3.5,-10);
        Point C(7,-3.5,-5);

        Edge AB(&A,&B);
        Edge BC(&B,&C);
        Edge CA(&C,&A);

        Face ABC(&AB,&BC,&CA);
        Point CCC;

        CenterCircumsribedSphere(&ABC,CCC);
        cout << "\n\nCoordonnees du centre du cercle circonscrit = (" << CCC.getX()
                << ";" << CCC.getY() << ";" << CCC.getZ() << ")" << endl;
    }

}
