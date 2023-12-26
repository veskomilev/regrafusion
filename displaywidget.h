#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QOpenGLWidget>
#include <QPoint>

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    DisplayWidget(QWidget* parent = NULL);

    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int w, int h) override;

private:
    QPoint size_;
    QPoint view_offset_;
    const QPoint kIdentity;
};



#endif // DISPLAYWIDGET_H
