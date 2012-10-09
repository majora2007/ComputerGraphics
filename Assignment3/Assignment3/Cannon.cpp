#include "Cannon.h"
#include "glut.h"
#include <math.h>
#include <stdio.h>
//#include "drawing_funcs.h"
#include <limits>
#include <vector>

Cannon::Cannon() : Entity()
{
	face[0] = 0.0f; face[1] = 1.0f; face[2] = 1.0f;
	angle = 0.0f;
	firespeed = 0.2f;
	this->boundBox = new BBox();

    // Calculate Bounding Box (AABB) (only reason I do it this way is because I don't use vertex arrays)
	float angle;
    for (int i = 0; i < 24; i++)
    {
        angle = 3.14159 * 2 * i / 24;
		float px = 0.4f * sin(angle);
		float py = 0.4f * cos(angle);

		if (px < minX) minX = px;
		if (py < minY) minY = py;
		if (px > maxX) maxX = px;
		if (py > maxY) maxY = py;
    }
	
	printf("Cannon: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);
	
	this->boundBox->setMinPoint(minX, minY);
	this->boundBox->setMaxPoint(maxX, maxY);
}


void Cannon::draw()
{
	update();

	
}

void Cannon::update()
{
	// Recalculate BBox
	this->boundBox->minX = this->position[0] + minX;
	this->boundBox->maxX = this->position[0] + maxX;
	this->boundBox->minY = this->position[1] + minY;
	this->boundBox->maxY = this->position[1] + maxY;
}

void Cannon::rotateZ(const float alpha)
{
	angle += alpha;
    float temp[3];
	temp[0] = face[0];
	temp[1] = face[1];
	temp[2] = face[2];
	
	this->face[0] = temp[0]*cos(alpha * 0.0174532925) - temp[1]*sin(alpha * 0.0174532925); // (2*3.14/360)*alpha
	this->face[1] = temp[0]*sin(alpha * 0.0174532925) + temp[1]*cos(alpha * 0.0174532925);

	this->update();
}

void Cannon::fire()
{
	// First check current bullets available and if any are invisible, reset them, and use them

	/*for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]->isVisible())
		{
			printf("bullet %d reused!\n", bullets[i]->getId());
			bullets[i]->setVisible(true);
			bullets[i]->setPosition(this->position);
			bullets[i]->setFace(this->face);
			bullets[i]->setDrawBBox(true);
			bullets[i]->setFireSpeed(firespeed);
			return;
		}
	}

	Bullet *b = new Bullet();
	b->setPosition(this->position);
	b->setFace(this->face);
	b->setDrawBBox(true);
	b->setId(bullets.size() + 1);
	b->setFireSpeed(firespeed);
	bullets.push_back(b);*/
}

/*std::vector<Bullet*> Cannon::getBullets()
{
	return bullets;
}

void Cannon::setBullets(std::vector<Bullet*> b)
{
	bullets = b;
}

void Cannon::setFireSpeed(const float speed)
{
	firespeed = speed;
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->setFireSpeed(speed);
	}
}*/

void Cannon::setRotation(const float alpha)
{
	angle = alpha;

	if (alpha == 0.0f)
	{
		this->face[0] = 0.0f;
		this->face[1] = 1.0f;
		this->update();
		return;
	}

	float temp[3];
	temp[0] = face[0];
	temp[1] = face[1];
	temp[2] = face[2];
	
	this->face[0] = temp[0]*cos(alpha * 0.0174532925) - temp[1]*sin(alpha * 0.0174532925);
	this->face[1] = temp[0]*sin(alpha * 0.0174532925) + temp[1]*cos(alpha * 0.0174532925);

	this->update();
}

void Cannon::move(const float amt)
{
	/*for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->setFireSpeed(amt);
	}*/
}