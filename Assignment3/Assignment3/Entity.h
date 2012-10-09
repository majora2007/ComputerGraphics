#ifndef ENTITY_H
#define ENTITY_H

#include <imathvec.h>				//- Vector library
#include "mesh.h"
#include "BBox.h"
#include "EntityTypes.h"
using namespace std;


class Entity
{
public:
    Entity() 
	{ 
		position[0] = 0.0f; position[1] = 1.0f; position[2] = 0.0f;
		face[0] = 0.0f; face[1] = 1.0f; face[2] = 1.0f;

		drawBBox = false;
		visible = true;
		minX = 1000.0f;
		minY = 1000.0f;
		maxX = -1000.0f;
		maxY = -1000.0f;
		_id = 0;
		type = ENTITY;

		_mesh = 0;
	}

    // Appearance
    virtual void update() =0;
    virtual void draw() =0;
	virtual void setBoundingBox(BBox &bb) { *boundBox = bb; }
	virtual BBox getBoundingBox() const { return *boundBox; }
	virtual entity_type_t getType() { return type; }
	virtual void drawBoundingBox();
	virtual void setVisible(const bool b) { visible = b; }
	inline bool isVisible() const { return visible; }

    // Hit Detection
	void load_mesh(Mesh &m);
	bool intersects(const BBox &bb);
	

    virtual void setPosition(float pos[]) { position[0] = pos[0]; position[1] = pos[1]; position[2] = pos[2]; this->boundBox->minX = this->position[0] + this->minX;
				this->boundBox->maxX = this->position[0] + this->maxX;
				this->boundBox->minY = this->position[1] + this->minY;
				this->boundBox->maxY = this->position[1] + this->maxY; }
	virtual void setPosition(float x, float y, float z) {position[0] = x; position[1] = y; position[2] = z; 	this->boundBox->minX = this->position[0] + this->minX;
				this->boundBox->maxX = this->position[0] + this->maxX;
				this->boundBox->minY = this->position[1] + this->minY;
				this->boundBox->maxY = this->position[1] + this->maxY; }
    virtual float* getPosition() { return position; }

	void setDrawBBox(bool b) { drawBBox = b; }
	virtual int getId() { return _id; }
	virtual void setId(int ID) { _id = ID; }
	virtual void move(const float amt) =0;

protected:
    float position[3];
    float face[3]; // The direction the Entity is looking at
    
	// Identifiers (replaced with model's id)
	int _id;
	entity_type_t type;

	// Old Bounding Box
	BBox *boundBox;
	float minX, maxX, minY, maxY;
	
	// Bounding box and Mesh
	Mesh* _mesh;
	//Vec3f bbxMin, bbxMax, bbxCenter;	//- Bounding box vectors of model

	// Random stuff
	bool drawBBox;
	bool visible;
};


#endif // ENTITY_H
