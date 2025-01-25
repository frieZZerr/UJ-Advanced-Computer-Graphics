#include "input/InputHandler.h"
#include "scene/objects/Cannon.h"

#include <iostream>

bool InputHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
    float holdDuration;

    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::KEYDOWN:
        switch(ea.getKey()) {
        case osgGA::GUIEventAdapter::KEY_Space:
            // std::cout << "Key Space pressed: Charging cannon." << std::endl;
            if (!_cannon->getFireKeyHeld()) {
                _cannon->setFireKeyHeld(true);
                _cannon->setFirePressStartTime(ea.getTime());
                _cannon->showHelperPath();
            }

            holdDuration = ea.getTime() - _cannon->getFirePressStartTime();
            holdDuration = clampHoldDuration(holdDuration, _cannon->getMaxHoldDuration());
            _cannon->createHelperPath(holdDuration);

            return true;

        case osgGA::GUIEventAdapter::KEY_W:
            // std::cout << "Key A pressed: Rotating cannon up." << std::endl;
            _cannon->rotate(-Cannon::ROTATE_CANNON_ANGLE, Cannon::RotationAxis::YAW);
            return true;

        case osgGA::GUIEventAdapter::KEY_S:
            // std::cout << "Key D pressed: Rotating cannon down." << std::endl;
            _cannon->rotate(Cannon::ROTATE_CANNON_ANGLE, Cannon::RotationAxis::YAW);
            return true;

        case osgGA::GUIEventAdapter::KEY_D:
            // std::cout << "Key A pressed: Rotating cannon right." << std::endl;
            _cannon->rotate(-Cannon::ROTATE_CANNON_ANGLE, Cannon::RotationAxis::PITCH);
            return true;

        case osgGA::GUIEventAdapter::KEY_A:
            // std::cout << "Key D pressed: Rotating cannon left." << std::endl;
            _cannon->rotate(Cannon::ROTATE_CANNON_ANGLE, Cannon::RotationAxis::PITCH);
            return true;

        default:
            break;
        }

    case osgGA::GUIEventAdapter::KEYUP:
        if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Space && _cannon->getFireKeyHeld()) {
            // std::cout << "Key Space released: Firing cannon." << std::endl;
            _cannon->setFireKeyHeld(false);
            _cannon->hideHelperPath();
            holdDuration = ea.getTime() - _cannon->getFirePressStartTime();
            holdDuration = clampHoldDuration(holdDuration, _cannon->getMaxHoldDuration());

            _cannon->fire(holdDuration, _sceneRoot, _targets);

            return true;
        }

    default:
        break;
    }

    return false;
}

float InputHandler::clampHoldDuration(float duration, float maxHoldTime) {
    if (duration < 0.0f)        { return 0.0f; }
    if (duration > maxHoldTime) { return maxHoldTime; }
    return duration;
}
