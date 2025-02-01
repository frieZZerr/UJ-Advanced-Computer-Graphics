#include "scene/collisions/ProjectileCollisionCallback.h"
#include "scene/objects/Projectile.h"
#include "scene/objects/Target.h"

#include <iostream>

void ProjectileCollisionCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
    traverse(node, nv);

    osg::PositionAttitudeTransform* projectilePAT = dynamic_cast<osg::PositionAttitudeTransform*>(node);
    if (!projectilePAT) return;

    osg::Vec3 projectilePos = projectilePAT->getPosition();

    if (projectilePos.z() <= -_projectile->getProjectileRadius()) {
        // std::cout << "Projectile hit the ground!" << std::endl;

        projectilePos.z() = -_projectile->getProjectileRadius();
        projectilePAT->setPosition(projectilePos);
        projectilePAT->setUpdateCallback(nullptr);

        return;
    }

    checkProjectileCollisions(projectilePos);

    return;
}

void ProjectileCollisionCallback::checkProjectileCollisions(osg::Vec3 projectilePosition) {
    float projectileRadius = _projectile->getProjectileRadius();

    for (auto& target : _targets) {
        osg::Vec3 targetPosition = target->getTransform()->getPosition();
        float targetRadius = target->getTargetRadius();

        osg::Vec3 diff = projectilePosition - targetPosition;
        float distance = diff.length();

        float sumRadius = projectileRadius + targetRadius;
        if (distance <= sumRadius * 2.0f) {
            // std::cout << "Projectile hit the target at position: " 
            //           << targetPosition.x() << ", " << targetPosition.y() << ", " << targetPosition.z()
            //           << std::endl;
            target->setHit(true);
        }
    }
}
