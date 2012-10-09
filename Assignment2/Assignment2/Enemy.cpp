#include "Enemy.h"
#include "glut.h"
#include "drawing_funcs.h"
#include <stdio.h>
#include <math.h>

void  Enemy_timerFunc(int value);
#define ENEMY 0x1001
static double time = 0.0f;
extern GLenum polygonFill;
extern BBox windowBox;

Enemy::Enemy() : Entity()
{
	type = ENTITY_ENEMY;
	points = 1;
	this->boundBox = new BBox();

	int i;
    float angle;
	for (i = 0; i < 8; i++)
    {
        angle = 3.14159 * 2 * i / 8; // 2PI * i/v
        float px = 0.6f * sin(angle); // 0.4 (size) + 0.2f (for circle)
		float py = 0.6f * cos(angle);

		if (px < minX) minX = px;
		if (py < minY) minY = py;
		if (px > maxX) maxX = px;
		if (py > maxY) maxY = py;
    }

	printf("Enemy: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);

	this->boundBox->setMinPoint(minX, minY);
	this->boundBox->setMaxPoint(maxX, maxY);

	this->update();
	//glutTimerFunc(1000, Enemy_timerFunc, ENEMY);
}

void Enemy::update() 
{
	if (this->position[0] >= windowBox.getRight())
		this->position[0] = position[0] - (windowBox.getRight() - windowBox.getLeft());

	this->boundBox->minX = this->position[0] + this->minX;
	this->boundBox->maxX = this->position[0] + this->maxX;
	this->boundBox->minY = this->position[1] + this->minY;
	this->boundBox->maxY = this->position[1] + this->maxY;
}

void Enemy::draw() 
{
	if (!visible) return;

	
	glPushMatrix();
	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);

	glColor3f(1.0f, 0.0f, 1.0f);
	draw_fireballs(0.4f, 0.1f, 24, polygonFill);

	glPopMatrix();
}

void  Enemy_timerFunc(int value)
{
	if (value == ENEMY)
	{
		time++;
		glutTimerFunc(100, Enemy_timerFunc, ENEMY);
	}
}

void Enemy::move(const float amt)
{
	this->position[0] = position[0] + amt;
	this->position[1] = sin(position[0]);
	
	update();
}