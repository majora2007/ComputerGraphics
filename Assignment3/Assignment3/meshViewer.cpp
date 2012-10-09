//#include <stdlib.h> //not sure
#include <windows.h> //to use exit() function
#include "glut.h" //OpenGL implementation
#include <imathvec.h> //Vector library
#include <fstream>
#include "loader.h"
#include "mesh.h"

using namespace std;

#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))

/* Global Variables */

// --- Window Parameters
GLint winID; //unique window id
int position_x = 100; //window position (x)
int position_y = 100; //window position (y)
int width = 800;
int height = 800;

// --- Controling Parameters
int mouse_button;
int mouse_x			= 0;
int mouse_y			= 0;
float scale			= 1.0;
float x_angle		= 0.0;
float y_angle		= 0.0;

// --- Perspective Projection Parameters
GLfloat fieldofview = 45.0; 
GLfloat aspect      = 1.0; //window scale ratio
GLfloat nearPlane   = 0.1; //don't draw if obj is closer than 0.1 from eye
GLfloat farPlane    = 100.0; //don't draw if obj is farther than 100 from the eye

float deltaAngle = 0.0; //control the rotation
static float x = 0.0f, y = 0.0f, z = 5.0f; //the camera position
static float lx = 0.0f, ly = 0.0f, lz = -1.0f; //a vector defining our line of sight
static float angle = y_angle; //the angle of rotation in the y axis. this variable will allow us to rotate the camera.
static float movSpeed = 0.3; // movement speed of camera

// --- Guide Variable
int frame = 0, time, timebase = 0;
char s[30];
//freame: the num of framces since we last computed the frame rate
//time: current num of miliseconds
//timebase: the time when we last computed the frame rate


// --- Scene Parameters
Vec3f bbxMin, bbxMax, bbxCenter;

// --- Mesh Model
Mesh* model = 0;

// --- Models
static const char* modelfile[] = {
	"./Models/al.obj",
	"./Models/Car_01.obj",
	"./Models/3DEye.obj",
	"./Models/a3.obj"
	};


// --- Menu 
#define QUIT -1




/* 
 * Facilities for drawing models. The functions below show:
 * - How to use ILM(Industrial Light & Magic)'s math library
 * - Mesh data structure
 * - How to draw the mesh in immediate mode
 */

// --- Compute the bounding box of the model --- //
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

// --- Load a model from a file --- //
void load_model(const char* name)
{
	if(model)
		delete model;
	
	model = ObjLoader::load(name);
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin + bbxMax) * 0.5f;
}

// --- Draw the model in immediate mode --- //
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

// --- Textures --- //
void prepTextures()
{
	// bind texture to id
	glBindTexture(GL_TEXTURE_2D, 13);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}


/* ----------------- My Callback Functions ----------------- */

// Handle's window resizing
void changeSize(int w, int h)
{
	width = w;
	height = h;

	// Prevent a divide by zero, when window is too short
	// (you can't make a window of zero width).
	if (height == 0)
		height = 1;

	//ratio = 1.0f * w / h;
	aspect = 1.0f * width / height;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective
	//gluPerspective(45, ratio, 1, 1000);
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f); //3 sets of parameters to set the camera position and orientation
	//1) indicate camera's position
	//2) defines the point we're looking at
	//3) idicates the up vector (usually set to (0.0, 1.0, 0.0)) meaning the camera isn't tilted.
	// if you want to view it upside down, try (0.0, -1.0, 0.0).
}

void renderBitmapString(float x, float y, float z, char *string)
{
	char *c;

	glRasterPos3f(x, y, z); // fonts position
	for(c = string; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}

// Responsible for rotating camera by computing the appropriate values 
// for the new x and z components of the line of sight vector. Note we are 
// only moving along the XZ plane, therefore we don't need to change the y coor
// of the line of sight vector.
void orientMe(float ang)
{
	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	
	float cz = 2.0f*(bbxMax.z - bbxMin.z);
	gluLookAt(0.0f, 0.0f, cz, lx + nearPlane, 0.0f, farPlane + lz, 0.0f, 1.0f, 0.0f);
}



//Display a triangle (animated)
void renderScene(void)
{

	// Calculate frames per second
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if(time - timebase > 1000)
	{
		//fps = frame*1000.0/(time-timebase));
		sprintf(s,"FPS:%4.2f", frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}

	// attempt at adding some smooth camera movement
	if(deltaAngle)
	{
		angle += deltaAngle;
		orientMe(angle);
	}

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);

	glPushMatrix();

	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(-bbxCenter.x, -bbxCenter.y, -bbxCenter.z);

	draw_model();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	renderBitmapString(0.0, height - 13.0f, 0.0f, "Use [Mouse Left Key] to rotate");
	renderBitmapString(0.0, height - 26.0f, 0.0f, "Use [Mouse Right Key] to scale");
	renderBitmapString(0.0, height - 39.0f, 0.0f, "Press [Mouse Middle Key] to open menu");
	renderBitmapString(0.0, height - 52.0f, 0.0f, s);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glutSwapBuffers();
}


void pressKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT :
		deltaAngle = -0.01f;
		break;
	case GLUT_KEY_RIGHT :
		deltaAngle = 0.01f;
		break;
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT :
	case GLUT_KEY_RIGHT :
		deltaAngle = 0.0f;
		break;
	}
}


void shut()
{
	if (model)
		delete model;
}

void processMenuEvents(int option)
{
	if(option == QUIT)
	{
		glutDestroyWindow(winID);
		shut();
		exit(0);
	}
	
	load_model(modelfile[option]);

	// adjust the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	farPlane = nearPlane + 4.0f * (bbxMax-bbxMin).length();
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);

	// adjust the viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float cz = 2.0f*(bbxMax.z - bbxMin.z);

	gluLookAt(0.0f, 0.0f, cz, 0.0f, 0.0f, cz - 1.0f, 0.0f, 1.0f, 0.0f);

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;

	mouse_button = button;
}

void motion(int x, int y)
{
	if(mouse_button == GLUT_LEFT_BUTTON)
	{
		// compute the angle [0-360] around x asix, and y axis
		y_angle += (float (x - mouse_x)/width)*360.0;
		x_angle += (float (y - mouse_y)/height)*360.0;
	}

	if(mouse_button == GLUT_RIGHT_BUTTON)
	{
		//scale
		scale += (y - mouse_y)/100.0;

		if(scale < 0.1) // too small
			scale = 0.1;
		if(scale > 7) // too big
			scale = 7;
	}

	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

/* ----------------- Windowing Functions ----------------- */

void createGLUTMenus()
{
	int menu, submenu;

	submenu = glutCreateMenu(processMenuEvents); //model menu
	glutAddMenuEntry("Al", 0);
	glutAddMenuEntry("Car01", 1);
	glutAddMenuEntry("Car14", 2);
	glutAddMenuEntry("Building", 3);

	menu = glutCreateMenu(processMenuEvents); //menu
	glutAddSubMenu("Models", submenu);
	glutAddMenuEntry("Quit", QUIT);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void init()
{
	// clear background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	//re-normalize normal (due to scale transforms)
	glEnable(GL_NORMALIZE);

	glDisable(GL_LIGHTING);

	// default: smooth shading
	glShadeModel(GL_SMOOTH);

	// load the first model by default
	load_model(modelfile[0]);

	// setup projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//aspect = (double)width/(double)height;
	aspect = 1.0f * width/height;
	farPlane = nearPlane+4.0f*(bbxMax-bbxMin).length();
	gluPerspective(fieldofview, aspect, nearPlane, farPlane);

	// setup viewing matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float cz = 2.0f*(bbxMax.z - bbxMin.z);

	gluLookAt(0.0f, 0.0f, cz, 0.0f, 0.0f, cz - 1.0f, 0.0f, 1.0f, 0.0f);

	// set up menus
	createGLUTMenus();
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(position_x, position_y);
	glutInitWindowSize(width, height);
	winID = glutCreateWindow("Snowman Example");

	// --- Keyboard Callback Registration --- //
	glutIgnoreKeyRepeat(1); //stop reporting key repeats
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// --- Screen Callback Registration --- //
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	init();

	glutMainLoop();

	shut();
}