//
// Created by Seth Climenhaga on 2022-05-20.
//

#pragma once
#include <JuceHeader.h>
#include "../../Backend/CurveManager.h"

class DraggableNodeEditor;

class DragArea : public juce::Component, ValueTree::Listener
{

public:
    DragArea(const juce::AudioProcessorValueTreeState& tree, CurveManager &dynCurM);
    ~DragArea() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

    void reDraw();

    void mouseDown(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    bool keyPressed(const KeyPress &key) override;

    juce::Point<float> scaleToCoord(juce::Point<float> position);

    void SetAddMode(bool mode);
    bool GetAddMode();

    /** This method is called when a child sub-tree is added.
        Note that when you register a listener to a tree, it will receive this callback for
        child changes in both that tree and any of its children, (recursively, at any depth).
        If your tree has sub-trees but you only want to know about changes to the top level tree,
        just check the parentTree parameter to make sure it's the one that you're interested in.
    */
    void valueTreeChildAdded(ValueTree &parentTree,
                             ValueTree &childWhichHasBeenAdded) override;

    /** This method is called when a child sub-tree is removed.

        Note that when you register a listener to a tree, it will receive this callback for
        child changes in both that tree and any of its children, (recursively, at any depth).
        If your tree has sub-trees but you only want to know about changes to the top level tree,
        just check the parentTree parameter to make sure it's the one that you're interested in.
    */
    void valueTreeChildRemoved(ValueTree &parentTree,
                               ValueTree &childWhichHasBeenRemoved,
                               int indexFromWhichChildWasRemoved) override;

    /** This method is called when a tree's children have been re-shuffled.

        Note that when you register a listener to a tree, it will receive this callback for
        child changes in both that tree and any of its children, (recursively, at any depth).
        If your tree has sub-trees but you only want to know about changes to the top level tree,
        just check the parameter to make sure it's the tree that you're interested in.
    */
    void valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved,
                                    int oldIndex, int newIndex) override;

    /** This method is called when a tree has been added or removed from a parent.

        This callback happens when the tree to which the listener was registered is added or
        removed from a parent. Unlike the other callbacks, it applies only to the tree to which
        the listener is registered, and not to any of its children.
    */
    void valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) override;

    /** This method is called when a tree is made to point to a different internal shared object.
        When operator= is used to make a ValueTree refer to a different object, this callback
        will be made.
    */
    void valueTreeRedirected(ValueTree &treeWhichHasBeenChanged) override;

private:
    const juce::AudioProcessorValueTreeState& nodes;
    CurveManager &curveManager;
    int selectedNodeId = -1;
    bool addMode = false;
};
