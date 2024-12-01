#include "Scene.h"
#include "objects/Grass.h"
#include "objects/Cannon.h"
#include "objects/Target.h"
#include "objects/Projectile.h"

osg::ref_ptr<osg::Group> createScene() {
    osg::ref_ptr<osg::Group> root = new osg::Group();

    // Dodanie łąki
    root->addChild(createGrass());

    // Dodanie armaty
    root->addChild(createCannon());

    // Dodanie celu
    root->addChild(createTarget());

    // Dodanie pocisku
    osg::ref_ptr<osg::PositionAttitudeTransform> projectile = createProjectile();
    osg::ref_ptr<osg::AnimationPath> path = createProjectilePath(osg::Vec3(0.0, 0.0, 1.0), osg::Vec3(5.0, 0.0, 10.0), 2.0);
    projectile->setUpdateCallback(new osg::AnimationPathCallback(path));
    root->addChild(projectile);

    return root;
}
