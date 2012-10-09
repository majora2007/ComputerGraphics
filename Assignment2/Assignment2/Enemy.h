#ifndef __ENEMY_H_
#define __ENENMY_H_
#include "Entity.h"


class Enemy : public Entity
{
public:
	Enemy();

	void update();
    void draw();

	int getPointWorth() { return points; }
	void setPointWorth(const int p) { points = p; }
	void move(const float amt);

private:
	int points;
	//float verticies[4][2] = { {-1.0f, 0.2f}, {-1.0f, -0.2f}, {1.0f, -0.2f}, {1.0f, 0.2f} };
	int height, width;
};

#endif