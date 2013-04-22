#include "Vector3D.h"
#include <math.h>
#include <stdio.h>

Vector3D VectorXYZ(float x, float y, float z){
	Vector3D newVector;
	newVector.x = x;
	newVector.y = y;
	newVector.z = z;
	return newVector;
}

Vector3D AddVectors(Vector3D Vector1, Vector3D Vector2){
	Vector3D newVector;
	newVector.x = Vector1.x + Vector2.x;
	newVector.y = Vector1.y + Vector2.y;
	newVector.z = Vector1.z + Vector2.z;
	return newVector;
}

Vector3D SubVectors(Vector3D Vector1, Vector3D Vector2){
	Vector3D newVector;
	newVector.x = Vector1.x - Vector2.x;
	newVector.y = Vector1.y - Vector2.y;
	newVector.z = Vector1.z - Vector2.z;
	return newVector;
}

Vector3D MultVector(Vector3D V, float scalar){
	Vector3D newVector;
	newVector.x = V.x*scalar;
	newVector.y = V.y*scalar;
	newVector.z = V.z*scalar;
	return newVector;
}

Vector3D DivVector(Vector3D V, float scalar){
	Vector3D newVector;
	newVector.x = V.x/scalar;
	newVector.y = V.y/scalar;
	newVector.z = V.z/scalar;
	return newVector;
}

float DotProduct(Vector3D Vector1, Vector3D Vector2){
	return Vector1.x*Vector2.x + Vector1.y*Vector2.y + Vector1.z*Vector2.z;
}

float Norm(Vector3D V){
	return sqrt(V.x*V.x + V.y*V.y + V.z*V.z);
}

Vector3D Normalize(Vector3D V){
	Vector3D newVector;
	float norm = Norm(V);
	newVector.x = V.x / norm;
	newVector.y = V.y / norm;
	newVector.z = V.z / norm;
	return newVector;
}

void dumpVector(Vector3D vect){
	printf("\nDump vector :\n");
	printf("\tx : %f\n", vect.x);
	printf("\ty : %f\n", vect.y);
	printf("\tz : %f\n", vect.z);
}
