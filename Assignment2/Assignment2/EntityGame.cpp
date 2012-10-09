#include "EntityGame.h"
#include "Wall.h"
#include "Cannon.h"
#include "Planet.h"
#include "glut.h"


EntityGame::EntityGame() : Entity()
{
	init();
}

void EntityGame::draw()
{

	for (int i = 0; i < currentScene.size(); i++)
	{
		currentScene[i]->draw();
	}
}

void EntityGame::update()
{
}

void EntityGame::init()
{
	// Add Wall's to currentScene
	Wall *w1 = new Wall();
	w1->setDrawBBox(true);
	w1->setPosition(1.0f, 4.0f, 0.0f);
	currentScene.push_back( w1 );
	w1 = new Wall();
	w1->setDrawBBox(true);
	w1->setPosition(0.0f, 4.0f, 0.0f);
	currentScene.push_back( w1 );

	// Add Cannon to currentScene
	Cannon *myChar = new Cannon();
	myChar->setDrawBBox(true);
	float p[3];
	p[0] = 0.0f;
	p[1] = 0.0f;
	p[2] = 0.0f;
	myChar->setPosition(p);
	currentScene.push_back( myChar );
}