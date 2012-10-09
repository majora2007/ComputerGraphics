#include "mypanelopengl.h"
#include <cmath>
#include "GL/glu.h"
#include <time.h>

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent) :
    QGLWidget(parent)
{
    sides = 3;
    radius = 1.0;

    myMode = test; // Test was for debugging, but left in so that we start on a blank canvas
    rect_width = 1.0;
    rect_height = 1.0;
    circle_radius = 1.0;
    circle_verticies = 5;

    currPoint = 1; // 1, 2, or 3 (triangle point selected)
}

void MyPanelOpenGL::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**
  * Reference: http://www.cs.tufts.edu/comp/175/support/Tutorial.pdf
  */
void MyPanelOpenGL::resizeGL(int width, int height)
{
    xMin = 0; xMax = 10; yMin = 0; yMax = 10;
    glViewport(0, 0, (GLint) width, (GLint) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f); // viewport size is 2

    if (width > height)
    {
        height = height ? height : 1;
        double newWidth = (xMax - xMin) * width / height;
        double diffWidth = newWidth - (xMax - xMin);
        xMin = 0.0 + diffWidth / 2.0;
        xMax = 10 + diffWidth / 2.0;
    } else {
        width = width ? width : 1;
        double newHeight = (yMax - yMin) * width / height;
        double diffHeight = newHeight - (yMax - yMin);
        yMin = 0.0 + diffHeight / 2.0;
        yMax = 10 + diffHeight / 2.0;
    }

    gluOrtho2D(xMin, xMax, yMin, yMax); //viewportWidth = abs(xMax) + abs(xMin); viewportHeight = abs(yMax) + abs(yMin);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MyPanelOpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(5.0, 5.0, 0.0); // The middle of the screen is located at (5, 5, 0), thus I translate.

    switch (myMode)
    {
        case rectangle:
        {
            glColor3f(1.0f, 1.0f, 0.0f);
            draw_rect2d(rect_width, rect_height);
            break;
        }
        case circle:
        {
			glColor3f(0.0f, 1.0f, 0.0f);
            draw_circle2d(circle_radius, circle_verticies);
            break;
        }
        case triangle:
        {
            draw_triangle2d(triangle_point1, triangle_point2, triangle_point3);
            break;
        }
        case custom:
        {
            draw_custom();
            break;
        }
    }

    glFlush();
}


void MyPanelOpenGL::draw_rect2d(double width, double height)
{
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f * width, 1.0f * height);
        glVertex2f(-1.0f * width, -1.0f * height);

        glVertex2f(1.0f * width, -1.0f * height);
        glVertex2f(1.0f * width, 1.0f * height);
    glEnd();
}

/**
  * This draws polygons by placing verticies along a unit circle * radius.
  */
void MyPanelOpenGL::draw_circle2d(double radius, int numVerticies)
{
    int i;
    float angle;

    glBegin(GL_POLYGON);

        // Plot numVerticies number of points from center
        for (i = 0; i < numVerticies; i++)
        {
            angle = 3.14159 * 2 * i / numVerticies; // 2PI * i/v
            glVertex2f(radius * sin(angle), radius * cos(angle));
        }
    glEnd();
}

void MyPanelOpenGL::draw_triangle2d(float *p1, float *p2, float *p3)
{
    // Only draw if all points have been defined.
    if ( (triangle_point3 == NULL) | (triangle_point2 == NULL) | (triangle_point1 == NULL) )
    {
        return;
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2fv(p1);
            glVertex2fv(p2);
            glVertex2fv(p3);
        glEnd();
    }
}

/**
  * This is my custom function in which I will draw something fun.
  */
void MyPanelOpenGL::draw_custom()
{
    // First draw the picture frame
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

}

void MyPanelOpenGL::changeRectangleHeight(double h)
{
    rect_height = h;
    updateGL();
}

void MyPanelOpenGL::changeRectangleWidth(double w)
{
    rect_width = w;
    updateGL();
}

void MyPanelOpenGL::changeCircleRadius(double r)
{
    circle_radius = r;
    updateGL();
}

void MyPanelOpenGL::changeCircleVerticies(int c)
{
    circle_verticies = c;
    updateGL();
}

void MyPanelOpenGL::setDrawMode(QString str)
{
    if (QString::compare(str, "Rectangle", Qt::CaseInsensitive) == 0)
    {
        myMode = rectangle;
    } else if (QString::compare(str, "Circle", Qt::CaseInsensitive) == 0)
    {
        myMode = circle;
    } else if (QString::compare(str, "Triangle", Qt::CaseInsensitive) == 0)
    {
        myMode = triangle;
    } else if (QString::compare(str, "Custom", Qt::CaseInsensitive) == 0)
    {
        myMode = custom;
    }

    emit(drawModeChanged(myMode));
    updateGL();
}

/**
  * What this function does is on a mouse press, it sets a vector to hold a specific triangle point.
  * Now, there is one problem in that the window's coordinate system is different from our drawing coordinate
  * system. In order to draw correctly, we transform the (x,y) from window to (x',y') of our world coordinate
  * system.
  *
  * The way I did this was to use gluUnProject which returns the transformed coordinates. The reason I pass the y
  * value in as I do is because y starts at upper left and I need to ensure that y is subtracted from viewport[3]
  * which holds the viewHeight(yMax + yMin). The z coordinate is passed as 0.0f as we are only drawing in 2D space.
  *
  * Reference: http://www.opengl.org/discussion_boards/showthread.php/126012-converting-window-coordinates-to-world-coordinates
  */
void MyPanelOpenGL::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    // Get the position of where the mouse clicked and set that as triangle point
    lastPos = event->pos();

    GLdouble   x;
    GLdouble   y;
    GLdouble   z;

    GLdouble projection[16]; //var to hold the projection matrix info
    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    gluUnProject( lastPos.x(), (float)viewport[3] - (float)lastPos.y(), 0.0f, modelview, projection, viewport, &x, &y, &z);


    if (currPoint == 1)
    {
        triangle_point1[0] = x;
        triangle_point1[1] = y;
        glLoadIdentity();
        glTranslated(5.0, 5.0, 0.0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POINT);
            glVertex2fv(triangle_point1);
        glEnd();

        //emit(trianglePoint1Changed());
    } else if (currPoint == 2)
    {
        triangle_point2[0] = x;
        triangle_point2[1] = y;
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POINT);
            glVertex2fv(triangle_point2);
        glEnd();

        //emit(trianglePoint1Changed());
    } else if (currPoint == 3)
    {
        triangle_point3[0] = x;
        triangle_point3[1] = y;
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POINT);
            glVertex2fv(triangle_point3);
        glEnd();

        //emit(trianglePoint1Changed());
    }

    updateGL();
}

void MyPanelOpenGL::setCursorTriangle(int index)
{
    currPoint = ++index;
}
