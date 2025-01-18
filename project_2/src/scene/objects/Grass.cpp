#include "scene/objects/Grass.h"

#include <osg/ShapeDrawable>

osg::ref_ptr<osg::Geode> createGrass() {
    osg::ref_ptr<osg::Geode> grass = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> groundShape = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0.0f, 0.0f, -0.5f), 50.0f, 50.0f, 1.0f)
    );

    groundShape->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    grass->addDrawable(groundShape);

    return grass;
}
