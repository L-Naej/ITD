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

//Ne fonctionne qu'en 2D
bool segment2segment(Point3D A,Point3D B,Point3D C,Point3D D){
		float Ax = A.x;
		float Ay = A.y;
		float Bx = B.x;
		float By = B.y;
		float Cx = C.x;
		float Cy = C.y;
		float Dx = D.x;
		float Dy = D.y;
 
		double Sx;
		double Sy;
 
		if(Ax==Bx)
		{
			if(Cx==Dx) return false;
			else
			{
				double pCD = (Cy-Dy)/(Cx-Dx);
				Sx = Ax;
				Sy = pCD*(Ax-Cx)+Cy;
			}
		}
		else
		{
			if(Cx==Dx)
			{
				double pAB = (Ay-By)/(Ax-Bx);
				Sx = Cx;
				Sy = pAB*(Cx-Ax)+Ay;
			}
			else
			{
				double pCD = (Cy-Dy)/(Cx-Dx);
				double pAB = (Ay-By)/(Ax-Bx);
				double oCD = Cy-pCD*Cx;
				double oAB = Ay-pAB*Ax;
				Sx = (oAB-oCD)/(pCD-pAB);
				Sy = pCD*Sx+oCD;
			}
		}
		if((Sx<Ax && Sx<Bx)|(Sx>Ax && Sx>Bx) | (Sx<Cx && Sx<Dx)|(Sx>Cx && Sx>Dx)
				| (Sy<Ay && Sy<By)|(Sy>Ay && Sy>By) | (Sy<Cy && Sy<Dy)|(Sy>Cy && Sy>Dy)) return false;
		  return true; //or :     return new Point2D.Float((float)Sx,(float)Sy)
}
