#ifndef CANNON_H
#define CANNON_H

#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Vec4>

class Cannon : public osg::Referenced {
public:
    Cannon(const osg::Vec3& position = osg::Vec3(-20.0f, 0.0f, 0.0f));

    enum RotationAxis {
        YAW,
        PITCH
    };

    static constexpr float ROTATE_CANNON_ANGLE = 5.0f;

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return _cannonTransform; }
    bool getFireKeyHeld() { return fireKeyHeld; };
    double getFirePressStartTime() { return firePressStartTime; };

    void setFireKeyHeld(bool keyHeld);
    void setFirePressStartTime(double pressStartTime);
    void setPosition(const osg::Vec3& position);

    void rotate(float angle, RotationAxis axis);
    void fire(float holdDuration, osg::Group* sceneRoot, osg::PositionAttitudeTransform* target);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannonTransform;
    osg::ref_ptr<osg::Geode> _cannonBody;

    const osg::Vec3 yAxisVector = osg::Vec3(0.0f, 1.0f, 0.0f);
    const osg::Vec3 zAxisVector = osg::Vec3(0.0f, 0.0f, 1.0f);

    const float maxHoldTime     = 3.0f;
    const float distancePerHold = 7.5f;

    const float minPitchAngle = -45.0f;
    const float maxPitchAngle = 45.0f;
    const float minYawAngle   = 5.0f;
    const float maxYawAngle   = 45.0f;

    float pitchAngle = 0.0f; // up-down
    float yawAngle   = 0.0f; // left-right

    double firePressStartTime = 0.0;

    bool fireKeyHeld = false;

    void createCannon(const osg::Vec3& position);
};

#endif // CANNON_H
