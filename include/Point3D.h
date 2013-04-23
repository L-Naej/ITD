#ifndef ITD_POINT3D_H
#define ITD_POINT3D_H
#include "utils.h"

typedef struct{
	float x;
	float y;
	float z;
}Point3D;

Point3D PointXYZ(float x, float y, float z);
bool arePointsEquals(Point3D A, Point3D B);

void dumpPoint(Point3D point);
#endif
