#ifndef TOWERS
#define TOWERS

typedef enum {
	ROCKET, LASER, MITRAILLETTE, HYBRID
}TowerType; 

typedef struct {
	TowerType type;
	int cadence;
	int range;
	int power;
	float price;
}Tower;

#endif
