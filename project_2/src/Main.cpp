#include "scene/Scene.h"
#include "input/InputHandler.h"

#include <osgViewer/Viewer>

int main(int argc, char** argv) {
    osg::ref_ptr<osg::Group> root = createScene();

    // Pobierz transformację armaty
    auto cannonTransform = dynamic_cast<osg::PositionAttitudeTransform*>(root->getChild(1));

    osgViewer::Viewer viewer;
    viewer.setSceneData(root);

    // Dodaj handler wejściowy
    viewer.addEventHandler(new InputHandler(cannonTransform));

    return viewer.run();
}
