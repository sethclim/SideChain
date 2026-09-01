#include "VisageMainView.h"

namespace ui
{

    VisageMainView::VisageMainView() : divisionButton_("Quarter")
    {
        window_.onDraw() = [this](visage::Canvas &canvas)
        {
            canvas.setColor(0xff1a1a1a);
            canvas.fill(0, 0, window_.width(), window_.height());
        };

        window_.addChild(dragView_);
        window_.addChild(divisionButton_);
        divisionButton_.onToggle() += [this](visage::Button *, bool)
        {
            if (divisionClicked_)
                divisionClicked_();
        };
    }

    VisageMainView::~VisageMainView()
    {
        remove();
    }

    void VisageMainView::embed(void *parentNativeHandle, int width, int height)
    {
        window_.setNativeWindowDimensions(width, height);
        layoutChildren(width, height);
        window_.show(parentNativeHandle);
    }

    void VisageMainView::resize(int width, int height)
    {
        window_.setNativeWindowDimensions(width, height);
        layoutChildren(width, height);
    }

    void VisageMainView::remove()
    {
        window_.removeFromWindow();
    }

    void VisageMainView::setDivisionLabel(const std::string &label)
    {
        divisionButton_.setText(label);
    }

    void VisageMainView::setDivisionClickedCallback(std::function<void()> callback)
    {
        divisionClicked_ = std::move(callback);
    }

    void VisageMainView::setCurvePoints(std::vector<std::pair<float, float>> normalizedPoints)
    {
        dragView_.setPoints(std::move(normalizedPoints));
    }

    void VisageMainView::setOnCurveNodeDragged(std::function<void(int, float, float)> callback)
    {
        dragView_.setOnNodeDragged(std::move(callback));
    }

    void VisageMainView::layoutChildren(int width, int height)
    {
        float buttonWidth = 140.0f;
        float buttonHeight = 36.0f;
        float dragAreaHeight = height * 0.7f;

        dragView_.setBounds(0.0f, 0.0f, (float)width, dragAreaHeight);
        divisionButton_.setBounds(width * 0.5f - buttonWidth * 0.5f, dragAreaHeight + (height - dragAreaHeight) * 0.5f - buttonHeight * 0.5f,
                                  buttonWidth, buttonHeight);
    }

}
