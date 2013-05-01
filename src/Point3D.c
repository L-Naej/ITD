#include "point3D.h"
#include <stdio.h>
#include <math.h>

Point3D PointXYZ(float x, float y, float z){
	Point3D p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

bool arePointsEquals(Point3D A, Point3D B){
	return (fabs(A.x-B.x) < 0.001) && (fabs(A.y-B.y) < 0.001) && (fabs(A.z-B.z) < 0.001);
}

void dumpPoint(Point3D point){
	printf("\nDump point :\n");
	printf("\tx : %f\n", point.x);
	printf("\ty : %f\n", point.y);
	printf("\tz : %f\n", point.z);
}
