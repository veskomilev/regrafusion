#include <QPainter>

#include "displaywidget.h"

DisplayWidget::DisplayWidget(QWidget* parent)
    : QOpenGLWidget {parent}
{

}

void DisplayWidget::initializeGL()
{
    fprintf(stderr, "initializeGL\n");
}


void DisplayWidget::paintGL()
{
    fprintf(stderr, "paintGL\n");
    QPainter painter(this);

    painter.setPen(Qt::red);
    painter.drawRect(0, 0, 500, 500);


    painter.fillRect(10, 10, 480, 480, Qt::red);
}

void DisplayWidget::resizeGL(int w, int h)
{

    fprintf(stderr, "resizeGL\n");
}
