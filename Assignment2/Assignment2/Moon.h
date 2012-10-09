#ifndef __MOON_H_
#define __MOON_H_

#include "Entity.h"
#include "glut.h"

class Moon : public Entity
{
public:
	Moon();
	
	void update();
    void draw();
	void setDrawMode(const GLenum mode) { drawMode = mode; }
	void move(const float amt) {}
	//void setDrawFunc(void* func);

protected:
	GLenum drawMode;
	//int (*pt2Function) () = NULL;
	
};

#endif