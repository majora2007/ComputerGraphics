#ifndef __GUN_H_
#define __GUN_H_
#include "Entity.h"

class Gun : public Entity
{
public:
	Gun() {}

	void update();
    void draw();
	void fire();

	void setFireRate(float rate);

protected:
	float fireRate;

};


#endif