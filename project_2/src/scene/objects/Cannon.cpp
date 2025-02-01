#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"
#include "scene/collisions/ProjectileCollisionCallback.h"
#include "libs/BalisticCurve.h"

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
    this->_helperPathGroup = new osg::Group();

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
    osg::Vec3 cannonPosition = this->getTransform()->getPosition();
    osg::Quat cannonRotation = this->getTransform()->getAttitude();

    osg::Vec3 velocity = computeVelocity(cannonRotation, holdDuration);

    osg::ref_ptr<Projectile> projectile = new Projectile(cannonPosition, velocity);
    osg::ref_ptr<osg::PositionAttitudeTransform> projectileTransform = projectile->getTransform();

    projectileTransform->setUpdateCallback(new osg::AnimationPathCallback(projectile->getAnimationPath()));

    osg::ref_ptr<ProjectileCollisionCallback> collisionCb =
        new ProjectileCollisionCallback(sceneRoot, targets, projectile);

    collisionCb->addNestedCallback(projectileTransform->getUpdateCallback());
    projectileTransform->setUpdateCallback(collisionCb);

    sceneRoot->addChild(projectileTransform);
}

void Cannon::createHelperPath(float holdDuration) {
    _cannonTransform->removeChild(_helperPathGroup);  // Remove old helper path

    this->_helperPathGroup = new osg::Group();

    osg::Vec3 cannonPosition = osg::Vec3(0.0f, 0.0f, 2.0f);  // Position is relative to the cannon
    osg::Quat cannonRotation = this->_cannonTransform->getAttitude();
    // osg::Quat cannonRotation = osg::Quat(0.0f, 0.0f, 0.0f, 0.0f);

    osg::Vec3 velocity = computeVelocity(cannonRotation, holdDuration);
    float timeToGroundHit = computeTimeToGround(cannonPosition, velocity, Projectile::getProjectileGravity());

    const int numSteps = 30;
    auto timePositionPairs = computeCurvePositionsWithTime(numSteps, cannonPosition, velocity, Projectile::getProjectileGravity(), timeToGroundHit);

    for (const auto& [t, pos] : timePositionPairs) {
        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(
            new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), 0.1f)
        );
        sphereDrawable->setColor(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));  // Yellow color

        geode->addDrawable(sphereDrawable);

        osg::ref_ptr<osg::PositionAttitudeTransform> helperTransform = new osg::PositionAttitudeTransform();
        helperTransform->setPosition(pos);
        helperTransform->addChild(geode);

        _helperPathGroup->addChild(helperTransform);
    }

    _cannonTransform->addChild(_helperPathGroup);
}

void Cannon::showHelperPath() {
    if (!_helperPathVisible) {
        // std::cout << "Showing helper path." << std::endl;
        _helperPathGroup->setNodeMask(~0x0);
        _helperPathVisible = true;
    }
}

void Cannon::hideHelperPath() {
    if (_helperPathVisible) {
        // std::cout << "Hiding helper path." << std::endl;
        _helperPathGroup->setNodeMask(0x0);
        _helperPathVisible = false;
    }
}
