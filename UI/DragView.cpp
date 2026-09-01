#include "DragView.h"

#include <algorithm>
#include <cmath>

namespace ui
{

    DragView::DragView()
    {
        points_ = {{0.0f, 0.5f}, {1.0f, 0.5f}};
    }

    DragView::~DragView() = default;

    void DragView::draw(visage::Canvas &canvas)
    {
        if (points_.empty())
            return;

        canvas.setColor(0xffaa88ff);
        for (size_t i = 0; i + 1 < points_.size(); ++i)
        {
            canvas.segment(points_[i].first * width(), points_[i].second * height(),
                           points_[i + 1].first * width(), points_[i + 1].second * height(), 2.0f, true);
        }

        for (const auto &point : points_)
        {
            canvas.circle(point.first * width() - kHandleRadius, point.second * height() - kHandleRadius,
                          2.0f * kHandleRadius);
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

    void DragView::mouseDown(const visage::MouseEvent &e)
    {
        draggedIndex_ = nearestPointIndex(e.position.x, e.position.y);
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

    std::pair<float, float> DragView::toNormalized(float pixelX, float pixelY) const
    {
        float x = width() > 0.0f ? std::clamp(pixelX / width(), 0.0f, 1.0f) : 0.0f;
        float y = height() > 0.0f ? std::clamp(pixelY / height(), 0.0f, 1.0f) : 0.0f;
        return {x, y};
    }

    int DragView::nearestPointIndex(float pixelX, float pixelY) const
    {
        int closestIndex = -1;
        float closestDistanceSquared = (kHandleRadius * 2.0f) * (kHandleRadius * 2.0f);

        for (size_t i = 0; i < points_.size(); ++i)
        {
            float dx = points_[i].first * width() - pixelX;
            float dy = points_[i].second * height() - pixelY;
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
