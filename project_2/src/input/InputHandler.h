#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "scene/objects/Cannon.h"

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>

class InputHandler : public osgGA::GUIEventHandler {
public:
    InputHandler(osg::Group* sceneRoot, Cannon* cannon, osg::PositionAttitudeTransform* target)
        : _sceneRoot(sceneRoot),
          _cannon(cannon),
          _target(target) {};

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    osg::Group* _sceneRoot;
    osg::ref_ptr<Cannon> _cannon;
    osg::ref_ptr<osg::PositionAttitudeTransform> _target;
};

#endif // INPUT_HANDLER_H
