#ifndef ITD_VECTOR3D_H
#define ITD_VECTOR3D_H

typedef struct{
	float x;
	float y;
	float z;
}Vector3D;

//Utiles pour les calculs d'angle avec les axes principaux
static const Vector3D ITD_X_AXIS = {.x=1.0,.y=0.0,.z=0.0};
static const Vector3D ITD_Y_AXIS = {.x=0.0,.y=1.0,.z=0.0};

Vector3D VectorXYZ(float x, float y, float z);
Vector3D AddVectors(Vector3D V1, Vector3D V2);
Vector3D SubVectors(Vector3D V1, Vector3D V2);
Vector3D MultVector(Vector3D V, float scalar);
Vector3D DivVector(Vector3D V, float scalar);
float DotProduct(Vector3D V1, Vector3D V2);
float Norm(Vector3D V);
Vector3D Normalize(Vector3D V);
void dumpVector(Vector3D vect);
#endif
