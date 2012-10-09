#include "Wall.h"
#include "glut.h"
#include "drawing_funcs.h"
#include <stdio.h>

void  Wall_timerFunc(int value);
static float movementAmt, prevMovementAmt;
float wallVerticies[4][2] = { {-1.0f, 0.4f}, {-1.0f, -0.4f}, {1.0f, -0.4f}, {1.0f, 0.4f} };
static bool moveRight = true;

extern GLenum polygonFill;

extern int mainWindow;
#define WALL 0x101

Wall::Wall() : Entity()
{
	type = ENTITY_WALL;
	stationary = true;
	movementAmt = 0.0f;
	prevMovementAmt = movementAmt;

	this->boundBox = new BBox();

    // Calculate Bounding Box (AABB) (only reason I do it this way is because I don't use vertex arrays)
    for (int i = 0; i < 4; i++)
    {
		float px = wallVerticies[i][0];
		float py = wallVerticies[i][1];

		if (px < minX) this->minX = px;
		if (py < minY) this->minY = py;
		if (px > maxX) this->maxX = px;
		if (py > maxY) this->maxY = py;
    }

	printf("Wall: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);

	this->boundBox->setMinPoint(minX, minY);
	this->boundBox->setMaxPoint(maxX, maxY);

	/*if (!stationary)
		glutTimerFunc(1000, Wall_timerFunc, WALL);*/
}

void Wall::update() 
{
	// Had to modify position like this because glutTimerFunc is not nice.
	if (prevMovementAmt == movementAmt)
	{
		return;
	} else {
		if (moveRight)
		{
			this->position[0] += 0.2f;
		}
		else
		{
			this->position[0] -= 0.2f;
		}
		
		this->boundBox->minX = this->position[0] + this->minX;
		this->boundBox->maxX = this->position[0] + this->maxX;
		this->boundBox->minY = this->position[1] + this->minY;
		this->boundBox->maxY = this->position[1] + this->maxY;
	}
}

void Wall::draw() 
{
	
	update();

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(this->position[0], this->position[1], this->position[2]);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(polygonFill);
		glVertex2fv(wallVerticies[0]);
		glVertex2fv(wallVerticies[1]);
		glVertex2fv(wallVerticies[2]);
		glVertex2fv(wallVerticies[3]);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (drawBBox)
	{
		drawBoundingBox();
		/*glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(boundBox->minX, boundBox->minY);
			glVertex2f(boundBox->maxX, boundBox->minY);
			glVertex2f(boundBox->maxX, boundBox->maxY);
			glVertex2f(boundBox->minX, boundBox->maxY);
		glEnd();*/
	}
	glPopMatrix();
}


void Wall_timerFunc(int value)
{
	if (value == WALL)
	{
		prevMovementAmt = movementAmt;

		if (moveRight)
			movementAmt += 0.2f;
		else
			movementAmt -= 0.2f;

		if (movementAmt >= 2.0f )
		{
			moveRight = false;
		}
		if (movementAmt <= 0.0f)
			moveRight = true;

		
		glutTimerFunc(100, Wall_timerFunc, WALL);
		glutSetWindow(mainWindow);
		glutPostRedisplay();
	}
}

void Wall::move(const float amt)
{
	if (stationary) return;

	prevMovementAmt = movementAmt;

	if (moveRight)
		movementAmt += 0.2f*amt;
	else
		movementAmt -= 0.2f*amt;

	if (movementAmt >= 4.0f )
	{
		moveRight = false;
	}
	if (movementAmt <= 0.0f)
		moveRight = true;
}

void Wall::setDefaultDir(int d)
{
	if (d >= 0)
		moveRight = true;
	else 
	{
		moveRight = false;
		movementAmt = 2.0f;
	}
}
void Wall::setStationary(bool b)
{
	stationary = b;

	if (!stationary)
		glutTimerFunc(100, Wall_timerFunc, WALL);
}

