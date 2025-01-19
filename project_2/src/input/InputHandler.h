#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osgGA/GUIEventHandler>

class InputHandler : public osgGA::GUIEventHandler {
public:
    InputHandler(osg::Group* sceneRoot, osg::PositionAttitudeTransform* cannon, osg::PositionAttitudeTransform* target)
        : _sceneRoot(sceneRoot),
          _cannon(cannon),
          _target(target) {};

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    osg::Group* _sceneRoot;
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannon;
    osg::ref_ptr<osg::PositionAttitudeTransform> _target;

    const osg::Vec3 yAxisVector = osg::Vec3(0.0f, 1.0f, 0.0f);
    const osg::Vec3 zAxisVector = osg::Vec3(0.0f, 0.0f, 1.0f);

    const float rotateCannonAngle = 5.0f;

    const float minPitchAngle = -45.0f;
    const float maxPitchAngle = 45.0f;
    const float minYawAngle   = 5.0f;
    const float maxYawAngle   = 45.0f;

    const float maxHoldTime     = 3.0f;
    const float distancePerHold = 7.5f;

    float pitchAngle = 0.0f; // up-down
    float yawAngle   = 0.0f; // left-right

    double spacePressStartTime = 0.0;
    bool spaceKeyHeld = false;

    void fireCannon(float holdDuration);
    void rotateCannonPitch(float angle);
    void rotateCannonYaw(float angle);
    void updateCannonRotation();
};

#endif // INPUT_HANDLER_H
