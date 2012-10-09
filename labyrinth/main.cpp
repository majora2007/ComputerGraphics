#include <math.h>
#include <freeglut.h>
#include <stdlib.h>					//- needed for exit() function
#include <imathvec.h>				//- Vector library
#include "loader.h"
#include "mesh.h"
#include <fstream>
using namespace std;

#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))

// --- Model Defines
#define CAR09 0
#define BUILDING 1
#define PERSON 2

// --- Menu Defines
#define TOPDOWN 11
#define DEG45 12
#define DLON 21
#define DLOFF 22
#define SHADEF 31
#define SHADES 32
#define CSPDSLOW 41
#define CSPDMED 42
#define CSPDFAST 43
#define CRESTART 44


// --- Window Parameters
GLint winID;//, subWinID_1;//, subWinID_2, subWinID_3;	//- unique window id
int width = 640;
int height = 640;
//int border = 6;
//int currentWindow;
int font = (int) GLUT_BITMAP_8_BY_13; //- Font used when rendering text on screen

// --- Controling Parameters

float angle = 0.0;
float deltaAngle = 0.0;

float x=0.0f,y=1.75f,z=5.0f;		//- eye coordinates
float lx=0.0f,ly=0.0f,lz=-1.0f;		//- line of sight vector

int deltaMove = 0;					//- 
float movSpeed = 0.1f;				//- Speed of movement [See MoveFlat()]


// --- Conditional Variables
bool useDL = true;					//- Use Display Lists or not
bool topdownView = true;				//- true: TOPDOWN, false: DEG45
bool BRAKE = false;					//- Whether brake has been applied or not
bool hideCP = false;				//- Whether to show/hide Control Panel (CP)

// --- Perspective Projection Parameters
float ratio;
float fieldofView = 45.0f;
float nearPlane = 1.0f;
float farPlane = 1000.0f;

// --- Data
static const char* modelfile[] = {"./Models/Required/cars09_obj/Car_09.obj", "./Models/Required/Building.obj"};
Mesh* model = 0;					//- Current model being used
GLint snowman_display_list;			//- Display list 

// --- Scene Parameters
Vec3f bbxMin, bbxMax, bbxCenter;	//- Bounding box vectors of model


void calc_bbox(Vec3f&, Vec3f&);		//- Calculate the bounding box's values
void load_model(char*, int);		//- Loads model into Mesh model and assigns ID
void draw_model(void);				//- Draws the model 
void shut(void);					//- Deletes model
void changeSize(int, int);			//- Handles resizing window
void drawBuilding(void);			//- Draws temp buildings (DEBUG ONLY)
void drawSnowMan(void);				//- Draws snowmen (DEBUG ONLY)
GLuint createDL(void);				//- Creates a display list (used for buildings)
void moveMeFlat(int);				//- Responsible for rotating camera
void orientMe(float);				//- Responsible for the camera movement
//void changeSize2(int, int);			//- Recalc projection matrix
void renderScene2(void);				//- Render callback for window2 (cp)
void renderScene(void); 
void renderSceneAll(void);
void move(int);						//- Keeps car moving unitl BREAK is pressed
void processMenuEvents(int);
void createGLUTMenus(void);
void setOrthographicProjection(void);
void resetPerspectiveProjection(void);

void createBox(); //- Creates a box
void createCylinder(); //- Creates a cylinder
void createSphere(); //- Creates a sphere
void renderBitmapString(float, float, float,void*, char*); //- render string on screen



// ---------- Model Functions ---------- //

/* Compute the bounding box of the model
 */
void calc_bbox(Vec3f& bbxmin, Vec3f& bbxmax)
{
	vector<Vec3f>& v = model->m_v; // array of vertices

	if(v.empty())
		return;

	bbxmax = bbxmin = v[0];

	for(unsigned int i = 1; i < v.size(); i++)
	{
		bbxmin.x = MIN2(bbxmin.x, v[i].x);
		bbxmin.y = MIN2(bbxmin.y, v[i].y);
		bbxmin.z = MIN2(bbxmin.z, v[i].z);

		bbxmax.x = MIN2(bbxmin.x, v[i].x);
		bbxmax.y = MIN2(bbxmin.y, v[i].y);
		bbxmax.z = MIN2(bbxmin.z, v[i].z);
	}
}

/* Load a model from a file 
 * Param: name 
 */
void load_model(const char* name, int SYMB)
{
	if(model)
		delete model;
	
	model = ObjLoader::load(name);
	model->ID = SYMB;
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin + bbxMax) * 0.5f;
}

/* Handles deleting model and freeing up space */
void shut()
{
	if (model)
		delete model;
}

/* Draw the model in immediate mode */
void draw_model()
{
	//data references for convenience purpose
	vector<int>& vi = model->m_vi;
	vector<int>& ni = model->m_ni;
	vector<int>& ti = model->m_ti;
	vector<Vec3f>& v = model->m_v;
	vector<Vec3f>& n = model->m_n;
	vector<Vec2f>& t = model->m_texc;

	// drawing in immediate mode

	float scale = ratio / 100.0f;

	if(model->ID == CAR09)
	{
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	}
	else if(model->ID == BUILDING)
	{
		glScalef(scale, scale, scale);
	}

	

	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // determines the angle (rotation) of model based on x,y,z axis
	glBegin(GL_TRIANGLES);
		for(unsigned int i = 0; i < vi.size(); i++)
		{
			//assign some funny color to each vertex
			Vec3f offset = (v[vi[i]] - bbxCenter);
			glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			
			
			glVertex3fv(&v[vi[i]].x);
		}
	glEnd();
}




// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------

//Moves object forward until BRAKE is pressed (DWN)
void move(int value)
{
	if(BRAKE == false)
	{
		value += -1;
		moveMeFlat(value);

		// Tell GLUT to call update again in 25 ms
		glutTimerFunc(25, move, 0);
	}
	else
		BRAKE = true;
		
	glutPostRedisplay();
}

// Added ability to handle subwindows
void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	width = w;
	height = h;

	ratio = 1.0f * width / height;

	//Reset the coor system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set the viewport to be entire window
	glViewport(0,0, width, height);

	//Set the clipping volume
	gluPerspective(fieldofView, ratio, nearPlane, farPlane);

	//gluLookAt(x, y, z, lx, lz, ly, 0.0f, 1.0f, 0.0f);
	//glMatrixMode(GL_MODELVIEW);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


//subwindow 1 - camera = current position
void renderScenesw1(void)
{
	glLoadIdentity();
	gluLookAt(x, y, z,
			  (x + lx), (y + ly), (z + lz),
			  0.0f, 1.0f, 0.0f);


	renderScene2();
}

// subwindow 2 - top view
void renderScenesw2(void)
{
	glLoadIdentity();
	/*gluLookAt(x, y+15, z,
			  x, y - 1, z, 
			  lx, 0, lz);*/

	gluLookAt(x, y+15, z,
			  x, y-1, z, 
			  lx, 0, lz);
	
	renderScene2();
}

/* Idel Rendering Callback.
 * Handles: movement changes
 *			viewport changes
 *			hide/show CP <-remove comment
 */
void renderSceneAll()
{
	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle)
	{
		angle += deltaAngle;
		orientMe(angle);
	}

	if (topdownView == true)  //set view to top down 
	{
		glLoadIdentity();
		gluLookAt(x, y+15, z,
				  x, y - 1, z, 
				  lx, 0, lz);
	}
	else
	{
		glLoadIdentity();
		gluLookAt(x, y, z,
				  (x - lx), (y + ly), (z - lz),
				  0.0f, 1.0f, 0.0f);
	}


	renderScene2();
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, width, 0, height/2);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -height/2.0f, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}


void renderScene2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draws pointer to track current location (x,y,z)
	glPushMatrix();
		glTranslatef(x, 0.0f, z);
		glRotatef(90.0-angle*180.0/3.14, 0.0, 1.0, 0.0);
		//glRotatef(90.0-angle*360.0/3.14, 0.0, 1.0, 0.0); // does some weird stuff
		draw_model();
	glPopMatrix();

	

// Draw ground (needs adjustment) add start/finish placers
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);

		/*glColor3f(0.7f, 0.1f, 0.3f); // start box 
		glVertex3f(-2.0f, 0.0f, -5.0f-2.0f);
		glVertex3f(-2.0f, 0.0f, -5.0f+2.0f);
		glVertex3f(+2.0f, 0.0f, -5.0f+2.0f);
		glVertex3f(+2.0f, 0.0f, -5.0f-2.0f);*/
	glEnd();

	// Draw 36 Buildings w/ Display Lists
	if(useDL == true)
	{
		glCallList(snowman_display_list);
	}
	else
	{
		// load BUILDING model
		//load_model(modelfile[BUILDING], BUILDING);
		// wants us to use glBegin and glEnd
		for(int i = -3; i < 3; i++)
		{
			for(int j = -3; j < 3; j++)
			{
				glColor3f(i*0.3f, (i*(j+0.2f))/0.1f, i*j/0.1f); //random colors
				glPushMatrix();
				glTranslatef(i*10.0, 0, j * 10.0);
				glutSolidCube(2.8f);//drawBuilding();//draw_model();
				glPopMatrix();
			}
		}

		// if I need to draw more objects to screen, 
		// I can load them then use the loops above with 
		// some tweened multiples of i,j.
	}
	

	// Render CP (via text)
	if (hideCP == false)  //show CP
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		renderBitmapString(0.0f, height - 13.0f, 0.0f, (void *)font, "Use [Left/Right Key] to turn car.");
		renderBitmapString(0.0f, height - 26.0f, 0.0f, (void *)font, "Use [Up Key] to accelerate.");
		renderBitmapString(0.0f, height - 39.0f, 0.0f, (void *)font, "Use [Down Key] to brake.");
		renderBitmapString(0.0f, height - 52.0f, 0.0f, (void *)font, "Press [Mouse Right Key] to open menu.");
		renderBitmapString(0.0f, height - 65.0f, 0.0f, (void *)font, "Press 'h' to hide the Control Panel.");
		//renderBitmapString(0.0f, height - 52.0f, 0.0f, s); fps
		glPopMatrix();
	}
	else //hide CP
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();	
		glColor3f(0.0f, 0.0f, 0.0f);
		renderBitmapString(0.0f, height - 13.0f, 0.0f, (void *)font, "Press 'h' to show the Control Panel.");
		glPopMatrix();
	}
	

	
	glutSwapBuffers();
}








//main window
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
	
}

void pressKey(int key, int x, int y)
{

	switch (key)
	{
		case GLUT_KEY_LEFT :
			deltaAngle = -0.03f;
			break;
		case GLUT_KEY_RIGHT :
			deltaAngle = 0.03f;
			break;
		case GLUT_KEY_UP :
			deltaMove = -1;
			BRAKE = false;
			move(deltaMove);
			break;
		case GLUT_KEY_DOWN :
			//deltaMove = 1; // reverse car
			deltaMove = 0; // brake
			BRAKE = true;
			break;
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT : 
		case GLUT_KEY_RIGHT : 
			deltaAngle = 0.0f;
			break;
		case GLUT_KEY_UP : 
		case GLUT_KEY_DOWN : 
			deltaMove = 0;
			break;
	}
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27 : // Exit the program
		exit(0);
	case 'h': // hide: if CP is hidden, show. if shown, hide it.
		{

		if(hideCP == true)
			hideCP = false;
		else if(hideCP == false)
			hideCP = true;
		//glutPostRedisplay();
		break;
		}
	}
}

//Initializes 3D Rendering
void initScene()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH); //allows for color blending
	//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);
		//glEnable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL); //enables colors

	//glutIgnoreKeyRepeat(1);

	load_model(modelfile[CAR09], CAR09); //- Load default model
	useDL = true;
	topdownView = false;

	//if (useDL == true)
		snowman_display_list = createDL(); //- Create a display list
	
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	winID = glutCreateWindow("My Labyrinth");
	
		glutKeyboardFunc(processNormalKeys);
		glutSpecialFunc(pressKey);
		glutSpecialUpFunc(releaseKey);

		//glutDisplayFunc(renderScene2);
		glutDisplayFunc(renderSceneAll); //still a problem with display list (laggy)
		glutIdleFunc(renderSceneAll);

		glutReshapeFunc(changeSize);
		
		initScene();

		createGLUTMenus();

	/*subWinID_1 = glutCreateSubWindow(winID, 
					1, 1, 
					width, height);

		glutDisplayFunc(renderScene);
		initScene();*/

	/*subWinID_2 = glutCreateSubWindow(winID, 
			width+border,(height+border)/2,
			width-2*border, height/2 - border*3/2);

		glutDisplayFunc(renderScenesw2);
		initScene();*/




	glutMainLoop();

	shut();

	return(0);
}


// ---------- Non-Callback Handlers ---------- //



/* Responsible for rotating camera by computing the appropriate values 
 * for the new x and z components of the line of sight vector. Note we are 
 * only moving along the XZ plane, therefore we don't need to change the y coor
 * of the line of sight vector.
 */
void orientMe(float ang) {
	lx = sin(ang);
	lz = -cos(ang);
	//glutPostRedisplay();
}

/* Responsible for the camera movement. We want to move the camera along line of sight, so 
 * the next camera position must be alont the line of sight vector. We are going to add 
 * a fraction (0.1) of the line of sight vector to our current position.
 * direction: -1 or 1 (backwards or forwards)
 * fraction (0.1): a possible speed implementation. We know that (lx,lz) is a unitary vector
 * (it's a point in the unit circle), therefore if the fraction is kept const, then the speed will be 
 * kept const as well. by increasing the fraction we're moving faster (in each frame).
 * Is used when FWD/BWD is pressed 
 * Param: deltaMove
 */
void moveMeFlat(int i) {
	x = x + i*(lx)*movSpeed;
	z = z + i*(lz)*movSpeed;
	//glutPostRedisplay();
}

GLuint createDL()
{
	GLuint snowManDL;

	// Create the id for the list
	snowManDL = glGenLists(2);

	glNewList(snowManDL+1,GL_COMPILE);
		//drawBuilding();
		drawSnowMan();
	glEndList();

	// start list
	glNewList(snowManDL,GL_COMPILE);

	// call the function that contains the rendering commands
	for(int i = -3; i < 3; i++)
	{
		for(int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			glCallList(snowManDL+1);
			glPopMatrix();
		}
	}

	// endList
	glEndList();

	return(snowManDL);
}

void drawSnowMan()
{

// Set color of body to white
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body	
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);


// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}



void drawBuilding()
{
	glColor3f(0.4f, 0.65f, 0.23f);
	glutSolidCube(2.8f);
}




void createGLUTMenus()
{
	int menu, viewMenu, dlMenu, shadingMenu, carSpeedMenu;

	// Menu to handle view changes
	viewMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Top Down", TOPDOWN); // TOPDOWN = 11
	glutAddMenuEntry("45 Degree Angle", DEG45); // DEG45 = 12

	// Menu to handle Displaylists (on, off)
	dlMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("ON", DLON); // DLON = 21
	glutAddMenuEntry("OFF", DLOFF); //DLOFF = 22

	// Menu to handle shading 
	shadingMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Flat", SHADEF); // SHADEF = 31
	glutAddMenuEntry("Smooth", SHADES); // SHADES = 32

	carSpeedMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Slow", CSPDSLOW);
	glutAddMenuEntry("Medium", CSPDMED);
	glutAddMenuEntry("Fast", CSPDFAST);
	glutAddMenuEntry("Restart", CRESTART);


	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("View", viewMenu);
	glutAddSubMenu("Display Lists", dlMenu);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Car Animation Speed", carSpeedMenu);
	glutAddMenuEntry("Quit", -1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void processMenuEvents(int option)
{
	switch(option)
	{
	case (11): //Top Down View
		topdownView = true;
		break;
	case (12): //45 Degree View
		topdownView = false;
		break;
	case (21): // Display Lists ON
		useDL = true;
		break;
	case (22): // Display Lists OFF
		useDL = false;
		break;
	case (31): // Shading: FLAT
		glEnable(GL_FLAT);
		break;
	case (32): // Shading: SMOOTH
		glEnable(GL_SMOOTH);
		break;
	case (41): // Car Animation Speed: Slow
		movSpeed = 0.1;
		break;
	case (42): // Car Animation Speed: Medium
		movSpeed = 0.25;
		break;
	case (43): // Car Animation Speed: Fast
		movSpeed = 0.4;
		break;
	case (44): // Place car back in starting position
		break;
	case (-1): // QUIT
		exit(0);
		break;
	}


}

/* Creates a bitmap out of passed string */
void renderBitmapString(float x, float y, float z, void* font, char *string)
{
	char *c;

	glRasterPos3f(x, y, z); // fonts position
	for(c = string; *c != '\0'; c++)
		glutBitmapCharacter(font, *c);
}