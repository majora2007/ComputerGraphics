#ifndef CANNON_H
#define CANNON_H
#include "Entity.h"
#include "Bullet.h"
#include <vector>

class Cannon : public Entity
{

public:
    Cannon();

    void update();
    void draw();
	void move(const float amt);

    void fire();
	std::vector<Bullet*> getBullets();
	void setBullets(std::vector<Bullet*> b);

    void rotateZ(const float angle);
	void setFireSpeed(const float speed);
	void setRotation(const float alpha);

private:
	float angle; // angle of rotation for gun
	float health;
	bool isAlive;
	std::vector<Bullet*> bullets;
	float firespeed;
};

#endif // CANNON_H