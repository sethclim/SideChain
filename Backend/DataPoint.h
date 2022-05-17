/*
  ==============================================================================

    DataPoint.h
    Created: 27 Dec 2021 3:47:13pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

struct DataPoint {
    float start, end, slope, yintercept;
    
    DataPoint(float start, float end, float slope, float yintercept)
    {
        this->start = start;
        this->end = end;
        this->slope = slope;
        this->yintercept = yintercept;
    }
};
