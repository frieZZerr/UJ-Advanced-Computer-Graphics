#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <osg/PositionAttitudeTransform>
#include <osg/AnimationPath>

static const float PROJECTILE_RADIUS = 0.25f;

osg::ref_ptr<osg::PositionAttitudeTransform> createProjectile();
osg::ref_ptr<osg::AnimationPath> createProjectilePath(const osg::Vec3& start, const osg::Vec3& velocity, float duration);

#endif // PROJECTILE_H
