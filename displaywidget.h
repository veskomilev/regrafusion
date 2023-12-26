#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    DisplayWidget(QWidget* parent = NULL);

    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int w, int h) override;

};



#endif // DISPLAYWIDGET_H
