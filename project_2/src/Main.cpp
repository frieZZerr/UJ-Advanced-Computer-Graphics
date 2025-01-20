#include "scene/Scene.h"
#include "scene/objects/Cannon.h"
#include "input/InputHandler.h"

#include <osgViewer/Viewer>

int main(int argc, char** argv) {
    Scene scene;
    osg::ref_ptr<osg::Group> root = scene.getRoot();

    osg::ref_ptr<Cannon> cannon = scene.getCannon();
    auto targetTransform = dynamic_cast<osg::PositionAttitudeTransform*>(root->getChild(2));

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);

    osg::ref_ptr<InputHandler> inputHandler = new InputHandler(root, cannon, targetTransform);
    viewer.addEventHandler(inputHandler);

    return viewer.run();
}
