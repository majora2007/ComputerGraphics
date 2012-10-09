#include "Moon.h"
#include "drawing_funcs.h"

extern GLenum polygonFill;
float verts[3][2] = {{-0.4f, 0.0f}, {0.4f, 0.0f}, {-0.4f, 0.4f}};
Moon::Moon() : Entity()
{
	this->boundBox = new BBox();

	type = ENTITY_MOON;

	// Calculate Bounding Box (AABB)
	for (int i = 0; i < 3; i++)
    {
		float px = verts[i][0];
		float py = verts[i][1];

		if (px < minX) this->minX = px;
		if (py < minY) this->minY = py;
		if (px > maxX) this->maxX = px;
		if (py > maxY) this->maxY = py;
    }

	printf("Moon: (%f, %f) -> (%f, %f)\n", minX, minY, maxX, maxY);

	this->boundBox->setMinPoint(minX, minY);
	this->boundBox->setMaxPoint(maxX, maxY);
	this->update();
}

void Moon::draw() 
{
	if (!visible) return;

	glPushMatrix();
		draw_triangle2d(0.4f, polygonFill);
	glPopMatrix();

	if (drawBBox)
	{
		drawBoundingBox();
	}
}

void Moon::update() 
{
	// Recalculate BBox
	this->boundBox->minX = this->position[0] + minX;
	this->boundBox->maxX = this->position[0] + maxX;
	this->boundBox->minY = this->position[1] + minY;
	this->boundBox->maxY = this->position[1] + maxY;
}