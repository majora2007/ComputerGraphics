#ifndef MYPANELOPENGL_H
#define MYPANELOPENGL_H

#include <QGLWidget>
#include <QMouseEvent>

class MyPanelOpenGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyPanelOpenGL(QWidget *parent = 0);
    enum Mode {rectangle, circle, triangle, custom, test};

signals:
    void drawModeChanged(Mode m);
    
public slots:
    void changeRectangleHeight(double h);
    void changeRectangleWidth(double w);
    void changeCircleRadius(double r);
    void changeCircleVerticies(int c);

    void setDrawMode(QString str);
    void mousePressEvent(QMouseEvent *event);
    void setCursorTriangle(int index);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
    double xMin, xMax, yMin, yMax; // perspective variables
    Mode myMode;
    int sides;
    double radius;

    double rect_height;
    double rect_width;
    double circle_radius;
    int circle_verticies;
    float triangle_point1[2];
    float triangle_point2[2];
    float triangle_point3[2];

    QPoint lastPos;
    int currPoint;

    void draw_rect2d(double width, double height);
    void draw_circle2d(double r, int numVerticies);
    void draw_triangle2d(float *p1, float *p2, float *p3);
    void draw_custom();
    
};

#endif // MYPANELOPENGL_H
