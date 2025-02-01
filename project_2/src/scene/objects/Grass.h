#ifndef GRASS_H
#define GRASS_H

#include <osg/PositionAttitudeTransform>
#include <osg/Geode>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Vec4>

class Grass : public osg::Referenced {
public:
    Grass(const osg::Vec3& position = osg::Vec3(0.0f, 0.0f, -0.5f), 
          float width = 50.0f, 
          float height = 50.0f, 
          float thickness = 1.0f);

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return this->_grassTransform; }
    float getWidth() const { return this->_width; }
    float getHeight() const { return this->_height; }
    float getThickness() const { return this->_thickness; }
    float getMargin() const { return this->_margin; }

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _grassTransform;
    osg::ref_ptr<osg::Geode> _grassBody;

    float _width     = 50.0f;
    float _height    = 50.0f;
    float _thickness = 1.0f;
    float _margin    = 5.0;

    void createGrass(const osg::Vec3& position);
};

#endif // GRASS_H
