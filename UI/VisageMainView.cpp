#include "VisageMainView.h"

#include <algorithm>

namespace ui
{

    VisageMainView::VisageMainView()
    {
        window_.onDraw() = [this](visage::Canvas &canvas)
        {
            canvas.setColor(0xff1a1a1a);
            canvas.fill(0, 0, window_.width(), window_.height());

            float radius = std::min(window_.width(), window_.height()) * 0.15f;
            float x = window_.width() * 0.5f - radius;
            float y = window_.height() * 0.5f - radius;
            canvas.setColor(0xff33ccff);
            canvas.ring(x, y, 2.0f * radius, radius * 0.2f);
        };
    }

    VisageMainView::~VisageMainView()
    {
        remove();
    }

    void VisageMainView::embed(void *parentNativeHandle, int width, int height)
    {
        window_.setNativeWindowDimensions(width, height);
        window_.show(parentNativeHandle);
    }

    void VisageMainView::resize(int width, int height)
    {
        window_.setNativeWindowDimensions(width, height);
    }

    void VisageMainView::remove()
    {
        window_.removeFromWindow();
    }

}
