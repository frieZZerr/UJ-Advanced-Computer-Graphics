#include "scene/objects/Cannon.h"
#include "scene/objects/Projectile.h"
#include "scene/collisions/ProjectileCollisionCallback.h"

#include <osg/Group>
#include <osg/Geode>
#include <osg/Quat>
#include <osg/ShapeDrawable>
#include <iostream>

Cannon::Cannon(const osg::Vec3& position) {
    createCannon(position);
}

void Cannon::createCannon(const osg::Vec3& position) {
    _cannonTransform = new osg::PositionAttitudeTransform();
    _cannonBody = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> cannonShape = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0.0f, 0.0f, 0.5f), 1.0f, 1.0f, 2.0f)
    );

    cannonShape->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 1.0f));
    _cannonBody->addDrawable(cannonShape);

    _cannonTransform->addChild(_cannonBody);
    _cannonTransform->setPosition(position);
}

void Cannon::setFireKeyHeld(bool keyHeld) {
    this->fireKeyHeld = keyHeld;
}

void Cannon::setFirePressStartTime(double pressStartTime) {
    this->firePressStartTime = pressStartTime;
}

void Cannon::setPosition(const osg::Vec3& position) {
    _cannonTransform->setPosition(position);
}

void Cannon::rotate(float angle, RotationAxis axis) {
    switch (axis) {
    case RotationAxis::YAW:
        yawAngle += angle;

        if (yawAngle < minYawAngle) yawAngle = minYawAngle;
        if (yawAngle > maxYawAngle) yawAngle = maxYawAngle;

        break;

    case RotationAxis::PITCH:
        pitchAngle += angle;

        if (pitchAngle < minPitchAngle) pitchAngle = minPitchAngle;
        if (pitchAngle > maxPitchAngle) pitchAngle = maxPitchAngle;

        break;

    default:
        break;
    }

    osg::Quat yawQuat(
        osg::DegreesToRadians(yawAngle),
        yAxisVector
    );

    osg::Quat pitchQuat(
        osg::DegreesToRadians(pitchAngle),
        zAxisVector
    );

    osg::Quat finalQuat = yawQuat * pitchQuat;

    getTransform()->setAttitude(finalQuat);
}

void Cannon::fire(float holdDuration, osg::Group* sceneRoot, osg::PositionAttitudeTransform* target) {
    if (holdDuration < 0.0f) {
        holdDuration = 0.0f;
    } else if (holdDuration > maxHoldTime) {
        holdDuration = maxHoldTime;
    }

    float distanceFactor = holdDuration * distancePerHold;
    
    // std::cout << "Firing cannon! Held for " << holdDuration 
    //           << " seconds. DistanceFactor=" << distanceFactor << std::endl;

    osg::Vec3 cannonPosition = getTransform()->getPosition();
    osg::Quat cannonRotation = getTransform()->getAttitude();

    osg::ref_ptr<osg::PositionAttitudeTransform> projectile = createProjectile();

    osg::Vec3 localDirection(zAxisVector);  
    osg::Vec3 worldDirection = cannonRotation * localDirection;

    float finalDistance = distancePerHold + distanceFactor;
    osg::Vec3 velocity = worldDirection * finalDistance;
    osg::ref_ptr<osg::AnimationPath> path = createProjectilePath(cannonPosition, velocity, 15.0f);

    projectile->setUpdateCallback(new osg::AnimationPathCallback(path));

    osg::ref_ptr<ProjectileCollisionCallback> collisionCb =
        new ProjectileCollisionCallback(sceneRoot, target);

    collisionCb->addNestedCallback(projectile->getUpdateCallback());
    projectile->setUpdateCallback(collisionCb);

    sceneRoot->addChild(projectile);
}
