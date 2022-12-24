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
    
    DataPoint(int _start, int _end, float _slope, float _yIntercept)
    {
        this->start = _start;
        this->end = _end;
        this->slope = _slope;
        this->y_intercept = _yIntercept;
    }
};
