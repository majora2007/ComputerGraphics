#ifndef __BULLET_H_
#define __BULLET_H_
#include "Entity.h"
#include "Timer.h"

class Bullet : public Entity {

public:
	Bullet();

	void draw();
	void update();
	void move(const float amt) {}
	void setFace(float *f) { dirVec[0] = f[0]; dirVec[1] = f[1]; }
	void setFireSpeed(const float s) { this->fireSpeed = s; }
	void setVisible(const bool b) { this->visible = b; }
	inline bool isVisible() const { return this->visible; }

protected:
	float dirVec[2];
	float fireSpeed;
	bool visible;
};

#endif