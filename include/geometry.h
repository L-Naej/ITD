#ifndef ITD_GEOMETRY_H
#define ITD_GEOMETRY_H
#include "vector3D.h"
#include "point3D.h"

Vector3D Vector(Point3D A, Point3D B);
Point3D PointPlusVector(Point3D P, Vector3D V);
bool segment2segment(Point3D A,Point3D B,Point3D C,Point3D D);
#endif
