#include "scene/Scene.h"
#include "scene/objects/Grass.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
// #include "ui/UI.h"

#include <osg/Vec3>

Scene::Scene() {
    this->_root = new osg::Group();
    buildScene();
}

void Scene::buildScene() {
    addGrass();

    addCannon();

    addTargets();

    // _ui = new UI("Hello OSG!", 50, 700);
    // _root->addChild(_ui->getUIElement());
}

void Scene::addGrass() {
    this->_grass = new Grass();
    _root->addChild(_grass->getTransform());
}

void Scene::addCannon() {
    osg::Vec3 cannonPosition = osg::Vec3(-(_grass->getHeight() / 2.0f - _grass->getMargin()), 0.0f, 0.0f);
    this->_cannon = new Cannon(cannonPosition);
    _root->addChild(_cannon->getTransform());
}

void Scene::addTargets() {
    for (int i = 0; i < _numTargets; ++i) {
        osg::ref_ptr<Target> target;
        bool validPosition = false;

        while (!validPosition) {
            target = new Target(_grass);

            osg::Vec3 newPosition = target->getTransform()->getPosition();
            validPosition = true;

            for (const auto& existingTarget : _targets) {
                osg::Vec3 existingPosition = existingTarget->getTransform()->getPosition();
                float distance = (newPosition - existingPosition).length();

                if (distance < _minTargetDistance) {
                    validPosition = false;
                    break;
                }
            }
        }

        this->_targets.push_back(target);
        _root->addChild(target->getTransform());
    }
}
