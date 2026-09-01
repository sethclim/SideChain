#include "VisageMainView.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace ui
{
    namespace
    {
        // visage's own resize path (Window::setNativeWindowSize ->
        // windowContentsResized) adds windowBorderSize(hwnd) on top of the
        // requested size, which is meant for decorated top-level windows.
        // For our borderless WS_CHILD embed that border computation drifts
        // on live resize, so force the exact bounds here instead of trusting it.
        void forceNativeBounds(visage::ApplicationWindow &window, int width, int height)
        {
#ifdef _WIN32
            if (window.window() == nullptr)
                return;

            auto hwnd = static_cast<HWND>(window.window()->nativeHandle());
            if (hwnd != nullptr)
                SetWindowPos(hwnd, nullptr, 0, 0, width, height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
#endif
        }
    }

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
        forceNativeBounds(window_, width, height);
    }

    void VisageMainView::resize(int width, int height)
    {
        window_.setNativeWindowDimensions(width, height);
        layoutChildren(width, height);
        forceNativeBounds(window_, width, height);
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
