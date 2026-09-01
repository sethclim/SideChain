#pragma once

#include <visage/graphics.h>
#include <visage/ui.h>

#include <functional>
#include <utility>
#include <vector>

namespace ui
{

    // Draws a polyline through normalized (0-1, 0-1) points and lets the user
    // drag them. Plain data in, plain callback out - no JUCE, no notion of
    // CurveManager's node ids. The host (PluginEditor) maps the reported index
    // back to a real node id and owns the source of truth for the points.
    class DragView : public visage::Frame
    {
    public:
        static constexpr float kHandleRadius = 6.0f;

        DragView();
        ~DragView() override;

        void draw(visage::Canvas &canvas) override;

        void setPoints(std::vector<std::pair<float, float>> normalizedPoints);
        void setOnNodeDragged(std::function<void(int index, float x, float y)> callback);

        // Fired on a right-click that isn't near an existing node.
        void setOnPointAdded(std::function<void(float x, float y)> callback);

        void mouseDown(const visage::MouseEvent &e) override;
        void mouseDrag(const visage::MouseEvent &e) override;
        void mouseUp(const visage::MouseEvent &e) override;

    private:
        std::pair<float, float> toNormalized(float pixelX, float pixelY) const;
        int nearestPointIndex(float pixelX, float pixelY) const;

        std::vector<std::pair<float, float>> points_;
        std::function<void(int, float, float)> onNodeDragged_;
        std::function<void(float, float)> pointAdded_;
        int draggedIndex_ = -1;
    };

}
