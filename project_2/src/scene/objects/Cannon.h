#ifndef CANNON_H
#define CANNON_H

#include <osg/PositionAttitudeTransform>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Vec4>

class Cannon : public osg::Referenced {
public:
    Cannon(const osg::Vec3& position = osg::Vec3(-20.0f, 0.0f, 0.0f));

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return _cannonTransform; }

    void setPosition(const osg::Vec3& position);

    // void rotate(float angle, const osg::Vec3& axis);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _cannonTransform;
    osg::ref_ptr<osg::Geode> _cannonBody;

    void createCannon(const osg::Vec3& position);
};

#endif // CANNON_H
