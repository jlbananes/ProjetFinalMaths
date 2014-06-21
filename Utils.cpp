#include <iostream>
#include "Utils.h"
#include "Mesh.h"

using namespace std;

namespace Utils
{
    // calcul du centre du cercle circonscrit
    void CenterCircumsribedSphere(const Face* f, Point &O)
    {
        Point* PointA = f->getEdge1()->getPoint1();
        Point* PointB = f->getEdge2()->getPoint1();
        Point* PointC = f->getEdge3()->getPoint1();

        if (*PointA==*PointB || *PointA==*PointC || *PointB==*PointC)
        {
            cout << "\n\n        ERREUR : POINTS IDENTIQUES !\n\n" << endl;
        }

        float xA = PointA->getX();
        float yA = PointA->getY();
        float zA = PointA->getZ();

        float xB = PointB->getX();
        float yB = PointB->getY();
        float zB = PointB->getZ();

        float xC = PointC->getX();
        float yC = PointC->getY();
        float zC = PointC->getZ();


        ////// résolutions matricielles consécutives de deux systèmes d'équations ///////
        float A = 2*(xA-xB);
        float B = 2*(yA-yB);
        float C = 2*(zA-zB);
        float D = (float)( pow(xA,2) + pow(yA,2) + pow(zA,2) - pow(xB,2) - pow(yB,2) - pow(zB,2) );
        float E = 2*(xA-xC);
        float F = 2*(yA-yC);
        float G = 2*(zA-zC);
        float H = (float)( pow(xA,2) + pow(yA,2) + pow(zA,2) - pow(xC,2) - pow(yC,2) - pow(zC,2) );
        float denominatorIJK = xA*yB*zC + xB*yC*zA + xC*yA*zB - xC*yB*zA - xB*yA*zC - xA*yC*zB;
        float I = (-yB*zC + yC*zB - yC*zA + yA*zC - yA*zB + yB*zA) / denominatorIJK;
        float J = (-xC*zB + xB*zC - xA*zC + xC*zA - xB*zA + xA*zB) / denominatorIJK;
        float K = (-yB*zC + yC*zB - xC*yA + xA*yC - xA*yB + xB*yA) / denominatorIJK;
        float L = -1.0;

        float denominatorXYZ = A*F*K + E*J*C + I*B*G - I*F*C - E*B*K - A*J*G;
        float centerX = (D*(F*K-J*G) + H*(J*C-B*K) + L*(B*G-F*C)) / denominatorXYZ;
        float centerY = (D*(I*G-E*K) + H*(A*K-I*C) + L*(E*C-A*G)) / denominatorXYZ;
        float centerZ = (D*(E*J-I*F) + H*(I*B-A*J) + L*(A*F-E*B)) / denominatorXYZ;

        O.set(centerX,centerY,centerZ);
    }

    // prend en entrée deux faces quelconques et renvoie vrai ainsi que leur arête commune si il en existe une.
    bool commonEdge(const Face* face1, const Face* face2, Edge* commonEdge)
    {
        for(unsigned short i=0; i<3; i++)
        {
            for(unsigned short j=0; j<3; j++)
            {
                if (face1->getEdges()[i] == face2->getEdges()[j])
                {
                    commonEdge = face1->getEdges()[i];
                    return true;
                }
            }
        }
        return false;
    }

    // prend en entrée deux faces voisines et renvoie vrai si elles respectent le critère de Delaunay
    bool DelaunayCriterium(const Face* face1, const Face* face2, const Edge* commonEdge)
    {
        Point center;
        CenterCircumsribedSphere(face1,center);
        // rayon de la sphère circonscrite à la première face
        float radius = (*(face1->getEdge1()->getPoint1()) - center).getNorm();

        Point* otherPoint;
        for(unsigned short i=0; i<3; i++)
        {
            for(unsigned short j=0; j<2; j++)
            {
                if ( !commonEdge->hasPoint(face2->getEdges()[i]->getPoints()[j]) )
                {
                    otherPoint = face2->getEdges()[i]->getPoints()[j];
                }
            }
        }

        float distanceOtherPoint = (*otherPoint - center).getNorm();

        if (distanceOtherPoint > radius)
        {
            return true;
        }
        return false;
    }

    // flipping de deux faces
    void FlipFaces(Face* face1, Face* face2, Edge* commonEdge)
    {
        // récupération des deux vertices NON communs au deux faces
        vector<Point*> nonCommonPoints;
        for(unsigned short i=0; i<3; i++)
        {
            for(unsigned short j=0; j<2; j++)
            {
                if (!commonEdge->hasPoint(face1->getEdges()[i]->getPoints()[j]))
                {
                    nonCommonPoints.push_back(face1->getEdges()[i]->getPoints()[j]);
                }
            }
        }
        for(unsigned short i=0; i<3; i++)
        {
            for(unsigned short j=0; j<2; j++)
            {
                if (!commonEdge->hasPoint(face2->getEdges()[i]->getPoints()[j]))
                {
                    nonCommonPoints.push_back(face2->getEdges()[i]->getPoints()[j]);
                }
            }
        }
        cout << "recuperation de " << nonCommonPoints.size() << " points non communs" << endl;

        // flipping des points de l'arête commune
        if (nonCommonPoints.size()==2)
        {
            commonEdge->replace(commonEdge->getPoint1(),nonCommonPoints[0]);
            commonEdge->replace(commonEdge->getPoint2(),nonCommonPoints[1]);
        }
        // flipping des arêtes des deux faces
        face1->swapEdges(face2,commonEdge);
    }

    // triangulation de Delaunay (prend en paramètre un mesh présentant une triangulation quelconque)
    void DelaunayAlgorithm(Mesh* mesh)
    {
        for (unsigned short i=0; i<mesh->getSurfaces().size(); i++)
        {
            for (unsigned short j=0; j<mesh->getSurfaces()[i]->getFaces().size() ; j++)
            {
                for (unsigned short k=0; k<mesh->getSurfaces()[i]->getFaces().size() ; k++)
                {
                    if (j!=k)
                    {
                        Edge* newCommonEdge;
                        if(commonEdge(mesh->getSurfaces()[i]->getFaces()[j],mesh->getSurfaces()[i]->getFaces()[k],newCommonEdge))
                        {
                            FlipFaces(mesh->getSurfaces()[i]->getFaces()[j],mesh->getSurfaces()[i]->getFaces()[k],newCommonEdge);
                        }
                    }
                }
            }
        }
    }

}
