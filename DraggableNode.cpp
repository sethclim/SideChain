/*
  ==============================================================================

    DraggableNode.cpp
    Created: 16 May 2021 9:21:42pm
    Author:  Seth Climenhaga

  ==============================================================================
*/

#include "DraggableNode.h"



DraggableNode::DraggableNode(float x, float y){
    posX = x;
    posY = y;
}

DraggableNode::~DraggableNode(){
  
}


float DraggableNode::getX(){
    return posX;
}



float DraggableNode::getY(){
    return posY;
}


void DraggableNode::setX(float x){
    this->posX = x;
}



void DraggableNode::setY(float y){
    this->posY = y;
}


