// Regrafusion - draws graphics recursively
// Distributed under GPL-3.0
// Copyright (C) 2023-2024  Vesko Milev

/*! \file uipainter.h */

#ifndef UIPAINTER_H
#define UIPAINTER_H

#include <memory>
#include <QPainter>

#include "rgf_ctx.h"
#include "gfx/tree.h"
#include "view.h"

//!  A class that paints auxiliary graphics to the user view buffer (coordinate axes, coordinate labels, etc.)
class UiPainter
{
public:
    //!
    //! \param view Current state of the viewport
    //! \param painter Pointer to the user view painter
    //!
    UiPainter(View view, std::shared_ptr<QPainter>  painter);

    ~UiPainter();

    void drawGridAndAxes();

    void drawCoordinateLabels();

    void drawRulerNumbers();

    //!
    //! Draws statistics about how fast the tree was drawn
    //!
    //! \param stats A reference to the tree statistics
    //!
    void drawStats(TreeStatistics& stats);

    //!
    //! Draws a label indicating which mode the context is in
    //!
    //! \param mode The context mode
    //!
    void drawCtxMode(RgfCtx::mode_t mode);

private:
    //!
    //! Scales grid offsets and sizes, so that the grid looks well at any viewport scale
    //!
    void scaleGrid();

    //!  Offset constant for drawing labels
    static constexpr uint kLabelsOffset = 15U;

    //!  Default text height
    static constexpr uint kTextHeight = 20U;

    //!  Default distance between grid lines
    static constexpr uint kGridSize = 100U;

    View view_;

    //!  Scaled grid size
    float grid_size_;

    //!   Scaled ruler numbers offset
    float ruler_size_;

    //!  Scaled ruler numbers text width
    float ruler_text_width_;

    std::shared_ptr<QPainter> painter_;
};

#endif // UIPAINTER_H
