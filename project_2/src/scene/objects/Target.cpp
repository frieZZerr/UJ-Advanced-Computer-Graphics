#include "scene/objects/Target.h"
#include "scene/objects/Grass.h"

#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <random>

Target::Target(Grass* grass) {
    this->_grass = grass;
    this->_targetSpawnRangeX = grass->getWidth();
    this->_targetSpawnRangeZ = grass->getHeight();
    createTarget();
}

void Target::createTarget() {
    this->_targetTransform = new osg::PositionAttitudeTransform();
    this->_targetBody = new osg::Geode();

    osg::ref_ptr<osg::ShapeDrawable> targetShape = new osg::ShapeDrawable(
        new osg::Sphere(osg::Vec3(0.0f, 0.0f, this->_target_radius * 2.0f), this->_target_radius * 2.0f)
    );

    targetShape->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
    _targetBody->addDrawable(targetShape);

    _targetTransform->addChild(_targetBody);

    _targetTransform->setPosition(generateRandomPosition());
}

void Target::setPosition(const osg::Vec3& position) {
    this->_targetTransform->setPosition(position);
}

void Target::setHit(bool isHit) {
    this->_hit = isHit;

    if (_hit) {
        this->_targetTransform->setNodeMask(0x0);
    }
}

osg::Vec3 Target::generateRandomPosition() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    float margin = this->_grass->getMargin();
    std::uniform_real_distribution<float> disX(margin, (this->_targetSpawnRangeX / 2.0f - margin));  // X_AXIS -> [25, 50]
    std::uniform_real_distribution<float> disZ(-(this->_targetSpawnRangeZ / 2.0f - margin), (this->_targetSpawnRangeZ / 2.0f - margin));  // Z_AXIS -> [-25, 25]

    float randomX = disX(gen);
    float randomZ = disZ(gen);
    float groundHeight = 0.0f;  // Assume ground height is zero

    return osg::Vec3(randomX, randomZ, groundHeight);
}
