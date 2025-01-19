#ifndef TARGET_H
#define TARGET_H

#include <osg/PositionAttitudeTransform>
#include <osg/ref_ptr>

static const float TARGET_DIAMETER = 1.0f;

osg::ref_ptr<osg::PositionAttitudeTransform> createTarget();

#endif // TARGET_H
