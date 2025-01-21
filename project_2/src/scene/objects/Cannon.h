#ifndef CANNON_H
#define CANNON_H

#include "scene/objects/Target.h"

#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Vec4>

class Cannon : public osg::Referenced {
public:
    Cannon(const osg::Vec3& position = osg::Vec3(-20.0f, 0.0f, 0.0f));

    static constexpr float ROTATE_CANNON_ANGLE = 2.5f;

    enum RotationAxis {
        YAW,
        PITCH
    };

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return this->_cannonTransform; }
    bool getFireKeyHeld() const { return this->_fireKeyHeld; };
    double getFirePressStartTime() const { return this->_firePressStartTime; };

    void setFireKeyHeld(bool keyHeld);
    void setFirePressStartTime(double pressStartTime);
    void setPosition(const osg::Vec3& position);

    void rotate(float angle, RotationAxis axis);
    void fire(float holdDuration, osg::Group* sceneRoot, std::vector<osg::ref_ptr<Target>>& targets);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannonTransform;
    osg::ref_ptr<osg::Geode> _cannonBody;

    const osg::Vec3 _yAxisVector = osg::Vec3(0.0f, 1.0f, 0.0f);
    const osg::Vec3 _zAxisVector = osg::Vec3(0.0f, 0.0f, 1.0f);

    const float _maxHoldTime     = 3.0f;
    const float _distancePerHold = 7.5f;

    const float _minPitchAngle = -45.0f;
    const float _maxPitchAngle = 45.0f;
    const float _minYawAngle   = 5.0f;
    const float _maxYawAngle   = 45.0f;

    float _pitchAngle = 0.0f; // up-down
    float _yawAngle   = 0.0f; // left-right

    double _firePressStartTime = 0.0;

    bool _fireKeyHeld = false;

    void createCannon(const osg::Vec3& position);
};

#endif // CANNON_H
