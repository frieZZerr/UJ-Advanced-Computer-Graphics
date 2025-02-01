#include "scene/objects/Projectile.h"
#include "libs/BalisticCurve.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>

#include "iostream"

Projectile::Projectile(const osg::Vec3& startPosition, const osg::Vec3& velocity) {
    createProjectile();
    createProjectilePath(startPosition, velocity);
}

void Projectile::createProjectile() {
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), _projectileRadius * 2.0f);

    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere);

    this->_projectileBody = new osg::Geode();
    _projectileBody->addDrawable(drawable);

    this->_projectileTransform = new osg::PositionAttitudeTransform();
    _projectileTransform->addChild(_projectileBody);
}

void Projectile::createProjectilePath(const osg::Vec3& start, const osg::Vec3& velocity) {
    this->_animationPath = new osg::AnimationPath();
    _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

    float timeToGroundHit = computeTimeToGround(start, velocity, _projectileGravity);

    const int numSteps = 100;
    auto timePositionPairs = computeCurvePositionsWithTime(numSteps, start, velocity, _projectileGravity);

    for (const auto& [t, pos] : timePositionPairs) {
        _animationPath->insert(t, osg::AnimationPath::ControlPoint(pos));
    }

    _projectileTransform->setUpdateCallback(new osg::AnimationPathCallback(_animationPath));
}

void Projectile::setPosition(const osg::Vec3& position) {
    this->_projectileTransform->setPosition(position);
}
