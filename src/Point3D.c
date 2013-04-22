#include "Point3D.h"
#include <stdio.h>

Point3D PointXYZ(float x, float y, float z){
	Point3D p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

bool arePointsEquals(Point3D A, Point3D B){
	return (A.x == B.x) && (A.y == B.y) && (A.z == B.z);
}

void dumpPoint(Point3D point){
	printf("\nDump point :\n");
	printf("\tx : %f\n", point.x);
	printf("\ty : %f\n", point.y);
	printf("\tz : %f\n", point.z);
}
