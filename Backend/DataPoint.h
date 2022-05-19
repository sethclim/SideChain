/*
  ==============================================================================

    DataPoint.h
    Created: 27 Dec 2021 3:47:13pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#pragma once

struct DataPoint {
    int start, end;
    float slope, y_intercept;
    
    DataPoint(int start, int end, float slope, float y_intercept)
    {
        this->start = start;
        this->end = end;
        this->slope = slope;
        this->y_intercept = y_intercept;
    }
};
