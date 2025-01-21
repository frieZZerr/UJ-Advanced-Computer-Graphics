#include "scene/objects/Grass.h"

#include <osg/ShapeDrawable>

Grass::Grass(const osg::Vec3& position, float width, float height, float thickness) {
    this->_width     = width;
    this->_height    = height;
    this->_thickness = thickness;
    this->_margin    = width / 10.0f;

    createGrass(position);
}

void Grass::createGrass(const osg::Vec3& position) {
    _grassTransform = new osg::PositionAttitudeTransform();
    _grassBody = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> groundShape = new osg::ShapeDrawable(
        new osg::Box(osg::Vec3(0.0f, 0.0f, 0.0f), this->_width, this->_height, this->_thickness)
    );

    groundShape->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green color
    _grassBody->addDrawable(groundShape);

    _grassTransform->addChild(_grassBody);
    _grassTransform->setPosition(position);
}
