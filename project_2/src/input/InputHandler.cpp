#include "input/InputHandler.h"
#include "scene/objects/Projectile.h"

#include <osg/Quat>
#include <osg/Vec3>
#include <iostream>

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        switch(ea.getKey()) {
        case osgGA::GUIEventAdapter::KEY_Space:
            // std::cout << "Left mouse button clicked." << std::endl;
            fireCannon();
            return true;

        case osgGA::GUIEventAdapter::KEY_A:
            // std::cout << "Key A pressed: Rotating cannon left." << std::endl;
            rotateCannon(-rotateCannonAngle);
            return true;

        case osgGA::GUIEventAdapter::KEY_D:
            // std::cout << "Key D pressed: Rotating cannon left." << std::endl;
            rotateCannon(rotateCannonAngle);
            return true;

        default:
            break;
        }

    default:
        break;
    }

    return false;
}

void InputHandler::rotateCannon(float angle) {

    osg::Quat currentRotation = _cannon->getAttitude();
    osg::Vec3 axis;
    double currentAngle;
    currentRotation.getRotate(currentAngle, axis);

    float currentAngleDegrees = osg::RadiansToDegrees(static_cast<float>(currentAngle));

    float newAngle = currentAngleDegrees + angle;

    if (newAngle < minCannonAngle) {
        newAngle = minCannonAngle;
    } else if (newAngle > maxCannonAngle) {
        newAngle = maxCannonAngle;
    }

    osg::Quat newRotation(osg::DegreesToRadians(newAngle), osg::Vec3(0.0, 1.0, 0.0));
    _cannon->setAttitude(newRotation);
}

void InputHandler::fireCannon() {
    // std::cout << "Cannon fired!" << std::endl;

    osg::Vec3 cannonPosition = _cannon->getPosition();
    osg::Quat cannonRotation = _cannon->getAttitude();

    osg::ref_ptr<osg::PositionAttitudeTransform> projectile = createProjectile();

    osg::Vec3 localDirection(1.0, 0.0, 1.0);
    osg::Vec3 worldDirection = cannonRotation * localDirection;

    osg::Vec3 startPosition = cannonPosition;
    osg::Vec3 endPosition = startPosition + worldDirection * 10.0;
    osg::ref_ptr<osg::AnimationPath> path = createProjectilePath(startPosition, endPosition, 2.0);

    projectile->setUpdateCallback(new osg::AnimationPathCallback(path));

    _sceneRoot->addChild(projectile);
}
