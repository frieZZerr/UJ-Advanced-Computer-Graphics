#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>

class InputHandler : public osgGA::GUIEventHandler {
public:
    InputHandler(osg::Group* sceneRoot, osg::PositionAttitudeTransform* cannon)
        : _sceneRoot(sceneRoot), _cannon(cannon) {};

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    osg::Group* _sceneRoot;
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannon;

    const float rotateCannonAngle = 5.0f;
    const float minCannonAngle = -45.0f;
    const float maxCannonAngle = 45.0f;

    void rotateCannon(float angle);
    void fireCannon();
};

#endif // INPUT_HANDLER_H
