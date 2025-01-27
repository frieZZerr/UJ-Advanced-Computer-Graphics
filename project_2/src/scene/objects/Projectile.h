#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>
#include <osg/Referenced>
#include <osg/Vec3>
#include <osg/Geode>
#include <osg/ref_ptr>

class Projectile : public osg::Referenced {
public:
    Projectile(const osg::Vec3& startPosition, const osg::Vec3& velocity);

    static float getProjectileGravity() { return _projectileGravity; }

    osg::ref_ptr<osg::PositionAttitudeTransform> getTransform() const { return _projectileTransform; }
    osg::ref_ptr<osg::AnimationPath> getAnimationPath() const { return _animationPath; }
    osg::Vec3 getPosition() const { return _projectileTransform->getPosition(); }
    float getProjectileRadius() const { return _projectileRadius; }

    void setPosition(const osg::Vec3& position);

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> _projectileTransform;
    osg::ref_ptr<osg::Geode> _projectileBody;
    osg::ref_ptr<osg::AnimationPath> _animationPath;

    static constexpr float _projectileGravity = -9.8f;

    const float _projectileRadius  = 0.25f;

    void createProjectile();
    void createProjectilePath(const osg::Vec3& start, const osg::Vec3& velocity);
};

#endif // PROJECTILE_H
