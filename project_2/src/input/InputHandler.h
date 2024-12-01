#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <osgGA/GUIEventHandler>
#include <osg/PositionAttitudeTransform>

class InputHandler : public osgGA::GUIEventHandler {
public:
    InputHandler(osg::ref_ptr<osg::PositionAttitudeTransform> cannon);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannon;

    void rotateCannon(float angle);
    void fireCannon();
};

#endif // INPUT_HANDLER_H
