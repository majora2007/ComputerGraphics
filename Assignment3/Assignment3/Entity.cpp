#include "Entity.h"
#include "glut.h"
#include <math.h>





void Entity::load_mesh(Mesh &m)
{
	*_mesh = m;
}

void Entity::drawBoundingBox()
{
	if (!visible) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(boundBox->minX, boundBox->minY);
		glVertex2f(boundBox->maxX, boundBox->minY);
		glVertex2f(boundBox->maxX, boundBox->maxY);
		glVertex2f(boundBox->minX, boundBox->maxY);
	glEnd();
	glPopMatrix();
}

bool Entity::intersects(const BBox &bb) 
	{ 
		// Handle that maxX,maxY is in area of boundBox (NOTE: This does not work if my box is inside another (no corners in). 
		// Read up on how to do here: http://programyourfaceoff.blogspot.com/2011/12/intersection-testing.html
		/*if (((bb.maxX > boundBox->minX && bb.maxX < boundBox->maxX) && (bb.maxY > boundBox->minY && bb.maxY < boundBox->maxY)) ||
			((bb.minX > boundBox->minX && bb.minX < boundBox->maxX) && (bb.minY > boundBox->minY && bb.minY < boundBox->maxY)) ||
			((bb.minY > boundBox->minY && bb.minY < boundBox->maxY) && (bb.minX > boundBox->minX && bb.minX < boundBox->maxX)) ||
			((bb.maxY < boundBox->maxY && bb.maxY > boundBox->maxY) && (bb.maxX < boundBox->maxX && bb.maxX > boundBox->maxX)) )
			return true;
		else
			return false;*/

		// Case 1 = b interesecting on lower line
		// Case 2 = b intersecting on upper line
		bool inX = (bb.minX >= boundBox->minX && bb.maxX <= boundBox->maxX);
		bool inY = (bb.minY >= boundBox->minY && bb.maxY <= boundBox->maxY);

		bool case1 = ((bb.maxY > boundBox->minY && bb.maxY <= boundBox->maxY) || (bb.maxY < boundBox->minY && bb.maxY >= boundBox->maxY)) && inX;
		bool case2 = ((bb.minY < boundBox->maxY && bb.minY >= boundBox->minY) || (bb.minY > boundBox->maxY && bb.minY <= boundBox->minY)) && inX;
		bool case3 = ((bb.minX < boundBox->maxX && bb.minX >= boundBox->minX) || (bb.minX > boundBox->maxX && bb.minX <= boundBox->minX)) && inY;
		bool case4 = ((bb.maxX > boundBox->minX && bb.minX <= boundBox->maxX) || (bb.maxX > boundBox->minX && bb.minX >= boundBox->maxX)) && inY;


		if (case1 || case2 || case3 || case4)
			return true;
		else return false;
	}