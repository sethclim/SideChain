#include "MeterView.h"

#include <algorithm>

namespace ui
{

    MeterView::MeterView()
    {
        startTimer(1000 / 24);
    }

    MeterView::~MeterView()
    {
        stopTimer();
    }

    void MeterView::setValueSupplier(std::function<float()> valueSupplier)
    {
        valueSupplier_ = std::move(valueSupplier);
    }

    void MeterView::timerCallback()
    {
        redraw();
    }

    void MeterView::draw(visage::Canvas &canvas)
    {
        canvas.setColor(0xff000000);
        canvas.fill(0, 0, width(), height());

        float level = valueSupplier_ ? valueSupplier_() : kMinDb;
        float t = std::clamp((level - kMinDb) / (kMaxDb - kMinDb), 0.0f, 1.0f);
        float filledHeight = t * height();

        // Gradient is anchored to the full meter bounds (green top, red
        // bottom) the same way Source/UI/Meters/Meters.h's ColourGradient is
        // - only the bottom filledHeight is ever revealed, by painting over
        // the rest with the background color, so the visible color at a
        // given fill level always matches its position in that fixed gradient.
        canvas.setColor(visage::Brush::vertical(visage::Gradient(0xff00cc44, 0xffffcc00, 0xffff3333)));
        canvas.rectangle(0.0f, 0.0f, width(), height());

        canvas.setColor(0xff000000);
        canvas.rectangle(0.0f, 0.0f, width(), height() - filledHeight);
    }

}
