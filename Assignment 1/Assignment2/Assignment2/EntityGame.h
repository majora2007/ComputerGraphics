#ifndef __ENTITYGAME_H_
#define __ENTITYGAME_H_
#include "Entity.h"
#include <vector>

class EntityGame : public Entity {
public:
	EntityGame();
	void draw();
	void update();

	void setWidth(int w) { width = w; }
	void setHeight(int h) {height = h; }

	
	
protected:
	int width;
	int height;
	std::vector<Entity*> currentScene;
	bool gameRunning;

	void init();
	
};

#endif