#ifndef PROJECTILE_COLLISION_CALLBACK_H
#define PROJECTILE_COLLISION_CALLBACK_H

#include <osg/NodeCallback>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/observer_ptr>

class ProjectileCollisionCallback : public osg::NodeCallback {
public:
    ProjectileCollisionCallback(osg::Group* sceneRoot, osg::PositionAttitudeTransform* target)
        : _sceneRoot(sceneRoot),
          _targetTransform(target) {};

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) override;

private:
    osg::observer_ptr<osg::Group> _sceneRoot;
    osg::observer_ptr<osg::PositionAttitudeTransform> _targetTransform;
};

#endif // PROJECTILE_COLLISION_CALLBACK_H
