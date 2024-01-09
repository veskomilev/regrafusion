// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#include "uipainter.h"
#include "view.h"

UiPainter::UiPainter(View view, std::shared_ptr<QPainter>  painter) :
    view_(view),
    grid_size_(kGridSize * view_.scale),
    ruler_size_(kGridSize),
    ruler_text_width_(25),
    painter_(painter)
{
    scaleGrid();
}

UiPainter::~UiPainter()
{
}

void UiPainter::scaleGrid()
{
    // hardcoded values for precise control over scaling
    if (grid_size_ < 2) {
        grid_size_ *= 100;
        ruler_size_ = 10'000;
        ruler_text_width_ = 55;
    } else if (grid_size_ < 2.5) {
        grid_size_ *= 40;
        ruler_size_ = 4000;
        ruler_text_width_ = 55;
    } else if (grid_size_ < 5) {
        grid_size_ *= 20;
        ruler_size_ = 2000;
        ruler_text_width_ = 45;
    } else if (grid_size_ < 20) {
        grid_size_ *= 10;
        ruler_size_ = 1000;
        ruler_text_width_ = 45;
    } else if (grid_size_ < 25) {
        grid_size_ *= 4;
        ruler_size_ = 400;
        ruler_text_width_ = 35;
    } else if (grid_size_ < 50) {
        grid_size_ *= 2;
        ruler_size_ = 200;
        ruler_text_width_ = 35;
    } else if (grid_size_ < 200) {
        // do nothing - identity
        ruler_text_width_ = 35;
    } else if (grid_size_ < 500) {
        grid_size_ /= 5;
        ruler_size_ = 20;
    } else if (grid_size_ < 1000) {
        grid_size_ /= 10;
        ruler_size_ = 10;
    } else if (grid_size_ < 2000) {
        grid_size_ /= 20;
        ruler_size_ = 5;
    } else if (grid_size_ < 5000) {
        grid_size_ /= 50;
        ruler_size_ = 2;
    } else if (grid_size_ < 20000) {
        grid_size_ /= 100;
        ruler_size_ = 1;
    } else {
        grid_size_ /= 500;
        ruler_size_ = 0.2;
    }
}

void UiPainter::drawGridAndAxes()
{
    float left_edge = -view_.offset.x();
    float right_edge = view_.size.x() - view_.offset.x();
    float top_edge = -view_.offset.y();
    float bottom_edge = view_.size.y() - view_.offset.y();

    // draw coordinate grid
    painter_->setPen(Qt::gray);
    for (float x = grid_size_; x < right_edge; x += grid_size_) {
        if (x < left_edge)
            continue;

        painter_->drawLine(x + view_.offset.x(), 0, x + view_.offset.x(), view_.size.y());
    }

    for (float x = -grid_size_; x > left_edge; x -= grid_size_) {
        if (x > right_edge)
            continue;

        painter_->drawLine(x + view_.offset.x(), 0, x + view_.offset.x(), view_.size.y());
    }

    for (float y = grid_size_; y < bottom_edge; y += grid_size_) {
        if (y < top_edge)
            continue;

        painter_->drawLine(0, y + view_.offset.y(), view_.size.x(), y + view_.offset.y());
    }

    for (float y = -grid_size_; y > top_edge; y -= grid_size_) {
        if (y > bottom_edge)
            continue;

        painter_->drawLine(0, y + view_.offset.y(), view_.size.x(), y + view_.offset.y());
    }

    // draw coordinate axes
    painter_->setPen(Qt::black);
    painter_->drawLine(0, view_.offset.y(), view_.size.x(), view_.offset.y());
    painter_->drawLine(view_.offset.x(), 0, view_.offset.x(), view_.size.y());
}

void UiPainter::drawCoordinateLabels()
{
    float left_edge = kLabelsOffset;
    float right_edge = view_.size.x() - kLabelsOffset;
    float top_edge = kLabelsOffset;
    float bottom_edge = view_.size.y() - kLabelsOffset;

    float label_left_edge = kLabelsOffset / 2;
    float label_right_edge = view_.size.x() - kLabelsOffset;
    float label_top_edge = kLabelsOffset / 2;
    float label_bottom_edge = view_.size.y() - kTextHeight;

    const QPointF& grid_origin = view_.offset; // rename for clarity's sake

    float xpos;
    float ypos;

    painter_->setPen(Qt::black);

    // use min/max to make labels stick to edges of the screen
    // common y coordinate for 'x' labels
    ypos = grid_origin.y() + kLabelsOffset;
    ypos = fmin(ypos, bottom_edge);
    ypos = fmax(ypos, top_edge);

    xpos = label_right_edge;
    xpos = fmax(xpos, grid_origin.x() + kLabelsOffset);
    painter_->drawText(xpos, ypos, "x");

    xpos = label_left_edge;
    xpos = fmin(xpos, grid_origin.x() - kLabelsOffset);
    painter_->drawText(xpos, ypos, "-x");


    // common x coordinate for 'y' labels
    xpos = grid_origin.x() - kLabelsOffset / 2;
    xpos = fmin(xpos, right_edge);
    xpos = fmax(xpos, left_edge);

    ypos = label_bottom_edge;
    ypos = fmax(ypos, grid_origin.y() + kLabelsOffset);
    painter_->drawText(
        QRectF(QPointF(0, ypos),
               QPointF(xpos, ypos + kTextHeight)),
        Qt::AlignRight,
        "y");

    ypos = label_top_edge;
    ypos = fmin(ypos, grid_origin.y() - kLabelsOffset);
    painter_->drawText(
        QRectF(QPointF(0, ypos),
               QPointF(xpos, ypos + kTextHeight)),
        Qt::AlignRight,
        "-y");
}

void UiPainter::drawRulerNumbers()
{
    float left_edge = ruler_text_width_;
    float right_edge = view_.size.x() - kLabelsOffset;
    float top_edge = kLabelsOffset;
    float bottom_edge = view_.size.y() - kLabelsOffset;

    const QPointF& grid_origin = view_.offset; // rename for clarity's sake

    painter_->setPen(Qt::black);

    // positive x coordinates
    // use min/max to make labels stick to edges of the screen
    // subtract an additional ruler_text_width_ in condition, so that numbers aren't overlaid on top of 'x' label
    for (float x = ruler_size_; x * view_.scale < right_edge - grid_origin.x() - ruler_text_width_; x += ruler_size_) {

        float xpos = x * view_.scale + grid_origin.x();
        float ypos = grid_origin.y() + kLabelsOffset;

        if (xpos < left_edge)
            continue;

        ypos = fmin(ypos, bottom_edge);
        ypos = fmax(ypos, top_edge);

        painter_->drawText(xpos, ypos, QString::number(x));
    }

    // negative x coordinates
    for (float x = -ruler_size_; x * view_.scale > left_edge - grid_origin.x(); x -= ruler_size_) {

        float xpos = x * view_.scale + grid_origin.x();
        float ypos = grid_origin.y() + kLabelsOffset;

        if (xpos > right_edge)
            continue;

        ypos = fmin(ypos, bottom_edge);
        ypos = fmax(ypos, top_edge);

        painter_->drawText(xpos, ypos, QString::number(x));
    }

    // negative y coordinates
    // subtract an additional kLabelsOffset in condition, so that numbers aren't overlaid on top of 'y' label
    for (float y = ruler_size_; y * view_.scale < bottom_edge - grid_origin.y() - kLabelsOffset; y += ruler_size_) {

        float ypos = y * view_.scale + grid_origin.y();
        float xpos = grid_origin.x() - kLabelsOffset / 2;

        if (ypos < top_edge)
            continue;

        xpos = fmin(xpos, right_edge);
        xpos = fmax(xpos, left_edge);

        painter_->drawText(
            QRectF(QPointF(0, ypos),
                   QPointF(xpos, ypos + kTextHeight)),
            Qt::AlignRight,
            QString::number(y));
    }

    // positive y coordinates
    for (float y = -ruler_size_; y * view_.scale > top_edge - grid_origin.y(); y -= ruler_size_) {

        float ypos = y * view_.scale + grid_origin.y();
        float xpos = grid_origin.x() - kLabelsOffset / 2;

        if (ypos > bottom_edge)
            continue;

        xpos = fmin(xpos, right_edge);
        xpos = fmax(xpos, left_edge);

        painter_->drawText(
            QRectF(QPointF(0, ypos),
                   QPointF(xpos, ypos + kTextHeight)),
            Qt::AlignRight,
            QString::number(y));
    }
}

void UiPainter::drawStats(TreeStatistics& stats)
{
    int avg_time_to_draw_tree = vector_average<uint>(stats.render_time_us);
    int avg_time_to_draw_first_branch = vector_average<uint>(stats.first_branch_render_time_us);
    int avg_time_to_draw_last_branch = vector_average<uint>(stats.last_branch_render_time_us);
    int avg_time_to_draw_branch = vector_average<uint>(stats.avg_branch_render_time_us);

    painter_->setPen(Qt::black);
    painter_->drawText(QRectF(kLabelsOffset, kLabelsOffset * 1.5, view_.size.x(), view_.size.y()),
                      "Average time to render the tree: " + QString::number(avg_time_to_draw_tree) + "µs\n" +
                      "Average time to render a branch: " + QString::number(avg_time_to_draw_branch) + "µs\n" +
                      "Average time to render first branch: " + QString::number(avg_time_to_draw_first_branch) + "µs\n" +
                      "Average time to render last branch: " + QString::number(avg_time_to_draw_last_branch) + "µs");
}
