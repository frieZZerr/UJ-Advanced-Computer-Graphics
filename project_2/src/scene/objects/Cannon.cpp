#include "scene/objects/Cannon.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>

osg::ref_ptr<osg::PositionAttitudeTransform> createCannon() {
    osg::ref_ptr<osg::PositionAttitudeTransform> cannon = new osg::PositionAttitudeTransform();

    osg::ref_ptr<osg::Geode> cannonBody = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> cannonShape = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0.0f, 0.0f, 0.5f), 1.0f, 1.0f, 2.0f)
    );

    cannonShape->setColor(osg::Vec4(0.3f, 0.3f, 0.3f, 1.0f));
    cannonBody->addDrawable(cannonShape);

    cannon->addChild(cannonBody);
    cannon->setPosition(osg::Vec3(-20.0f, 0.0f, 0.0f));

    return cannon;
}
