#include "scene/Scene.h"
#include "scene/objects/Grass.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"

#include <osg/Vec3>

Scene::Scene() {
    _root = new osg::Group();
    buildScene();
}

void Scene::buildScene() {
    _grass = new Grass();
    _root->addChild(_grass->getTransform());

    osg::Vec3 cannonPosition = osg::Vec3(-(_grass->getHeight() / 2.0f - _grass->getMargin()), 0.0f, 0.0f);
    _cannon = new Cannon(cannonPosition);
    _root->addChild(_cannon->getTransform());

    for (int i = 0; i < _numTargets; ++i) {
        osg::ref_ptr<Target> target = new Target(_grass);
        _targets.push_back(target);
        _root->addChild(target->getTransform());
    }
}
