#include "InputHandler.h"

#include <osg/Quat>
#include <osg/Vec3>
#include <iostream>

InputHandler::InputHandler(osg::ref_ptr<osg::PositionAttitudeTransform> cannon)
    : _cannon(cannon) {}

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        if (ea.getKey() == 'a' || ea.getKey() == 'A') {
            std::cout << "Key A pressed: Rotating cannon left." << std::endl;
            rotateCannon(-5.0);
            return true;
        } else if (ea.getKey() == 'd' || ea.getKey() == 'D') {
            std::cout << "Key D pressed: Rotating cannon right." << std::endl;
            rotateCannon(5.0);
            return true;
        }
        break;

    case osgGA::GUIEventAdapter::PUSH:
        if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) {
            std::cout << "Left mouse button clicked." << std::endl;
            fireCannon();
            return true;
        }
        break;

    default:
        break;
    }
    return false;
}

void InputHandler::rotateCannon(float angle) {
    osg::Quat rotation = _cannon->getAttitude();
    rotation *= osg::Quat(osg::DegreesToRadians(angle), osg::Vec3(0.0, 1.0, 0.0));
    _cannon->setAttitude(rotation);
}

void InputHandler::fireCannon() {
    std::cout << "Cannon fired!" << std::endl;
    // Tutaj można dodać animację pocisku.
}
