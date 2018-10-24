#include <iostream>

#include "IrrMotion.h"

int main()
{
    irr::IrrlichtDevice *device = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1920, 1080), 16, false, false, false);

    irr::video::IVideoDriver *driver = device->getVideoDriver();
    irr::scene::ISceneManager *manager = device->getSceneManager();
    irr::scene::ICameraSceneNode *camera = manager->addCameraSceneNode(0, irr::core::vector3df(0, 55, -50), irr::core::vector3df(0, 0, 0), -1, true);

    IrrMotion irrMotion(manager);

    while(device->run())
    {
        driver->beginScene(true, true, irr::video::SColor(100, 100, 100, 100));

        irrMotion.Update();
        manager->drawAll();

        driver->endScene();
    }

    return 0;
}