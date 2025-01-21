#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"
#include "scene/collisions/ProjectileCollisionCallback.h"

#include <osg/Group>
#include <osg/Geode>
#include <osg/Quat>
#include <osg/ShapeDrawable>
#include <iostream>
#include <vector>

Cannon::Cannon(const osg::Vec3& position) {
    createCannon(position);
}

void Cannon::createCannon(const osg::Vec3& position) {
    this->_cannonTransform = new osg::PositionAttitudeTransform();
    this->_cannonBody = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> cannonShape = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0.0f, 0.0f, 0.5f), 1.0f, 1.0f, 2.0f)
    );

    cannonShape->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 1.0f));
    _cannonBody->addDrawable(cannonShape);

    _cannonTransform->addChild(_cannonBody);
    _cannonTransform->setPosition(position);
}

void Cannon::setFireKeyHeld(bool keyHeld) {
    this->_fireKeyHeld = keyHeld;
}

void Cannon::setFirePressStartTime(double pressStartTime) {
    this->_firePressStartTime = pressStartTime;
}

void Cannon::setPosition(const osg::Vec3& position) {
    this->_cannonTransform->setPosition(position);
}

void Cannon::rotate(float angle, RotationAxis axis) {
    switch (axis) {
    case RotationAxis::YAW:
        _yawAngle += angle;

        if (_yawAngle < _minYawAngle) _yawAngle = _minYawAngle;
        if (_yawAngle > _maxYawAngle) _yawAngle = _maxYawAngle;

        break;

    case RotationAxis::PITCH:
        _pitchAngle += angle;

        if (_pitchAngle < _minPitchAngle) _pitchAngle = _minPitchAngle;
        if (_pitchAngle > _maxPitchAngle) _pitchAngle = _maxPitchAngle;

        break;

    default:
        break;
    }

    osg::Quat yawQuat(
        osg::DegreesToRadians(_yawAngle),
        _yAxisVector
    );

    osg::Quat pitchQuat(
        osg::DegreesToRadians(_pitchAngle),
        _zAxisVector
    );

    osg::Quat finalQuat = yawQuat * pitchQuat;

    this->getTransform()->setAttitude(finalQuat);
}

void Cannon::fire(float holdDuration, osg::Group* sceneRoot, std::vector<osg::ref_ptr<Target>>& targets) {
    if (holdDuration < 0.0f) {
        holdDuration = 0.0f;
    } else if (holdDuration > _maxHoldTime) {
        holdDuration = _maxHoldTime;
    }

    float distanceFactor = holdDuration * _distancePerHold;
    
    // std::cout << "Firing cannon! Held for " << holdDuration 
    //           << " seconds. DistanceFactor=" << distanceFactor << std::endl;

    osg::Vec3 cannonPosition = this->getTransform()->getPosition();
    osg::Quat cannonRotation = this->getTransform()->getAttitude();

    osg::Vec3 localDirection(_zAxisVector);  
    osg::Vec3 worldDirection = cannonRotation * localDirection;

    float finalDistance = _distancePerHold + distanceFactor;
    osg::Vec3 velocity = worldDirection * finalDistance;

    osg::ref_ptr<Projectile> projectile = new Projectile(cannonPosition, velocity, 15.0f);
    osg::ref_ptr<osg::PositionAttitudeTransform> projectileTransform = projectile->getTransform();

    projectileTransform->setUpdateCallback(new osg::AnimationPathCallback(projectile->getAnimationPath()));

    osg::ref_ptr<ProjectileCollisionCallback> collisionCb =
        new ProjectileCollisionCallback(sceneRoot, targets, projectile);

    collisionCb->addNestedCallback(projectileTransform->getUpdateCallback());
    projectileTransform->setUpdateCallback(collisionCb);

    sceneRoot->addChild(projectileTransform);
}
