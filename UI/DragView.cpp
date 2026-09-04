#include "DragView.h"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace ui
{

    DragView::DragView()
    {
        points_ = {{0.0f, 0.5f}, {0.8f, 0.5f}};
    }

    DragView::~DragView() = default;

    void DragView::draw(visage::Canvas &canvas)
    {
        // Recessed panel look so the curve canvas reads as distinct from the
        // surrounding chrome (preset bar / division button / meters), which
        // all show the window's lighter background straight through.
        canvas.setColor(0xff101012);
        canvas.roundedRectangle(0.0f, 0.0f, width(), height(), 6.0f);
        canvas.setColor(0xff000000);
        canvas.roundedRectangleBorder(0.0f, 0.0f, width(), height(), 6.0f, 1.0f);

        if (points_.empty())
            return;

        canvas.setColor(0xffaa88ff);
        for (size_t i = 0; i + 1 < points_.size(); ++i)
        {
            auto a = pixelForNormalized(points_[i].first, points_[i].second);
            auto b = pixelForNormalized(points_[i + 1].first, points_[i + 1].second);
            canvas.segment(a.first, a.second, b.first, b.second, 2.0f, true);
        }

        for (const auto &point : points_)
        {
            auto p = pixelForNormalized(point.first, point.second);
            canvas.circle(p.first - kHandleRadius, p.second - kHandleRadius, 2.0f * kHandleRadius);
        }
    }

    void DragView::setPoints(std::vector<std::pair<float, float>> normalizedPoints)
    {
        points_ = std::move(normalizedPoints);
        redraw();
    }

    void DragView::setOnNodeDragged(std::function<void(int, float, float)> callback)
    {
        onNodeDragged_ = std::move(callback);
    }

    void DragView::setOnPointAdded(std::function<void(float, float)> callback)
    {
        pointAdded_ = std::move(callback);
    }

    void DragView::mouseDown(const visage::MouseEvent &e)
    {
        if (e.isRightButton())
        {
            auto normalized = toNormalized(e.position.x, e.position.y);
            if (pointAdded_)
                pointAdded_(normalized.first, normalized.second);
        }
        else
        {
            draggedIndex_ = nearestPointIndex(e.position.x, e.position.y);
        }
    }

    void DragView::mouseDrag(const visage::MouseEvent &e)
    {
        if (draggedIndex_ < 0 || draggedIndex_ >= (int)points_.size())
            return;

        auto normalized = toNormalized(e.position.x, e.position.y);
        points_[draggedIndex_] = normalized;
        redraw();

        if (onNodeDragged_)
            onNodeDragged_(draggedIndex_, normalized.first, normalized.second);
    }

    void DragView::mouseUp(const visage::MouseEvent &e)
    {
        draggedIndex_ = -1;
    }

    std::pair<float, float> DragView::pixelForNormalized(float normalizedX, float normalizedY) const
    {
        float usableWidth = std::max(0.0f, width() - 2.0f * kHandleRadius);
        float usableHeight = std::max(0.0f, height() - 2.0f * kHandleRadius);
        return {kHandleRadius + normalizedX * usableWidth, kHandleRadius + normalizedY * usableHeight};
    }

    std::pair<float, float> DragView::toNormalized(float pixelX, float pixelY) const
    {
        float usableWidth = width() - 2.0f * kHandleRadius;
        float usableHeight = height() - 2.0f * kHandleRadius;
        float x = usableWidth > 0.0f ? std::clamp((pixelX - kHandleRadius) / usableWidth, 0.0f, 1.0f) : 0.0f;
        float y = usableHeight > 0.0f ? std::clamp((pixelY - kHandleRadius) / usableHeight, 0.0f, 1.0f) : 0.0f;
        return {x, y};
    }

    int DragView::nearestPointIndex(float pixelX, float pixelY) const
    {
        int closestIndex = -1;
        float closestDistanceSquared = (kHandleRadius * 2.0f) * (kHandleRadius * 2.0f);

        for (size_t i = 0; i < points_.size(); ++i)
        {
            auto p = pixelForNormalized(points_[i].first, points_[i].second);
            float dx = p.first - pixelX;
            float dy = p.second - pixelY;
            float distanceSquared = dx * dx + dy * dy;
            if (distanceSquared < closestDistanceSquared)
            {
                closestDistanceSquared = distanceSquared;
                closestIndex = (int)i;
            }
        }

        return closestIndex;
    }

}
