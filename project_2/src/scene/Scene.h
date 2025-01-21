#ifndef SCENE_H
#define SCENE_H

#include "scene/objects/Grass.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"
#include "scene/objects/Grass.h"

#include <osg/Group>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <vector>

class Scene : public osg::Referenced {
public:
    Scene();
    osg::ref_ptr<osg::Group> getRoot() const { return _root; }

    osg::ref_ptr<Grass> getGrass() const { return _grass; }
    osg::ref_ptr<Cannon> getCannon() const { return _cannon; }
    std::vector<osg::ref_ptr<Target>>& getTargets() { return _targets; }

private:
    osg::ref_ptr<osg::Group> _root;
    osg::ref_ptr<Grass> _grass;
    osg::ref_ptr<Cannon> _cannon;
    std::vector<osg::ref_ptr<Target>> _targets;

    int _numTargets = 5;

    void buildScene();
};

#endif // SCENE_H
