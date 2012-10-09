#include "Bullet.h"
#include "glut.h"
#include "drawing_funcs.h"
#include <iostream>

float bulletVerticies[4][2] = { {-0.15f, 0.15f}, {-0.15f, -0.15f}, {0.15f, -0.15f}, {0.15f, 0.15f} };
extern int mainWindow;
extern GLenum polygonFill;

Bullet::Bullet() : Entity() 
{
	dirVec[0] = 1.0f;
	dirVec[1] = 0.0f;
	this->fireSpeed = 0.2f;
	visible = true;

	type = ENTITY_BULLET;

	this->boundBox = new BBox();

	 // Calculate Bounding Box (AABB)
    for (int i = 0; i < 4; i++)
    {
		float px = bulletVerticies[i][0];
		float py = bulletVerticies[i][1];

		if (px < minX) minX = px;
		if (py < minY) minY = py;
		if (px > maxX) maxX = px;
		if (py > maxY) maxY = py;
    }

	printf("Bullet: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);

	boundBox->setMinPoint(minX, minY);
	boundBox->setMaxPoint(maxX, maxY);
}

void Bullet::update()
{
	this->position[0] += (dirVec[0] * this->fireSpeed);
	this->position[1] += (dirVec[1] * this->fireSpeed);

	// Recalculate BBox
	this->boundBox->minX = this->position[0] + this->minX;
	this->boundBox->maxX = this->position[0] + this->maxX;
	this->boundBox->minY = this->position[1] + this->minY;
	this->boundBox->maxY = this->position[1] + this->maxY;
}

void Bullet::draw() 
{
	if (this->visible)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
			glTranslatef(position[0], position[1], position[2]);
			glColor3f(0.0f, 0.0f, 1.0f);
		
			glBegin(polygonFill);
				glVertex2fv(bulletVerticies[0]);
				glVertex2fv(bulletVerticies[1]);
				glVertex2fv(bulletVerticies[2]);
				glVertex2fv(bulletVerticies[3]);
			glEnd();
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		if (drawBBox)
		{
			drawBoundingBox();
			/*glColor3f(0.0f, 1.0f, 0.0f);
			glBegin(GL_LINE_LOOP);
				glVertex2f(this->boundBox->minX, this->boundBox->minY);
				glVertex2f(this->boundBox->maxX, this->boundBox->minY);
				glVertex2f(this->boundBox->maxX, this->boundBox->maxY);
				glVertex2f(this->boundBox->minX, this->boundBox->maxY);
			glEnd();*/
		}
		glPopMatrix();
	}
}