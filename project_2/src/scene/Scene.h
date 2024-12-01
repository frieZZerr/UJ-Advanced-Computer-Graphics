#ifndef SCENE_H
#define SCENE_H

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osg/ref_ptr>

osg::ref_ptr<osg::Group> createScene();

#endif // SCENE_H
