#include "scene/Scene.h"
#include "scene/objects/Cannon.h"

Scene::Scene() {
    _root = new osg::Group();
    buildScene();
}

void Scene::buildScene() {
    _root->addChild(createGrass());

    _cannon = new Cannon();
    _root->addChild(_cannon->getTransform());

    _target = createTarget();
    _root->addChild(_target);
}
