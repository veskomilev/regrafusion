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

    void resetView();

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

    QPoint size_;
    QPoint view_offset_;
    const QPoint kViewIdentity;

    // navigation related
    QPoint drag_start_position_;
    QPoint view_offset_before_drag_start_;
};

#endif // DISPLAYWIDGET_H
