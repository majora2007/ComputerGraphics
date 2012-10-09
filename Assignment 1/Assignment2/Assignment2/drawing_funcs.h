#ifndef __DRAWING_FUNCS_H_
#define __DRAWING_FUNCS_H_

#include "glut.h"
#include <cmath>
#define PI 3.1415926535898 


static void draw_rect2d(float width, float height, GLenum drawStyle = GL_POLYGON)
{
    glBegin(drawStyle);
        glVertex2f(-1.0f * width, 1.0f * height);
        glVertex2f(-1.0f * width, -1.0f * height);

        glVertex2f(1.0f * width, -1.0f * height);
        glVertex2f(1.0f * width, 1.0f * height);
    glEnd();
}

/**
  * This draws polygons by placing verticies along a unit circle * radius.
  */
static void draw_circle2d(float radius, int numVerticies, GLenum drawStyle = GL_POLYGON)
{
    int i;
    float angle;

    glBegin(drawStyle);

        // Plot numVerticies number of points from center
        for (i = 0; i < numVerticies; i++)
        {
            angle = PI * 2 * i / numVerticies; // 2PI * i/v
            glVertex2f(radius * sin(angle), radius * cos(angle));
        }
    glEnd();
}

static void draw_triangle2d(float *p1, float *p2, float *p3, GLenum drawStyle = GL_LINE_LOOP)
{
    glBegin(drawStyle);
        glVertex2fv(p1);
        glVertex2fv(p2);
        glVertex2fv(p3);
    glEnd();
}

static void draw_triangle2d(float size, GLenum drawStyle = GL_LINE_LOOP)
{
    glBegin(drawStyle);
        glVertex2f(-size, 0.0f);
        glVertex2f(size, 0.0f);
        glVertex2f(-size, size);
    glEnd();
}

static void draw_star(int size, GLenum drawStyle = GL_LINES)
{
	//glPushMatrix();
	glPushAttrib(11);

	int i;
    float angle;
	glLineWidth(2.0f);
    glBegin(drawStyle);
        for (i = 0; i < 8; i++)
        {
            angle = 3.14159 * 2 * i / 8; // 2PI * i/v
            glVertex2f((1.0f*size) * sin(angle), (1.0f*size) * cos(angle));
            glVertex2f((-1.0f*size) * sin(angle), (-1.0f*size) * cos(angle));
        }
    glEnd();

	glPopAttrib();
	//glPopMatrix();
}

static void draw_fireballs(float size, float fireballSize, int numVerts, GLenum drawStyle = GL_POLYGON)
{
	glPushMatrix();
	int i;
    float angle;
	for (i = 0; i < 8; i++)
    {
        angle = 3.14159 * 2 * i / 8; // 2PI * i/v
        glTranslatef(size * sin(angle), size * cos(angle), 0.0f);
        draw_circle2d(fireballSize, numVerts, drawStyle);
		glTranslatef(-size * sin(angle), -size * cos(angle), 0.0f);
    }
	glPopMatrix();
}


/**
  * This is my custom function in which I will draw something fun.
  */
static void draw_custom()
{
    // First draw the picture frame
	glPushMatrix();
	glPushAttrib(12);

    glLineWidth(3.0f);
    glColor3f(0.6f, 0.0f, 0.2f);
    glBegin(GL_LINE_LOOP);
		glVertex2f(-2.0f, -2.0f);
		glVertex2f(2.0f, -2.0f);
		glVertex2f(2.0f, 2.0f);
		glVertex2f(-2.0f, 2.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
		glVertex2f(-1.8f, -1.8f);
		glVertex2f(1.8f, -1.8f);
		glVertex2f(1.8f, 1.8f);
		glVertex2f(-1.8f, 1.8f);
    glEnd();

    // Draw insides - star (circle)

    // Inside: Draw Lines
    int i;
    float angle;

    glBegin(GL_LINES);
        for (i = 0; i < 8; i++)
        {
            angle = 3.14159 * 2 * i / 8; // 2PI * i/v
            glColor3f(angle, angle, angle);
            glVertex2f(1.0f * sin(angle), 1.0f * cos(angle));
            glColor3f(angle/128, angle/128, angle/128);
            glVertex2f(-1.0f * sin(angle), -1.0f * cos(angle));
        }
    glEnd();

    // Inside: Draw Circles on each tip
    for (i = 0; i < 8; i++)
    {
        // Every time I will reset matrix and re-translate to center so that I can properly translate to end of line.
        glLoadIdentity();
        glTranslatef(5.0f, 5.0f, 0.0f);
        angle = 3.14159 * 2 * i / 8; // 2PI * i/v
        glTranslatef(1.0f * sin(angle), 1.0f * cos(angle), 0.0f);
        glColor3f(0.1f, 0.0f, 1.0f);
        draw_circle2d(0.1f, 16);
    }

	glPopAttrib();
	glPopMatrix();
}


#endif