#include "Planet.h"
#include "glut.h"
#include "drawing_funcs.h"
#include <stdio.h>

#define PLANET 0x100

float angleM, angleM2, angleM3;
extern bool boundingBox;
void timerFunc(int value);

extern GLenum polygonFill;

Planet::Planet() : Entity()
{
	staticOrbit = true;
	type = ENTITY_PLANET;

	this->boundBox = new BBox();


	// Calculate 3 BBoxes. 1 for main object, and 2 for sub-moons.
	Moon *moon1 = new Moon();
	moon1->setPosition(position);
	moons[0] = moon1;
	
	moon1 = new Moon();
	moon1->setPosition(position);
	moon1->setVisible(false);
	moons[1] = moon1;

	// Calculate Bounding Box (AABB) (only reason I do it this way is because I don't use vertex arrays)
	float angle;
    for (int i = 0; i < 24; i++)
    {
        angle = PI * 2 * i / 24;
		float px = 1.0f * sin(angle); 
		float py = 1.0f * cos(angle);

		if (px < minX) minX = px;
		if (py < minY) minY = py;
		if (px > maxX) maxX = px;
		if (py > maxY) maxY = py;
    }

	printf("Planet: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);

	this->boundBox->setMinPoint(minX, minY);
	this->boundBox->setMaxPoint(maxX, maxY);


	//glutTimerFunc(100, timerFunc, PLANET);
}

void Planet::draw() 
{
	if (!visible) return;

	if (staticOrbit)
	{
		if (boundingBox)
		{
			moons[0]->setDrawBBox(true);
		} else {
			moons[0]->setDrawBBox(false);
		}

		// Draw main circle at position
		glPushMatrix();
		glLoadIdentity();

		glTranslatef(position[0], position[1], position[2]);

		glColor3f(0.8f, 1.0f, 1.0f);
		draw_circle2d(1.0f, 12, polygonFill);

		// Draw first moon slightly above
		glRotatef(angleM, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 1.2f, 0.0f);

		float *m1Pos = moons[0]->getPosition();

		glGetFloatv( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
		const float position_x = modelview[12];
		const float position_y = modelview[13];
		const float position_z = modelview[14];
		moons[0]->setPosition(position_x, position_y, 0.0f); // This is done to emulate the translate (translate still occurs, moon's position does not affect drawing) and update bounding box

		glRotatef(-angleM, 0.0f, 0.0f, 1.0f);

		glColor3f(1.0f, 0.4f, 1.0f);

		moons[0]->draw(); //draw_triangle2d(0.4f, GL_POLYGON);

		glPopMatrix();
	} else {

		if (boundingBox)
		{
			moons[0]->setDrawBBox(true);
			moons[1]->setDrawBBox(true);
		} else {
			moons[0]->setDrawBBox(false);
			moons[1]->setDrawBBox(true);
		}


		// Draw main circle at position
		glPushMatrix();
		glLoadIdentity();

		glTranslatef(position[0], position[1], position[2]);

		glColor3f(0.8f, 1.0f, 1.0f);
		draw_circle2d(1.0f, 12, polygonFill); // MAIN PLANET

	
		// Draw first moon slightly above
		glRotatef(angleM, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 1.2f, 0.0f);
		glRotatef(-angleM, 0.0f, 0.0f, 1.0f);

		glColor3f(0.8f, 0.4f, 1.0f);
		glGetFloatv( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
		float position_x = modelview[12];
		float position_y = modelview[13];
		float position_z = modelview[14];
		moons[0]->setPosition(position_x, position_y, 0.0f); // This is done to emulate the translate (translate still occurs, moon's position does not affect drawing) and update bounding box
		moons[0]->draw();
		
		glPushMatrix();
		// Draw 2nd moon slightly above 1st, rotate oposite direction 
		glRotatef(angleM3, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 1.2f, 0.0f);
		glRotatef(-angleM3, 0.0f, 0.0f, 1.0f);
		

		glColor3f(0.1f, 0.4f, 1.0f);
		glGetFloatv( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
		position_x = modelview[12];
		position_y = modelview[13];
		position_z = modelview[14];
		moons[1]->setPosition(position_x, position_y, 0.0f); // This is done to emulate the translate (translate still occurs, moon's position does not affect drawing) and update bounding box
		moons[1]->draw();

		glPopMatrix();
		glPopMatrix();
	}
}

void Planet::update() 
{
	// Recalculate BBox
	this->boundBox->minX = this->position[0] + minX;
	this->boundBox->maxX = this->position[0] + maxX;
	this->boundBox->minY = this->position[1] + minY;
	this->boundBox->maxY = this->position[1] + maxY;

	// Update Moon's (This allows their bounding boxes to readjust to previous updates)
	moons[0]->update();
	moons[1]->update();
}


void timerFunc(int value)
{
	
	if (value == PLANET)
	{
		// Increase angles
		angleM += 4.0f;
		angleM2 += 2.0f;
		angleM3 -= 6.0f;

		if (angleM >= 360.0f)
			angleM = 0.0f;
		if (angleM2 >= 360.0f)
			angleM2 = 0.0f;
		if (angleM3 <= 0.0f)
			angleM3 = 360.0f;
	}

	glutTimerFunc(100, timerFunc, PLANET);
	glutPostRedisplay();
}

void Planet::move(const float amt)
{
	// Increase angles
	angleM += 4.0f*amt;
	angleM2 += 2.0f*amt;
	angleM3 -= 6.0f*amt;

	if (angleM >= 360.0f)
		angleM = 0.0f;
	if (angleM2 >= 360.0f)
		angleM2 = 0.0f;
	if (angleM3 <= 0.0f)
		angleM3 = 360.0f;
}

void Planet::setStaticOrbit(bool orbit)
{
	staticOrbit = orbit;
	moons[1]->setVisible(!staticOrbit);
}

Moon** Planet::getMoons()
{
	return moons;
}
