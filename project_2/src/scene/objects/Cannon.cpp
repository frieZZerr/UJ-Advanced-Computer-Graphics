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

void Cannon::createHelperPath(float duration) {
    _cannonTransform->removeChild(_helperPathGroup);  // Remove old helper path

    this->_helperPathGroup = new osg::Group();

    osg::Vec3 start    = osg::Vec3(0.0f, 0.0f, 2.0f);  // Position is relative to the cannon
    osg::Quat rotation = this->_cannonTransform->getAttitude();
    // osg::Quat rotation = osg::Quat(0.0f, 0.0f, 0.0f, 0.0f);

    float distanceFactor = duration * _distancePerHold;

    osg::Vec3 localDirection(_zAxisVector);
    osg::Vec3 worldDirection = rotation * localDirection;

    float finalDistance = _distancePerHold + distanceFactor;
    osg::Vec3 velocity = worldDirection * finalDistance;

    const int numSteps = 20;
    float x, y, z;
    float timeToGroundHit = computeTimeToGround(start, velocity, Projectile::getProjectileGravity());
    for (int i = 0; i <= numSteps; ++i) {
        float t = timeToGroundHit * static_cast<float>(i) / numSteps;
        float x = start.x() + velocity.x() * t;
        float y = start.y() + velocity.y() * t;
        float z = start.z() + velocity.z() * t + 0.5f * Projectile::getProjectileGravity() * t * t;

        osg::Vec3 position(x, y, z);

        osg::ref_ptr<osg::Geode> geode = new osg::Geode();
        osg::ref_ptr<osg::ShapeDrawable> sphereDrawable = new osg::ShapeDrawable(
            new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 0.1f)
        );
        sphereDrawable->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));

        geode->addDrawable(sphereDrawable);

        osg::ref_ptr<osg::PositionAttitudeTransform> helperTransform = new osg::PositionAttitudeTransform();
        helperTransform->setPosition(position);
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

float Cannon::computeTimeToGround(const osg::Vec3& start, const osg::Vec3& velocity, float gravity)
{
    float z0 = start.z();
    float vz = velocity.z();
    float a = 0.5f * gravity;
    float b = vz;
    float c = z0;

    float discriminant = b*b - 4.0f*a*c;
    if (discriminant < 0.0f) {
        return -1.0f;
    }

    float sqrtDisc = std::sqrt(discriminant);

    float t1 = (-b + sqrtDisc) / (2.0f * a);
    float t2 = (-b - sqrtDisc) / (2.0f * a);

    float tPos = -1.0f;
    if (t1 >= 0.0f && t2 >= 0.0f) {
        tPos = std::min(t1, t2);
    } else if (t1 >= 0.0f) {
        tPos = t1;
    } else if (t2 >= 0.0f) {
        tPos = t2;
    }

    return tPos;
}
