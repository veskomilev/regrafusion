#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QPoint>
#include <QStatusBar>
#include <QOpenGLWidget>

#include "gfx/tree.h"

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
    void initializeCanvas(std::shared_ptr<QPainter> painter);

    void scaleGridSizes(float& grid_size, float& ruler_size, float& ruler_text_width);

    void drawGridAndAxes(std::shared_ptr<QPainter>  painter, float grid_size);

    void drawCoordinateLabels(std::shared_ptr<QPainter> painter);

    void drawRulerNumbers(std::shared_ptr<QPainter> painter, float grid_size, float ruler_size, float ruler_text_width);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    // limit view position to mitigate artifacts due to loss of floating point precision
    void limitViewPosition();

    void updateStatus();

    QStatusBar* status_bar_;

    QPointF window_size_;
    QPointF view_offset_;
    static constexpr QPointF kViewIdentity = QPointF(0.0f, 0.0f);
    static constexpr float kMaxViewOffset = 5000.0f;

    float view_scale_;
    static constexpr float kMinViewScale = 0.2f;
    static constexpr float kMaxViewScale = 100.0f;

    const uint kGridSize = 100U;

    // navigation related
    QPointF drag_start_position_;
    QPointF view_offset_before_drag_start_;

    // axes and ruler related
    static constexpr uint kLabelsOffset = 15;
    static constexpr uint kTextHeight = 20;
};

#endif // DISPLAYWIDGET_H
