#include "scene/objects/Projectile.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>

Projectile::Projectile(const osg::Vec3& startPosition, const osg::Vec3& velocity, float duration) {
    createProjectile();
    createProjectilePath(startPosition, velocity, duration);
}

void Projectile::createProjectile() {
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), _projectileRadius * 2.0f);

    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere);

    this->_projectileBody = new osg::Geode();
    _projectileBody->addDrawable(drawable);

    this->_projectileTransform = new osg::PositionAttitudeTransform();
    _projectileTransform->addChild(_projectileBody);
}

void Projectile::createProjectilePath(const osg::Vec3& start, const osg::Vec3& velocity, float duration) {
    this->_animationPath = new osg::AnimationPath();
    _animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

    const int numSteps = 100;

    for (int i = 0; i <= numSteps; ++i) {
        float t = duration * static_cast<float>(i) / numSteps;
        float x = start.x() + velocity.x() * t;
        float y = start.y() + velocity.y() * t;
        float z = start.z() + velocity.z() * t + 0.5f * _projectileGravity * t * t;

        osg::Vec3 position(x, y, z);
        osg::Quat rotation;
        _animationPath->insert(t, osg::AnimationPath::ControlPoint(position, rotation));
    }

    _projectileTransform->setUpdateCallback(new osg::AnimationPathCallback(_animationPath));
}

void Projectile::setPosition(const osg::Vec3& position) {
    this->_projectileTransform->setPosition(position);
}
