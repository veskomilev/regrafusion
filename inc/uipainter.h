// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

#ifndef UIPAINTER_H
#define UIPAINTER_H

#include <memory>
#include <QPainter>

#include "gfx/tree.h"
#include "view.h"

class UiPainter
{
public:
    UiPainter(View view, std::shared_ptr<QPainter>  painter);

    ~UiPainter();

    void drawGridAndAxes();

    void drawCoordinateLabels();

    void drawRulerNumbers();

    void drawStats(TreeStatistics& stats);

private:
    void scaleGrid();

    static constexpr uint kLabelsOffset = 15U;

    static constexpr uint kTextHeight = 20U;

    static constexpr uint kGridSize = 100U;

    View view_;

    float grid_size_;

    float ruler_size_;

    float ruler_text_width_;

    std::shared_ptr<QPainter> painter_;
};

#endif // UIPAINTER_H
