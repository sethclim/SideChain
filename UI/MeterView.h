#pragma once

#include <visage/graphics.h>
#include <visage/ui.h>

#include <functional>

namespace ui
{

    // Vertical level meter. Polls a value supplier (expected to return a dB
    // value, roughly -60 to 6, matching Source/UI/Meters/Meters.h's
    // VerticalMeter) at a fixed rate and redraws - no JUCE, no notion of
    // which channel/processor it's reading from. The host supplies the
    // function that reaches into the audio processor's atomic RMS value.
    class MeterView : public visage::Frame, public visage::EventTimer
    {
    public:
        static constexpr float kMinDb = -60.0f;
        static constexpr float kMaxDb = 6.0f;

        MeterView();
        ~MeterView() override;

        void setValueSupplier(std::function<float()> valueSupplier);

        void draw(visage::Canvas &canvas) override;
        void timerCallback() override;

    private:
        std::function<float()> valueSupplier_;
    };

}
