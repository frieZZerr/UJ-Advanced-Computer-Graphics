#include "Target.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>

osg::ref_ptr<osg::PositionAttitudeTransform> createTarget() {
    osg::ref_ptr<osg::PositionAttitudeTransform> target = new osg::PositionAttitudeTransform();
    osg::ref_ptr<osg::Geode> targetNode = new osg::Geode();
    osg::ref_ptr<osg::ShapeDrawable> targetShape = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3(0.0f, 0.0f, 1.0f), 1.0f)
    );
    targetShape->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    targetNode->addDrawable(targetShape);
    target->addChild(targetNode);
    target->setPosition(osg::Vec3(10.0f, 0.0f, 0.0f));
    return target;
}
