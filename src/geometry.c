#include "geometry.h"
#include <stdlib.h>
#include <math.h>

Vector3D Vector(Point3D A, Point3D B){
	Vector3D v;
	v.x = B.x - A.x;
	v.y = B.y - A.y;
	v.z = B.z - A.z;
	return v;
}

Point3D PointPlusVector(Point3D P, Vector3D V){
	Point3D newPoint;
	newPoint.x = P.x + V.x;
	newPoint.y = P.y + V.y;
	newPoint.z = P.z + V.z;	
	return newPoint;
}
