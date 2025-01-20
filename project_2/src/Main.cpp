#include "scene/Scene.h"
#include "input/InputHandler.h"

#include <osgViewer/Viewer>

int main(int argc, char** argv) {
    Scene scene;
    osg::ref_ptr<osg::Group> root = scene.getRoot();

    auto cannonTransform = dynamic_cast<osg::PositionAttitudeTransform*>(root->getChild(1));
    auto targetTransform = dynamic_cast<osg::PositionAttitudeTransform*>(root->getChild(2));

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);

    osg::ref_ptr<InputHandler> inputHandler = new InputHandler(root, cannonTransform, targetTransform);
    viewer.addEventHandler(inputHandler);

    return viewer.run();
}
