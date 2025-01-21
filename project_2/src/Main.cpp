#include "scene/Scene.h"
#include "scene/objects/Cannon.h"
#include "scene/objects/Target.h"
#include "input/InputHandler.h"

#include <osgViewer/Viewer>

int main(int argc, char** argv) {
    osg::ref_ptr<Scene> scene = new Scene();
    osg::ref_ptr<osg::Group> root = scene->getRoot();

    osg::ref_ptr<Cannon> cannon = scene->getCannon();
    std::vector<osg::ref_ptr<Target>>& targets = scene->getTargets();

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);

    osg::ref_ptr<InputHandler> inputHandler = new InputHandler(root, cannon, targets);
    viewer.addEventHandler(inputHandler);

    return viewer.run();
}
