#include "ControlPoint.hpp"

ControlPoint::ControlPoint()
{
}

ControlPoint::~ControlPoint()
{
}

void ControlPoint::paint(juce::Graphics &g)
{
    auto width  = getWidth();
    auto height = getHeight();
    int centerY = height / 2;
    int centreX = width / 2;
    float lineThinkness = 2.0f;
    float lineLength = 80.0f;
    int controlSize = 10;

    //g.setColour(juce::Colours::red);
    //g.fillRect(Rectangle<float>(0, centerY, width, lineThinkness));
    //g.fillRect(Rectangle<float>(centreX, 0, lineThinkness,  height));

    g.fillAll(juce::Colour::fromRGBA(255, 255, 255, 50));

    g.setColour(juce::Colours::white);
    g.fillRect(Rectangle<float>(centreX - (lineLength / 2.0f), centerY, lineLength, lineThinkness));

    auto rect = Rectangle<float>(centreX - ((float)controlSize / 2), centerY - ((float)controlSize / 2), controlSize, controlSize);

    g.fillEllipse(rect);

    auto rect2 = Rectangle<float>(centreX - (lineLength / 2), centerY - ((float)controlSize / 2), controlSize, controlSize);
    g.drawEllipse(rect2, lineThinkness);

    auto rect3 = Rectangle<float>(lineLength, centerY - ((float)controlSize / 2), controlSize, controlSize);
    g.drawEllipse(rect3, lineThinkness);
}

void ControlPoint::resized()
{
}