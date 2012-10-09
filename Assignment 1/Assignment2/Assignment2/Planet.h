#ifndef __PLANET_H_
#define __PLANET_H_

#include "Entity.h"
#include "Moon.h"
#include <vector>

class Planet : public Entity
{
public:
	Planet();
	
	void update();
    void draw();

	void setStaticOrbit(bool orbit);
	Moon** getMoons();
	void move(const float amt);

private:
	bool staticOrbit;
	Moon* moons[2];
	GLfloat modelview[16]; //var to hold the modelview info
	
};

#endif