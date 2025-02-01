#ifndef TARGET_H
#define TARGET_H

#include "scene/objects/Grass.h"

#include <osg/PositionAttitudeTransform>
#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/ref_ptr>

class Target : public osg::Referenced {
public:
    Target(Grass* grass);

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return _targetTransform; }

    osg::Vec3 getPosition() const { return _targetTransform->getPosition(); }
    float getTargetRadius() const { return _target_radius; }

    void setPosition(const osg::Vec3& position);
    void setHit(bool isHit);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _targetTransform;
    osg::ref_ptr<osg::Geode> _targetBody;
    osg::ref_ptr<Grass> _grass;

    const float _target_radius = 0.5f;

    bool _hit = false;

    float _targetSpawnRangeX;
    float _targetSpawnRangeZ;

    osg::Vec3 generateRandomPosition() const;
    void createTarget();
};

#endif // TARGET_H
