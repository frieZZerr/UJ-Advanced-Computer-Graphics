#ifndef SCENE_H
#define SCENE_H

#include "scene/objects/Grass.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"

#include <osg/ref_ptr>
#include <osg/Group>

class Scene {
public:
    Scene();
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

    osg::ref_ptr<Cannon> getCannon() const { return _cannon; }
    osg::ref_ptr<osg::PositionAttitudeTransform> getTarget() const { return _target; }

private:
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<Cannon> _cannon;
    osg::ref_ptr<osg::PositionAttitudeTransform> _target;

    void buildScene();
};

#endif // SCENE_H
