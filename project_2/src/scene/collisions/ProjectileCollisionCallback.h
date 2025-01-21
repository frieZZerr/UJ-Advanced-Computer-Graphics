#ifndef PROJECTILE_COLLISION_CALLBACK_H
#define PROJECTILE_COLLISION_CALLBACK_H

#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"

#include <osg/NodeCallback>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/observer_ptr>
#include <osg/ref_ptr>
#include <vector>

class ProjectileCollisionCallback : public osg::NodeCallback {
public:
    ProjectileCollisionCallback(osg::Group* sceneRoot, std::vector<osg::ref_ptr<Target>>& targets, Projectile* projectile)
        : _sceneRoot(sceneRoot),
          _targets(targets),
          _projectile(projectile) {};

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) override;

private:
    osg::observer_ptr<osg::Group> _sceneRoot;
    std::vector<osg::ref_ptr<Target>> _targets;
    osg::observer_ptr<Projectile> _projectile;

    void checkProjectileCollisions(osg::Vec3 projectilePosition);
};

#endif // PROJECTILE_COLLISION_CALLBACK_H
