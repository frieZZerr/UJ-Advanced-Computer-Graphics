#include "input/InputHandler.h"
#include "scene/objects/Projectile.h"
#include "scene/collisions/ProjectileCollisionCallback.h"

#include <osg/Quat>
#include <osg/Vec3>
#include <iostream>

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        switch(ea.getKey()) {
        case osgGA::GUIEventAdapter::KEY_Space:
            // std::cout << "Key Space pressed: Charging cannon." << std::endl;
            if (!spaceKeyHeld) {
                spaceKeyHeld = true;
                spacePressStartTime = ea.getTime();
            }
            return true;

        case osgGA::GUIEventAdapter::KEY_W:
            // std::cout << "Key A pressed: Rotating cannon up." << std::endl;
            rotateCannonYaw(-rotateCannonAngle);
            return true;

        case osgGA::GUIEventAdapter::KEY_S:
            // std::cout << "Key D pressed: Rotating cannon down." << std::endl;
            rotateCannonYaw(rotateCannonAngle);
            return true;

        case osgGA::GUIEventAdapter::KEY_D:
            // std::cout << "Key A pressed: Rotating cannon right." << std::endl;
            rotateCannonPitch(-rotateCannonAngle);
            return true;

        case osgGA::GUIEventAdapter::KEY_A:
            // std::cout << "Key D pressed: Rotating cannon left." << std::endl;
            rotateCannonPitch(rotateCannonAngle);
            return true;

        default:
            break;
        }

    case osgGA::GUIEventAdapter::KEYUP:
        if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Space && spaceKeyHeld) {
            // std::cout << "Key Space released: Firing cannon." << std::endl;
            spaceKeyHeld = false;
            double holdDuration = ea.getTime() - spacePressStartTime;

            fireCannon(static_cast<float>(holdDuration));
            return true;
        }

    default:
        break;
    }

    return false;
}

void InputHandler::fireCannon(float holdDuration) {
    if (holdDuration < 0.0f) {
        holdDuration = 0.0f;
    } else if (holdDuration > maxHoldTime) {
        holdDuration = maxHoldTime;
    }

    float distanceFactor = holdDuration * distancePerHold;
    
    // std::cout << "Firing cannon! Held for " << holdDuration 
    //           << " seconds. DistanceFactor=" << distanceFactor << std::endl;

    osg::Vec3 cannonPosition = _cannon->getPosition();
    osg::Quat cannonRotation = _cannon->getAttitude();

    osg::ref_ptr<osg::PositionAttitudeTransform> projectile = createProjectile();

    osg::Vec3 localDirection(zAxisVector);  
    osg::Vec3 worldDirection = cannonRotation * localDirection;

    float finalDistance = distancePerHold + distanceFactor;

    osg::Vec3 velocity = worldDirection * finalDistance;

    osg::ref_ptr<osg::AnimationPath> path = createProjectilePath(cannonPosition, velocity, 15.0f);

    projectile->setUpdateCallback(new osg::AnimationPathCallback(path));

    osg::ref_ptr<ProjectileCollisionCallback> collisionCb =
        new ProjectileCollisionCallback(_sceneRoot, _target);

    collisionCb->addNestedCallback(projectile->getUpdateCallback());
    projectile->setUpdateCallback(collisionCb);

    _sceneRoot->addChild(projectile);
}

void InputHandler::updateCannonRotation() {
    osg::Quat yawQuat(
        osg::DegreesToRadians(yawAngle),
        yAxisVector
    );

    osg::Quat pitchQuat(
        osg::DegreesToRadians(pitchAngle),
        zAxisVector
    );

    osg::Quat finalQuat = yawQuat * pitchQuat;

    _cannon->setAttitude(finalQuat);
}

void InputHandler::rotateCannonPitch(float delta) {
    pitchAngle += delta;

    if (pitchAngle < minPitchAngle) pitchAngle = minPitchAngle;
    if (pitchAngle > maxPitchAngle) pitchAngle = maxPitchAngle;

    updateCannonRotation();
}

void InputHandler::rotateCannonYaw(float delta) {
    yawAngle += delta;

    if (yawAngle < minYawAngle) yawAngle = minYawAngle;
    if (yawAngle > maxYawAngle) yawAngle = maxYawAngle;

    updateCannonRotation();
}
