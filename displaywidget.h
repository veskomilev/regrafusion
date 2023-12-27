#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

class DisplayWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    DisplayWidget(QWidget* parent = NULL);

    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int w, int h) override;

    void resetViewPosition();

    void resetViewScale();

    void setStatusBar(QStatusBar* const& bar);

private:
    void initializeCanvas(QPainter *painter);

    void scaleGridSizes(float& grid_size, float& ruler_size);

    void drawGridAndAxes(QPainter *painter, float grid_size);

    void drawCoordinateLabels(QPainter *painter);

    void drawRulerNumbers(QPainter *painter, float grid_size, float ruler_size);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void updateStatus();

    QStatusBar* status_bar_;

    QPointF window_size_;
    QPointF view_offset_;
    const QPointF kViewIdentity;

    float view_scale_;
    float kMinViewScale;
    float kMaxViewScale;

    const uint kGridSize = 100U;

    // navigation related
    QPointF drag_start_position_;
    QPointF view_offset_before_drag_start_;

    // axes and ruler related
    static constexpr uint kLabelsOffset = 15;
    static constexpr uint kTextHeight = 20;
    static constexpr uint kRulerTextWidth = 50;
};

#endif // DISPLAYWIDGET_H
