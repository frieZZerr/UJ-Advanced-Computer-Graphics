#include "scene/objects/Projectile.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>

osg::ref_ptr<osg::PositionAttitudeTransform> createProjectile() {
    osg::ref_ptr<osg::Sphere> sphere = new osg::Sphere(osg::Vec3(0.0f, 0.0f, 0.0f), PROJECTILE_DIAMETER);

    osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(sphere);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(drawable);

    osg::ref_ptr<osg::PositionAttitudeTransform> projectileTransform = new osg::PositionAttitudeTransform();
    projectileTransform->addChild(geode);

    return projectileTransform;
}

osg::ref_ptr<osg::AnimationPath> createProjectilePath(const osg::Vec3& start, const osg::Vec3& velocity, float duration) {
    osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath();
    animationPath->setLoopMode(osg::AnimationPath::NO_LOOPING);

    const float gravity = -9.8f;
    const int numSteps = 100;

    for (int i = 0; i <= numSteps; ++i) {
        float t = duration * static_cast<float>(i) / numSteps;
        float x = start.x() + velocity.x() * t;
        float y = start.y() + velocity.y() * t;
        float z = start.z() + velocity.z() * t + 0.5f * gravity * t * t;

        osg::Vec3 position(x, y, z);
        osg::Quat rotation;
        animationPath->insert(t, osg::AnimationPath::ControlPoint(position, rotation));
    }

    return animationPath;
}
