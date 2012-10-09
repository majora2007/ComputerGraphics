#ifndef __WALL_H_
#define __WALL_H_
#include "Entity.h"


class Wall : public Entity
{
public:
	Wall();

	void update();
    void draw();
	void move(const float amt);

	void setDefaultDir(int d);
	void setStationary(bool b);

protected:
	bool stationary;
};


#endif