#include "scene/Scene.h"
#include "scene/objects/Grass.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "scene/objects/Projectile.h"

osg::ref_ptr<osg::Group> createScene() {
    osg::ref_ptr<osg::Group> root = new osg::Group();

    root->addChild(createGrass());

    root->addChild(createCannon());

    root->addChild(createTarget());

    return root;
}
