#include <math.h>
#include <stdlib.h>					//- needed for exit() function
#include "glut.h"
#include <imathvec.h>				//- Vector library
#include "loader.h"
#include "mesh.h"
#include <fstream>
using namespace std;



#define DEGREES_TO_RADIANS 3.14159/180.0
#define MIN2(a,b) (((a) < (b))?(a):(b))
#define MAX2(a,b) (((a) > (b))?(a):(b))



typedef float vec3f[3];


const int CANNON = 1;
#define CAR01 0

// GLOBALS
// These are used to identify menu options
const int quit = 0;
const int flat_shading = 50, smooth_shading = 51, no_shading = 52;
const int per_face_normals = 60, per_vertex_normals_weighted = 61;
const int normalsViz_on = 70, normalsViz_off = 71;


int shading = 1; // 0 - none, 1 - flat, 2 - smooth
bool normalViz = false; 


// --- Perspective Projection Parameters
float ratio;
float fieldofView = 45.0f;
float nearPlane = 1.0f;
float farPlane = 1000.0f;

float x=0.0f,y=1.75f,z=5.0f;		//- eye coordinates
float lx=0.0f,ly=0.0f,lz=-1.0f;		//- line of sight vector


int deltaMove = 0;					//- 
float movSpeed = 0.1f;				//- Speed of movement [See MoveFlat()]
float angle = 0.0;
float deltaAngle = 0.0;



std::vector<Mesh*> scene; // All objects minus myCannon
Mesh *groundPlane =0;

static const char* modelfile[] = {"./Models/Car_01.obj", "./Models/3DEye.obj", "./Models/al.obj"};
Mesh* model = 0;					//- Current model being used
Vec3f bbxMin, bbxMax, bbxCenter;	//- Bounding box vectors of model


void calc_bbox(Vec3f&, Vec3f&);		//- Calculate the bounding box's values
void load_model(char*, int);		//- Loads model into Mesh model and assigns ID
void draw_model(void);				//- Draws the model 
void moveMeFlat(int);				//- Responsible for rotating camera
void orientMe(float);				//- Responsible for the camera movement


Mesh* createRectMesh(float width, float height, float depth);
Mesh* createConeMesh(float height, float radius, int numVerts);
Mesh* createCylinderMesh(float height, float radius, int numVerts);

// ---------- Model Functions ---------- //

/* Compute the bounding box of the model
 */
void calc_bbox(Vec3f& bbxmin, Vec3f& bbxmax)
{
	std::vector<Vec3f>& v = model->m_v; // array of vertices

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
Mesh* load_model(const char* name, int SYMB)
{
	Mesh *m;

	/*if(model)
		delete model;*/
	
	m = ObjLoader::load(name);
	m->ID = SYMB;
	calc_bbox(bbxMin, bbxMax);
	bbxCenter = (bbxMin + bbxMax) * 0.5f;
	return m;
}

/* Draw the model in immediate mode */
void draw_model(Mesh* mmodel, GLenum drawMode = GL_TRIANGLE_STRIP) //GL_TRIANGLES
{
	//data references for convenience purpose
	std::vector<int>& vi = mmodel->m_vi;
	std::vector<int>& ni = mmodel->m_ni;
	std::vector<int>& ti = mmodel->m_ti;
	std::vector<Vec3f>& v = mmodel->m_v;
	std::vector<Vec3f>& n = mmodel->m_n;
	std::vector<Vec2f>& t = mmodel->m_texc;

	// drawing in immediate mode

	float scale = ratio / 100.0f;

	if(mmodel->ID == CAR01)
	{
		//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // I can have the "camera" be the car in top down
	}
	

	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // determines the angle (rotation) of model based on x,y,z axis
	glBegin(drawMode);
		for(unsigned int i = 0; i < vi.size(); i++)
		{
			//assign some funny color to each vertex
			Vec3f offset = (v[vi[i]] - bbxCenter);
			glColor3f(fabs(sin(offset.x)), fabs(cos(offset.y)), fabs(offset.z));
			
			glVertex3fv(&v[vi[i]].x);
		}
	glEnd();
}

void calc_vertex_normals_avg(Mesh *model)
{
	//data references for convenience purpose
	std::vector<int>& vi = model->m_vi;
	std::vector<int>& ni = model->m_ni;
	std::vector<int>& ti = model->m_ti;
	std::vector<Vec3f>& v = model->m_v;
	std::vector<Vec3f>& n = model->m_n;
	std::vector<Vec2f>& t = model->m_texc;


}


/* Responsible for rotating camera by computing the appropriate values 
 * for the new x and z components of the line of sight vector. Note we are 
 * only moving along the XZ plane, therefore we don't need to change the y coor
 * of the line of sight vector.
 */
void orientMe(float ang) {
	lx = sin(ang);
	lz = -cos(ang);
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
}


/*
 * Handles resizing the window and display window.
 * <p/>
 * This will maintain the aspect ratio of our shapes and not scale with our window. This is done because I care that my scene is as I designed it.
 */
void resize_window(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	w = w;
	h = h;

	ratio = 1.0f * w / h;

	//Reset the coor system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Set the viewport to be entire window
	glViewport(0,0, w, h);

	//Set the clipping volume
	gluPerspective(fieldofView, ratio, nearPlane, farPlane);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void processNormalKeys(unsigned char key, int xp, int yp) {
	
	if (key == 27)
	{
		exit(0);
	} else if (key == 97 || key == 65) // a || A
	{
		// Elevate up
		y += 0.5f;
	} else if (key == 115 || key == 83) // s || S
	{
		// Elevate down
		y += -0.5f;
	}

	glutPostRedisplay();
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

void processSpecialKeys(int key, int x, int y) {

	switch (key)
	{
		case GLUT_KEY_LEFT :
			deltaAngle += -0.03f;
			break;
		case GLUT_KEY_RIGHT :
			deltaAngle += 0.03f;
			break;
		case GLUT_KEY_UP :
			deltaMove += -1;
			break;
		case GLUT_KEY_DOWN :
			deltaMove += 1; // reverse car
			break;
	}

	
	glutPostRedisplay();
}

void processMenuEvents(int option) 
{
	switch (option) 
	{
		case quit:
		{
			exit(0);
			break;
		}
		case flat_shading:
		{
			glShadeModel(GL_FLAT);
			shading = 1;
			break;
		}
		case smooth_shading:
		{
			glShadeModel(GL_SMOOTH);
			shading = 2;
			break;
		}
		case no_shading:
		{
			shading = 0;
			break;
		}
		case per_face_normals:
		{
			// TODO
			break;
		}
		case per_vertex_normals_weighted:
		{
			//TODO
			break;
		}
		case normalsViz_on:
		{
			normalViz = true;
			break;
		}
		case normalsViz_off:
		{
			normalViz = false;
			break;
		}
		default:
			break;

	}

	glutPostRedisplay();
}


void createGLUTMenus() {

	int menu, shadingMenu, normalsMenu, normalVizMenu;

	shadingMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Flat", flat_shading);
	glutAddMenuEntry("Smooth", smooth_shading);
	glutAddMenuEntry("None", no_shading);

	normalsMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Per Face Normals", per_face_normals);
	glutAddMenuEntry("Per Vertex Normals Weighted", per_vertex_normals_weighted);

	normalVizMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("On", normalsViz_on);
	glutAddMenuEntry("Off", normalsViz_off);


	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Shading", shadingMenu);
	glutAddSubMenu("Normals", normalsMenu);
	glutAddSubMenu("Normal Visualization", normalVizMenu);
	glutAddMenuEntry("Quit", quit);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


/* 
 * The rountine display() gets called each time the window is redrawn. (MAIN WINDOW)
 */
void renderScene()
{
	static GLenum drawMode;

	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle)
	{
		angle += deltaAngle;
		orientMe(angle);
	}


	// Ensure we are working with ModelView matrix before render
	glMatrixMode(GL_MODELVIEW);

	/* clear the screen*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	gluLookAt(x, y, z,
				  (x - lx), (y + ly), (z - lz),
				  0.0f, 1.0f, 0.0f);


	if (shading == 0)
	{
		drawMode = GL_LINE_LOOP;
	} else if (shading == 1)
	{
		glShadeModel(GL_FLAT);
		drawMode = GL_TRIANGLE_STRIP;
	} else if (shading == 2)
	{
		glShadeModel(GL_SMOOTH);
		drawMode = GL_TRIANGLE_STRIP;
	}

	// Draws pointer to track current location (x,y,z)
	/*glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslatef(x, 0.1f, z);
		glRotatef(90.0-angle*180.0/3.14, 0.0, 1.0, 0.0);
		draw_model(drawMode);
	glPopMatrix();*/
	


	glPushMatrix();
	// Draw ground
	glColor3f(1.0f, 0.0f, 0.0f);
	/*glBegin(drawMode);
		glVertex3f(-1000.0f, 0.0f, -1000.0f);
		glVertex3f(-1000.0f, 0.0f, 1000.0f);
		glVertex3f(1000.0f, 0.0f, -1000.0f);
		glVertex3f(1000.0f, 0.0f, -1000.0f);
		glVertex3f(-1000.0f, 0.0f, 1000.0f);
		glVertex3f(1000.0f, 0.0f, 1000.0f);
	glEnd();*/
	
	//draw_model(groundPlane, drawMode);

	glPopMatrix();

	glColor3f(0.0f, 0.0f, 1.0f);
	draw_model(model, drawMode);


	/*for(int i = -3; i < 3; i++)
	{
		for(int j = -3; j < 3; j++)
		{
			glColor3f(i*0.3f, (i*(j+0.2f))/0.1f, i*j/0.1f); //random colors
			glPushMatrix();
			glTranslatef(i*10.0, 1.0f, j * 10.0);

			draw_model(scene[1], drawMode);
				
			glPopMatrix();
		}
	}*/
	


	/*for (int i = 1; i < scene.size(); i++)
	{
		model = scene[i];
		draw_model();
	}*/
	//model = scene[0];
	


	/* ready to draw now! forces buffered OGL commands to execute */
	glutSwapBuffers();
}

void updateScene()
{

}

void setupGame() 
{
	for (int i = 0; i < 3; i++)
	{
		model = new Mesh;
		model = load_model(modelfile[i], i);
		scene.push_back(model);
	}

	model = scene[0];

	model = createCylinderMesh(4.0f, 1.4f, 8); //createConeMesh(8.0f, 1.4f, 8);

	groundPlane = createRectMesh(100.0f, 0.0f, 100.0f);
}

/* Initialize states -- called before */

void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	/* set current color to white */
	glColor3f(1.0, 1.0, 1.0);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH); //allows for color blending
	glDisable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL); //enables colors

	// Load my default model here...

	/* identify the projection matrix that we would like to alter */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* identify the modeling and viewing matrix that can be modified from here on */
	/* we leave the routine in this mode in case we want to move the object around */
	/* or specify the camera */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Glut stuff
	glutKeyboardUpFunc(processNormalKeys); 

	glutSpecialFunc(processSpecialKeys);
	glutSpecialUpFunc(releaseKey);
}

/* The main program */

//int main(int argc, char** argv)
int main(int argc, char *argv[])
{

	/* create a window, position it, and name it */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Assignment 3");

	/* create a callback routine for (re-)display */
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize_window);
	glutIdleFunc(updateScene);
	
	init();

	createGLUTMenus();

	setupGame();


	/* entering the event loop */
	glutMainLoop();

	/* code here will not be executed */
	return 1;
}



Mesh* createRectMesh(float width, float height, float depth)
{
	Mesh *obj = new Mesh;

	//data references for convenience purpose
	std::vector<int>& vi = obj->m_vi;
	std::vector<int>& ni = obj->m_ni;
	std::vector<int>& ti = obj->m_ti;
	std::vector<Vec3f>& v = obj->m_v;
	std::vector<Vec3f>& n = obj->m_n;
	std::vector<Vec2f>& t = obj->m_texc;

	// Define 8 vertex indicies
	v.push_back(Vec3f(-1.0f * width, -1.0f * height, 1.0f * depth)); // 0
	v.push_back(Vec3f(1.0f * width, -1.0f * height, 1.0f * depth)); // 1
	v.push_back(Vec3f(1.0f * width, 1.0f * height, 1.0f * depth)); // 2
	v.push_back(Vec3f(-1.0f * width, 1.0f * height, 1.0f * depth)); // 3

	v.push_back(Vec3f(-1.0f * width, -1.0f * height, -1.0f * depth)); // 4 
	v.push_back(Vec3f(1.0f * width, -1.0f * height, -1.0f * depth)); // 5
	v.push_back(Vec3f(1.0f * width, 1.0f * height, -1.0f * depth)); // 6
	v.push_back(Vec3f(-1.0f * width, 1.0f * height, -1.0f * depth)); // 7


	// Front face
	vi.push_back(0); //glVertex3f(-1.0f, -1.0f, 1.0f);
	vi.push_back(3); //glVertex3f(-1.0f, 1.0f, 1.0f);
	vi.push_back(1); //glVertex3f(1.0f, -1.0f, 1.0f);

	vi.push_back(1); //glVertex3f(1.0f, -1.0f, 1.0f);
	vi.push_back(3); //glVertex3f(-1.0f, 1.0f, 1.0f);
	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);
	vi.push_back(1); //glVertex3f(1.0f, -1.0f, 1.0f);

	// Right (side) face
	vi.push_back(1); //glVertex3f(1.0f, -1.0f, 1.0f);
	vi.push_back(5); //glVertex3f(1.0f, -1.0f, -1.0f);
	vi.push_back(6); //glVertex3f(1.0f, 1.0f, 1.0f);
	vi.push_back(1); //glVertex3f(1.0f, -1.0f, -1.0f);

	vi.push_back(1); //glVertex3f(1.0f, -1.0f, -1.0f);
	vi.push_back(6); //glVertex3f(1.0f, 1.0f, -1.0f);
	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);
	vi.push_back(1); //glVertex3f(1.0f, -1.0f, -1.0f);

	// Top face
	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);
	vi.push_back(6); //glVertex3f(-1.0f, 1.0f, -1.0f);
	vi.push_back(7); //glVertex3f(1.0f, 1.0f, -1.0f);
	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);

	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);
	vi.push_back(7); //glVertex3f(-1.0f, 1.0f, -1.0f);
	vi.push_back(3); //glVertex3f(-1.0f, 1.0f, 1.0f);
	vi.push_back(2); //glVertex3f(1.0f, 1.0f, 1.0f);
	
	// Left (side) face (THIS NEEDS TO BE DOUBLE CHECKED)
	vi.push_back(3);
	vi.push_back(4);
	vi.push_back(7);
	vi.push_back(3);

	vi.push_back(3);
	vi.push_back(4);
	vi.push_back(0);
	vi.push_back(3);

	// Bottom face
	vi.push_back(0);
	vi.push_back(5);
	vi.push_back(1);
	vi.push_back(0);

	vi.push_back(0);
	vi.push_back(5);
	vi.push_back(4);

	// Back face
	vi.push_back(4);
	vi.push_back(6);
	vi.push_back(1);
	vi.push_back(4);

	vi.push_back(4);
	vi.push_back(6);
	vi.push_back(7);


	return obj;
}

// Center at (0,0,0)
Mesh* createConeMesh(float height, float radius, int numVerts)
{
	Mesh *obj = new Mesh;
	float x,y;

	// Vert Indecies
	//data references for convenience purpose
	std::vector<int>& vi = obj->m_vi;
	std::vector<int>& ni = obj->m_ni;
	std::vector<int>& ti = obj->m_ti;
	std::vector<Vec3f>& v = obj->m_v;
	std::vector<Vec3f>& n = obj->m_n;
	std::vector<Vec2f>& t = obj->m_texc;

	// Define vertex indicies
	v.push_back(Vec3f(0.0f, 0.0f, 0.0f)); // 0 (center vert)
	v.push_back(Vec3f(0.0f, height, 0.0f)); // 1 (top center vert)
	int i;
	for (i = 0; i < numVerts; i++) // v starts at 2
	{
		x = cos((2*3.14159) * i/numVerts);
		y = sin((2*3.14159) * i/numVerts);
		v.push_back(Vec3f(radius * x, 0.0f, radius * y));
	}
	printf("i = %d\n", i);
	for (i = 0; i < numVerts - 1; i++)
	{
		vi.push_back(0);

		vi.push_back(i+2);
		vi.push_back(i+3);
	}

	vi.push_back(0);
	vi.push_back(numVerts + 1);
	vi.push_back(2);

	

	// Create tent
	for (i = 0; i < numVerts - 1; i++)
	{
		vi.push_back(1);

		vi.push_back(i+2);
		vi.push_back(i+3);
	}

	vi.push_back(1);
	vi.push_back(numVerts + 1);
	vi.push_back(2);
	

	return obj;
}

Mesh* createCylinderMesh(float height, float radius, int numVerts)
{
	Mesh *obj = new Mesh;
	float x,y,z;

	// Vert Indecies
	//data references for convenience purpose
	std::vector<int>& vi = obj->m_vi;
	std::vector<int>& ni = obj->m_ni;
	std::vector<int>& ti = obj->m_ti;
	std::vector<Vec3f>& v = obj->m_v;
	std::vector<Vec3f>& n = obj->m_n;
	std::vector<Vec2f>& t = obj->m_texc;

	// Define vertex indicies
	v.push_back(Vec3f(0.0f, 0.0f, 0.0f)); // 0 (center vert)
	//v.push_back(Vec3f(0.0f, height, 0.0f)); // 1 (top center vert)
	
	int i;
	// Adds 7 vertecies
	for (i = 0; i < numVerts; i++) // v starts at 2
	{
		x = cos((2*3.14159) * i/numVerts);
		y = sin((2*3.14159) * i/numVerts);
		v.push_back(Vec3f(radius * x, 0.0f, radius * y * -1.0f));
	}

	v.push_back(Vec3f(0.0f, height, 0.0f)); // numVerts+1 (top center vert) (8)

	// Adds 7 vertecies
	for (i = 0; i < numVerts; i++) // v starts at 1//2
	{
		x = cos((2*3.14159) * i/numVerts);
		y = sin((2*3.14159) * i/numVerts);
		v.push_back(Vec3f(radius * x, height, radius * y));
	}

	printf("v size: %d\n", v.size());

	// Create bottom plate
	/*for (i = 1; i < numVerts ; i++)
	{
		vi.push_back(0);
		vi.push_back(i);
		vi.push_back(i+1);
	}

	vi.push_back(0);
	vi.push_back(numVerts);
	vi.push_back(1);*/

	printf("bottom plate, last coor: %d\n", 1);

	// Create parallel lines accross top and bottom (in this case, I will be using 2 triangles)

	vi.push_back(1);
	vi.push_back(10);
	vi.push_back(2);
	//vi.push_back(1);

	//vi.push_back(2);
	vi.push_back(10);
	vi.push_back(11);
	vi.push_back(2);

	
	/*vi.push_back(10);
	vi.push_back(3);
	vi.push_back(2); // loop back
	
	vi.push_back(3);
	vi.push_back(11);
	vi.push_back(12);*/

	/*vi.push_back(3 + numVerts);
	vi.push_back(4 + numVerts);
	vi.push_back(3 + numVerts);

	vi.push_back(4 + numVerts);
	vi.push_back(5 + numVerts);
	vi.push_back(4 + numVerts);*/


	/*vi.push_back(3 + numVerts);
	vi.push_back(2 + (numVerts));
	vi.push_back(2);

	vi.push_back(3 + (numVerts));
	vi.push_back(3);
	
	vi.push_back(3);
	vi.push_back(4+ numVerts);
	vi.push_back(3+ numVerts);
	vi.push_back(3);

	vi.push_back(4 + numVerts);
	vi.push_back(4);*/


	/*for (i = 2; i < numVerts; i+=2)
	{
		//if (i % 2 == 0) // Even
		//{
			vi.push_back((i+1) + numVerts);
			vi.push_back((i) + (numVerts));
			vi.push_back(i);

			vi.push_back((i+1) + (numVerts));
			vi.push_back((i+1));
			
		//} else {
			vi.push_back(i+1);
			vi.push_back((i+2)+ numVerts);
			vi.push_back(i+1 + numVerts);
			vi.push_back(i+1);

			vi.push_back((i+2) + numVerts);
			vi.push_back((i+2));
		//}
	}*/

	/*vi.push_back(1);
	vi.push_back(numVerts + 1);
	vi.push_back(2 + numVerts);

	printf("ll plate, last coor: %d\n", 1);

	// Create top plate
	for (i = 0; i < numVerts - 1; i++)
	{
		vi.push_back(1);

		vi.push_back((i+numVerts) + 2);
		vi.push_back((i+numVerts) + 3);
	}

	vi.push_back(1);
	vi.push_back((numVerts)*2);
	vi.push_back(2+numVerts); */

	// Create top plate
	/*for (i = 0; i < numVerts - 1; i++)
	{
		vi.push_back(numVerts+1); // top plate center
		vi.push_back(i+1 + numVerts);
		vi.push_back(i+2 + numVerts);
	}

	vi.push_back(1 + numVerts);
	vi.push_back(numVerts*2 + 1);
	vi.push_back(2 + numVerts);

	printf("top plate, last coor: %d\n", 2+numVerts);*/
	

	return obj;
}

template<class T> T triangle_area(const Vec3<T> &v1, const Vec3<T> &v2, const Vec3<T> &v3)
{
	Vec3<T> AB = v2-v1, AC = v3-v1;

	return (AB.cross(AV)).length()/2;
}