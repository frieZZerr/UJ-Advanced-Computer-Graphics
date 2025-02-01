#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>
#include <vector>

class InputHandler : public osgGA::GUIEventHandler {
public:
    InputHandler(osg::Group* sceneRoot, Cannon* cannon, std::vector<osg::ref_ptr<Target>>& targets)
        : _sceneRoot(sceneRoot),
          _cannon(cannon),
          _targets(targets) {};

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    osg::Group* _sceneRoot;
    osg::ref_ptr<Cannon> _cannon;
    std::vector<osg::ref_ptr<Target>> _targets;

    float clampHoldDuration(float holdDuration, float maxHoldTime);
};

#endif // INPUT_HANDLER_H
