#include <iostream>
#include "glut.h"
//#include "drawing_funcs.h"
#include "Cannon.h"
#include "Planet.h"
#include "Wall.h"
#include "Bullet.h"
#include <math.h>
#include <vector>
#include "Timer.h"
#include "Enemy.h"

//#define DEGREES_TO_RADIANS 3.14159/180.0


// MUST IMPLEMENT 3 TYPES OF MOTION (spiral, sinusoidal, straightline, etc) [DONE]
// Object RENDERING [DONE]
// DOUBLE ORBIT [DONE]
// STATIC ORBIT [DONE]
// WINDOW RESHAPE (CHECK BEFORE SUBMIT)
// COLLISION DETECT BTW BULLETS AND OBJECTS [DONE]
// Test INTERSECTION METHOD [DONE]
// BOUNDING BOXES [DONE]
// RESTART GAME [DONE]
// GAME-LIKE STUFF [DONE]

typedef float vec3f[3];
#define UPDATE 0x112
#define SCORETIMER 0x113

#define WALL_LEFT 31
#define WALL_RIGHT 32
#define PLANET_S 11
#define PLANET_D 12
#define FILLED GL_POLYGON
#define OUTLINE GL_LINE_LOOP


void updateTime(int value);
void restartGame();

// GLOBALS
// These are used to identify menu options
const int animate_slow = 10, animate_medium = 11, animate_fast = 12;
const int polygon_filled = 20, polygon_outline = 21;
const int restart = 1, quit = 0;
const int bbox_on = 30, bbox_off = 31;
const int collision_on = 40, collision_off = 41;

int mainWindow = 0;
int gameWindow = 0;
int scoreWindow = 0;

bool boundingBox = true;
bool collisionDetection = true;
GLenum polygonFill = FILLED;

int winWidth, winHeight, border = 6;
float aspectRatio; 
float animationSpeed = 0.1f;
double time;
char s[50];
Timer *scoreTimer = new Timer();
int score = 0;
BBox windowBox;

int totalEnemies = 0;
bool gameWon = false;


Cannon *myCannon;
vec3f cannonPos;
std::vector<Entity*> scene; // All objects minus myCannon


// If a is being invaded by b or vise versa
bool intersects(BBox &a, BBox &b)
{
	bool inX = (b.getLeft() >= a.getLeft() && b.getRight() <= a.getRight()) || (a.getLeft() >= b.getLeft() && a.getRight() <= b.getRight());
	/*bool inY = (b.minY >= a.minY && b.maxY <= a.maxY);

	bool case1 = (b.maxY > a.minY && b.maxY <= a.maxY) && inX;
	bool case2 = (b.minY < a.maxY && b.minY >= a.minY) && inX;
	bool case3 = (b.minX < a.maxX && b.minX >= a.minX) && inY;
	bool case4 = (b.maxX > a.minX && b.minX <= a.maxX) && inY;*/

	// B's top right corner is in A
	//bool tprght = (b.maxX > a.minX && b.maxY > a.minX) && (b.maxY > a.minY && b.maxY <= a.maxY);
	bool tprght = (b.getRight() >= a.getLeft() && b.getTop() >= a.getLeft()) && (b.getTop() >= a.getBottom() && b.getTop() <= a.getTop());
	bool tprghtR = (a.getRight() >= b.getLeft() && a.getTop() >= b.getLeft()) && (a.getTop() >= b.getBottom() && a.getTop() <= b.getTop());
	
	bool brmrght = (b.getRight() >= a.getLeft() && b.getTop() >= a.getLeft()) && (b.getBottom() >= a.getBottom() && b.getBottom() <= a.getBottom());
	bool brmrghtR = (a.getRight() >= b.getLeft() && a.getTop() >= b.getLeft()) && (a.getBottom() >= b.getBottom() && a.getBottom() <= b.getBottom());
	
	if (inX && (tprght || tprghtR || brmrght|| brmrghtR))
		return true;
	else return false;
}

void setProjection(int w1, int h1, int window = 0)
{
	float ratio; // All variables must be declared at top of function (because this is C) 
	if (h1 == 0) h1 = 1; // avoid divide by 0

	ratio = 1.0f * w1/h1;
	
	if (window == gameWindow)
		aspectRatio = 1.0f * w1/h1;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
    glViewport(0, 0, w1, h1);

	if (window == gameWindow) 
	{
		gluOrtho2D(-10.0*ratio, 10.0*ratio, -10.0, 10.0); 
		windowBox.setMinPoint(-10.0*ratio, -10.0f*ratio);
		windowBox.setMaxPoint(10.0f*ratio, 10.0f*ratio);
	}

	glMatrixMode(GL_MODELVIEW);
}

/*
 * Handles resizing the window and display window.
 * <p/>
 * This will maintain the aspect ratio of our shapes and not scale with our window. This is done because I care that my scene is as I designed it.
 */
void resize_window(int width, int height)
{
	if (height == 0) height = 1; // avoid divide by 0

	winWidth = width;
	winHeight = height;

	// set gameWindow as the active window
	glutSetWindow(gameWindow);
	//gameWindow = glutCreateSubWindow(mainWindow, border, border, winWidth - border, (winHeight*3/4) - border); //winHeight - winHeight/4,, winHeight/4 - border*3/2
	// resize and reposition the game window
	glutPositionWindow(border, border);
	glutReshapeWindow(winWidth - border, (winHeight*3/4) - border);
	setProjection(winWidth - border, (winHeight*3/4) - border, gameWindow);

	// set scoreWindow as the active window
	glutSetWindow(scoreWindow);

	// resize and reposition the score window
	glutPositionWindow(border, (winHeight*3/4) + border); //glutPositionWindow(border, winHeight - (winHeight + border)/4);
	glutReshapeWindow(winWidth - border, (winHeight - (winHeight)/4) - 2*border);
	setProjection(winWidth - border, (winHeight - (winHeight)/4) - 2*border, scoreWindow);

	glutSetWindow(mainWindow);
}

// Only when key up is pressed
void processNormalKeys(unsigned char key, int x, int y) {
	int mod = glutGetModifiers();

	
	if (key == 27)
	{
		scoreTimer->stop();
		glutDestroyWindow(gameWindow);
		glutDestroyWindow(scoreWindow);
		glutDestroyWindow(mainWindow);
		exit(0);
	} else if (key == ' ') 
	{
		// Shoot
		myCannon->fire();
	}

	glutSetWindow(mainWindow);
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {

	int mod;
	mod = glutGetModifiers();
	static float *oldPos = myCannon->getPosition();
	
	if (key == GLUT_KEY_LEFT) 
	{
		// Move cannon left
		cannonPos[0] -= 0.5f;
	} else if (key == GLUT_KEY_RIGHT) 
	{
		// Move cannon right
		cannonPos[0] += 0.5f;
	} else if (key == GLUT_KEY_UP) 
	{
		// Rotate cannon down
		myCannon->rotateZ(5);
		//cannonPos[1] += 0.5f;
	} else if (key == GLUT_KEY_DOWN) 
	{
		// Rotate cannon up
		myCannon->rotateZ(-5);
		//cannonPos[1] -= 0.5f;
	}

	
	myCannon->setPosition(cannonPos);

	glutSetWindow(mainWindow);
	glutPostRedisplay();
}

void processMenuEvents(int option) {

	switch (option) 
	{
		case quit:
		{
			scoreTimer->stop();
			glutDestroyWindow(gameWindow);
			glutDestroyWindow(scoreWindow);
			glutDestroyWindow(mainWindow);
			exit(0);
			break;
		}
		case restart:
		{
			scoreTimer->stop();
			restartGame();
			break;
		}
		case animate_slow:
		{
			//myCannon->setFireSpeed(0.1f);
			animationSpeed = 0.1f;
			break;
		}
		case animate_medium:
		{
			//myCannon->setFireSpeed(0.2f);
			animationSpeed = 0.2f;
			break;
		}
		case animate_fast:
		{
			//myCannon->setFireSpeed(1.2f);
			animationSpeed = 0.4f;
			break;
		}
		case polygon_filled:
		{
			polygonFill = FILLED;
			break;
		}
		case polygon_outline:
		{
			polygonFill = OUTLINE;
			break;
		}
		case bbox_on:
		{
			boundingBox = true;
			break;
		}
		case bbox_off:
		{
			boundingBox = false;
			break;
		}
		case collision_on:
		{
			collisionDetection = true;
			break;
		}
		case collision_off:
		{
			collisionDetection = false;
			break;
		}

	}

	glutSetWindow(mainWindow);
	glutPostRedisplay();
}

// DONE //
void createGLUTMenus() {

	int menu, animationMenu, polygonMenu, bboxMenu, collisionMenu;
	
	glutSetWindow(gameWindow);
	// subwindow1: Shooting Menu - quit, pick animation speed (submenu), restart, polygons
	// Pick animation speed sub menu - slow, medium, fast
	// Polygons - Filled, Outline
	
	animationMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Slow", animate_slow);
	glutAddMenuEntry("Medium", animate_medium);
	glutAddMenuEntry("Fast", animate_fast);

	polygonMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Filled", polygon_filled);
	glutAddMenuEntry("Outline", polygon_outline);

	bboxMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("On", bbox_on);
	glutAddMenuEntry("Off", bbox_off);

	collisionMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("On", collision_on);
	glutAddMenuEntry("Off", collision_off);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Animation Speed", animationMenu);
	glutAddSubMenu("Polygon Draw", polygonMenu);
	glutAddSubMenu("Bounding Box", bboxMenu);
	glutAddSubMenu("Collision", collisionMenu);
	glutAddMenuEntry("Restart", restart);
	glutAddMenuEntry("Quit", quit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutSetWindow(mainWindow);

}

void renderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection (flips coor system for easy bitmap rendering)
	gluOrtho2D(0, winWidth, winHeight, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void restoreNormalProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScoreScene() 
{
	glutSetWindow(scoreWindow);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// glLookAt

	if (gameWon)
	{
		sprintf(s,"You Won!          Score: %d", score);
		scoreTimer->stop();
	} else {
		// display time elapsed in top window
		sprintf(s,"Time: %4.2f          Score: %d", time, score);
	}

	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(border+5,border+30,0,GLUT_BITMAP_HELVETICA_12,s);
	glPopMatrix();

	restoreNormalProjection();

	glutSwapBuffers();
}

void renderGameScene() 
{
	glutSetWindow(gameWindow);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// glLookAt

	// Draw all object's bounding boxes (including the main windows')
	if (boundingBox)
	{
		// Draw window's BBox if we are debugging
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(windowBox.minX, windowBox.minY);
			glVertex2f(windowBox.maxX, windowBox.minY);
			glVertex2f(windowBox.maxX, windowBox.maxY);
			glVertex2f(windowBox.minX, windowBox.maxY);
		glEnd();

		for (int i = 0; i < scene.size(); i++)
		{
			scene[i]->drawBoundingBox();
		}

		myCannon->setDrawBBox(true);
		myCannon->drawBoundingBox();
	} else {
		myCannon->setDrawBBox(false);
	}



	if (polygonFill == FILLED)
	{
		// TODO
	} else 
	{

	}


	for (int i = 0; i < scene.size(); i++)
	{
		scene[i]->draw();
	}

	myCannon->draw();

	glutSwapBuffers();
}
/* 
 * The rountine display() gets called each time the window is redrawn. (MAIN WINDOW)
 */
void renderScene()
{
	// Ensure we are working with ModelView matrix before render
	glMatrixMode(GL_MODELVIEW);
	glutSetWindow(mainWindow);

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	renderGameScene();
	renderScoreScene();

	/* ready to draw now! forces buffered OGL commands to execute */
	glutSwapBuffers();
}

void updateScene()
{
	static int enemyCount = 0;
	static Planet *planet;

	if (enemyCount >= totalEnemies) 
	{
		// You won! 
		gameWon = true;
		return;
	}

	// Update enemies movement/entities (speed)
	for (int i = 0; i < scene.size(); i++)
	{
		if (scene[i]->isVisible())
		{
			scene[i]->move(animationSpeed);
		}
	}

	myCannon->move(animationSpeed);

	std::vector<Bullet*> bullets = myCannon->getBullets();
	Moon** moons = NULL;

	if (collisionDetection)
	{
		for (int i = 0; i < scene.size(); i++)
		{
			if (scene[i]->isVisible() && myCannon->intersects(scene[i]->getBoundingBox()))
			{
				printf("my cannon interesects object %d\n", scene[i]->getType());
			}

			// Check all bullets and whether they interact with current object
			for (int j = 0; j < bullets.size(); j++)
			{
				if (bullets[j]->isVisible())
				{
					float *pos = bullets[j]->getPosition();

					if (pos[0] <= windowBox.getLeft() || pos[0] >= windowBox.getRight() ||
						pos[1] <= windowBox.getBottom() || pos[1] >= windowBox.getTop())
					{
						bullets[j]->setVisible(false);
						//printf("bullet %d out of bounds\n", j);
					}
					
					if (scene[i]->isVisible() && scene[i]->getType() == ENTITY_PLANET)
					{
						planet = (Planet*) scene[i];
						moons = planet->getMoons();

						// Check against moons first
						for (int k = 0; k < 2; k++)
						{
							if (moons[k]->isVisible() && intersects(bullets[j]->getBoundingBox(), moons[k]->getBoundingBox()))
							{
								//printf("Bullet hit a moon!\n");
								moons[k]->setVisible(false);
								score += 2;
								enemyCount++;
								bullets[j]->setVisible(false);
							}
						}
					} 

					if (scene[i]->isVisible() && intersects(bullets[j]->getBoundingBox(), scene[i]->getBoundingBox()))
					{
						//printf("bullet %d hit %d\n", j, scene[i]->getType());
						if (scene[i]->getType() == ENTITY_ENEMY)
						{
							scene[i]->setVisible(false);
							score++;
							enemyCount++;
						} else if (scene[i]->getType() == ENTITY_PLANET)
						{
							scene[i]->setVisible(false);
							score += 2;
							enemyCount++;
						}

						bullets[j]->setVisible(false);
					}
				}
			}
		}
	} // End of if (collisionDetection)
	else
	{

		// I will still check if bullet is out of window's view
		// Check all bullets and whether they interact with current object
		for (int j = 0; j < bullets.size(); j++)
		{
			if (bullets[j]->isVisible())
			{
				float *pos = bullets[j]->getPosition();

				if (pos[0] <= windowBox.getLeft() || pos[0] >= windowBox.getRight() ||
					pos[1] <= windowBox.getBottom() || pos[1] >= windowBox.getTop())
				{
					bullets[j]->setVisible(false);
				}
			}
		}
	}

	// Update time
	double new_time = scoreTimer->getElapsedTimeInSec();

	if (new_time >= 1)
	{
		time = new_time;
		glutPostRedisplay();
	}
}

void setupGame() 
{
	Wall *w1;
	Enemy *e1;
	Planet *p1;

	w1 = new Wall();
	w1->setPosition(2.0f, 4.5f, 0.0f);
	w1->setDefaultDir(1);
	w1->setStationary(false);
	w1->setId(WALL_RIGHT); // WALL_RIGHT
	scene.push_back( w1 );

	w1 = new Wall();
	w1->setPosition(-3.0f, 4.0f, 0.0f);
	w1->setDefaultDir(-1);
	w1->setStationary(false);
	w1->setId(WALL_LEFT); // WALL_RIGHT
	scene.push_back( w1 );

	// Planets
	p1 = new Planet();
	p1->setStaticOrbit(true);
	p1->setId(PLANET_S);
	p1->setPosition(4.0f, 8.2f, 0.0f);
	scene.push_back( p1 );
	totalEnemies++;

	
	p1 = new Planet();
	p1->setStaticOrbit(false);
	p1->setId(PLANET_D);
	p1->setPosition(-4.0f, 8.2f, 0.0f);
	scene.push_back( p1 );
	totalEnemies++;

	// Place in for loop (10 enemies)
	for (int i = 0; i < 10; i++)
	{
		e1 = new Enemy();
		e1->setPosition(-windowBox.getLeft() + i*3.2f, 5.0f, 0.0f); // (windowBox.getRight() - windowBox.getLeft())/i
		scene.push_back(e1);
		totalEnemies++;
	}

	myCannon = new Cannon();
	cannonPos[0] = 0.0f;
	cannonPos[1] = -10.0f;
	cannonPos[2] = 0.0f;
	myCannon->setPosition(cannonPos);

	boundingBox = false;
	collisionDetection = true;
	
	scoreTimer->start();
}

void restartGame() 
{
	int c = 0;
	for (int i = 0; i < scene.size(); i++)
	{
		scene[i]->setVisible(true);
		if (scene[i]->getType() == ENTITY_ENEMY)
		{
			scene[i]->setPosition(-windowBox.getLeft() + c*3.2f, 5.0f, 0.0f);
			c++;
		} else if (scene[i]->getType() == ENTITY_WALL)
		{
			if (scene[i]->getId() == WALL_LEFT)
			{
				scene[i]->setPosition(-3.0f, 4.0f, 0.0f);
			} else 
			{
				scene[i]->setPosition(2.0f, 4.0f, 0.0f);
			}
		} else if (scene[i]->getType() == ENTITY_PLANET)
		{
			if (scene[i]->getId() == PLANET_S)
			{
				scene[i]->setPosition(4.0f, 8.2f, 0.0f);
			} else 
			{
				scene[i]->setPosition(-4.0f, 8.2f, 0.0f);
			}
		}
	}

	cannonPos[0] = 0.0f;
	cannonPos[1] = -10.0f;
	cannonPos[2] = 0.0f;
	myCannon->setDrawBBox(true);
	myCannon->setPosition(cannonPos);
	myCannon->setRotation(0.0f);

	animationSpeed = 0.1f;
	boundingBox = false;
	collisionDetection = true;

	// Ensure any fired bullets are "destroyed" on a restart
	std::vector<Bullet*> bullets = myCannon->getBullets();
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->isVisible())
			bullets[i]->setVisible(false);
	}
	
	time = 0;
	score = 0;
	polygonFill = FILLED;
	boundingBox = true;
	gameWon = false;

	scoreTimer->start();
	
}

/* Initialize states -- called before */

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* set current color to white */
	//glColor3f(1.0, 1.0, 1.0);

	glShadeModel(GL_SMOOTH);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* the window will correspond to these world coorinates */
	/*gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	windowBox.setMinPoint(-1.0f, -1.0f);
	windowBox.setMaxPoint(1.0f, 1.0f);*/

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Glut stuff
	//glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(processNormalKeys); 
	glutSpecialFunc(processSpecialKeys); // Up
}

/* The main program */

//int main(int argc, char** argv)
int main(int argc, char *argv[])
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	mainWindow = glutCreateWindow("Assignment 2");
	winWidth = 500;
	winHeight = 500;

	/* create a callback routine for (re-)display */
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize_window);
	glutIdleFunc(updateScene);

	// Game Window
	gameWindow = glutCreateSubWindow(mainWindow, border, border, winWidth - border, (winHeight*3/4) - border); //winHeight - winHeight/4,, winHeight/4 - border*3/2
	glutDisplayFunc(renderGameScene);
	init();

	createGLUTMenus();

	// Score Window Pos: (border, gameWindow.height + border) (I don't want a top border on scoreWindow)
	scoreWindow = glutCreateSubWindow(mainWindow, border, (winHeight*3/4) + border, winWidth - border, (winHeight - (winHeight)/4) - border); 
	glutDisplayFunc(renderScoreScene);
	init();

	setupGame();

	glutSetWindow(mainWindow);

	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
	return 1;
}
