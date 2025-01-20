#include "scene/objects/Cannon.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>

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

void Cannon::setPosition(const osg::Vec3& position) {
    _cannonTransform->setPosition(position);
}

// void Cannon::rotate(float angle, const osg::Vec3& axis) {
//     osg::Quat currentRotation = _cannonTransform->getAttitude();
//     osg::Quat rotationIncrement(osg::DegreesToRadians(angle), axis);
//     _cannonTransform->setAttitude(currentRotation * rotationIncrement);
// }
