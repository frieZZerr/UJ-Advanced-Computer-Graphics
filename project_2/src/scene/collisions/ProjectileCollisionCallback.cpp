#include "scene/collisions/ProjectileCollisionCallback.h"
#include "scene/objects/Projectile.h"
#include "scene/objects/Target.h"

#include <osg/PositionAttitudeTransform>
#include <iostream>

void ProjectileCollisionCallback::operator()(osg::Node* node, osg::NodeVisitor* nv) {
    traverse(node, nv);

    osg::PositionAttitudeTransform* projectilePAT = dynamic_cast<osg::PositionAttitudeTransform*>(node);
    if (!projectilePAT) return;

    osg::Vec3 projectilePos = projectilePAT->getPosition();

    if (projectilePos.z() <= -PROJECTILE_DIAMETER) {
        // std::cout << "Projectile hit the ground!" << std::endl;

        projectilePos.z() = -PROJECTILE_DIAMETER/2.0f;
        projectilePAT->setPosition(projectilePos);
        projectilePAT->setUpdateCallback(nullptr);

        return;
    }

    if (!_targetTransform.valid()) return;

    osg::Vec3 targetPos = _targetTransform->getPosition();

    osg::Vec3 diff = projectilePos - targetPos;
    float distance = diff.length();

    float sumDiameter = PROJECTILE_DIAMETER + TARGET_DIAMETER;
    if (distance <= sumDiameter) {
        // std::cout << "Projectile hit the target! Removing target." << std::endl;
        _targetTransform->setNodeMask(0x0);

        return;
    }

    return;
}
