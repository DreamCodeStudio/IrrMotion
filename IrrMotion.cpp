#include "IrrMotion.h"

/* ################ Constructors ################ */

IrrMotion::IrrMotion(irr::scene::ISceneManager *manager)
{
    //Save SceneManager
    _manager = manager;

    //First connect the controller
    while(!_leapController.isConnected())
    {
        std::cout << "Waiting for connection to the LeapMotion hardware..." << std::endl;
        sleep(1);
    }
    std::cout << "LeapMotion controller connected!" << std::endl;

    this->SetupHandBlocks(); //Create all blocks for the hand model
}

/* ################ Public methods ################ */

void IrrMotion::SetSceneManager(irr::scene::ISceneManager *manager)
{
    //Set manager for adding the cubes, which will form the hand, to the scene
    _manager = manager;
}

void IrrMotion::Update()
{
    //Update the position of every block in the hand model
    this->UpdateHandBlocks();
    this->InterpolateHandBlocks();
}

/* ################ Private methods ################ */

void IrrMotion::SetupHandBlocks()
{
    Leap::Frame frame = _leapController.frame();
    Leap::HandList handList = frame.hands();    
    Leap::Hand firstHand = handList[0];
    
    //Create a block on every bones position
    //The blocks which are positioned on every bone
    for (unsigned int i = 0; i < 30; i++)
    {
        _handBlocks.push_back(_manager->addCubeSceneNode(2, 0, -1));
    }

    //Interpolated blocks
    for (unsigned int i = 0; i < INTERPOLATED_BLOCK_AMOUNT * 20; i++)
    {
        _interpolatedHandBlocks.push_back(_manager->addCubeSceneNode(1, 0, -1));
    }
}

void IrrMotion::UpdateHandBlocks()
{
    Leap::Frame frame = _leapController.frame();
    Leap::HandList handList = frame.hands();    
    Leap::Hand firstHand = handList[0];

    for (unsigned int k = 0; k < handList.count(); k++)
    {
        int fingerIndex = 0;
        for (unsigned int i = k * 5; i < (k * 5) + 5; i++)
        {
                _handBlocks[i * 3]->setPosition(irr::core::vector3df(handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_METACARPAL).center().x * 0.2f,
                                                                    handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_METACARPAL).center().y * 0.2f,
                                                                    -handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_METACARPAL).center().z * 0.2f));
                fingerIndex++;
        }
    }
    for (unsigned int k = 0; k < handList.count(); k++)
    {
        int fingerIndex = 0;
        for (unsigned int i = k * 5; i < (k * 5) + 5; i++)
        {
            _handBlocks[1 + (i * 3)]->setPosition(irr::core::vector3df(handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_PROXIMAL).center().x * 0.2f,
                                                                handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_PROXIMAL).center().y * 0.2f,
                                                                -handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_PROXIMAL).center().z * 0.2f));
            fingerIndex++;
        }
    }
    
    for (unsigned int k = 0; k < handList.count(); k++)
    {
        int fingerIndex = 0;
        for (unsigned int i = k * 5; i < (k * 5) + 5; i++)
        {
            _handBlocks[2 + (i * 3)]->setPosition(irr::core::vector3df(handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_INTERMEDIATE).center().x * 0.2f,
                                                                handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_INTERMEDIATE).center().y * 0.2f,
                                                                -handList[k].fingers()[fingerIndex].bone(Leap::Bone::Type::TYPE_INTERMEDIATE).center().z * 0.2f));
            fingerIndex++;            
        }
    }
}

void IrrMotion::InterpolateHandBlocks()
{
    int start = 0;
    int end = INTERPOLATED_BLOCK_AMOUNT;
    for (unsigned int k = 0; k < 10; k++)
    {
        //Interpolate between METACARPAL and PROXIMAL bone
        float dx = _handBlocks[(k * 3) + 1]->getAbsolutePosition().X - _handBlocks[k * 3]->getAbsolutePosition().X;
        float dy = _handBlocks[(k * 3) + 1]->getAbsolutePosition().Y - _handBlocks[k * 3]->getAbsolutePosition().Y;
        float dz = _handBlocks[(k * 3) + 1]->getAbsolutePosition().Z - _handBlocks[k * 3]->getAbsolutePosition().Z;

        float interpolatedX = dx / INTERPOLATED_BLOCK_AMOUNT;
        float interpolatedY = dy / INTERPOLATED_BLOCK_AMOUNT;
        float interpolatedZ = dz / INTERPOLATED_BLOCK_AMOUNT;

        
        for (unsigned int i = start; i < end; i++)
        {
            _interpolatedHandBlocks[i]->setPosition(irr::core::vector3df(_handBlocks[k * 3]->getAbsolutePosition().X + (interpolatedX * (i - start)),
                                                                         _handBlocks[k * 3]->getAbsolutePosition().Y + (interpolatedY * (i - start)),
                                                                         _handBlocks[k * 3]->getAbsolutePosition().Z + (interpolatedZ * (i - start))));
        }
        start += INTERPOLATED_BLOCK_AMOUNT;
        end += INTERPOLATED_BLOCK_AMOUNT;

        //Interpolate between PROXIMAL and INTERMEDIATE bone
        dx = _handBlocks[(k * 3) + 2]->getAbsolutePosition().X - _handBlocks[(k * 3) + 1]->getAbsolutePosition().X;
        dy = _handBlocks[(k * 3) + 2]->getAbsolutePosition().Y - _handBlocks[(k * 3) + 1]->getAbsolutePosition().Y;
        dz = _handBlocks[(k * 3) + 2]->getAbsolutePosition().Z - _handBlocks[(k * 3) + 1]->getAbsolutePosition().Z;

        interpolatedX = dx / INTERPOLATED_BLOCK_AMOUNT;
        interpolatedY = dy / INTERPOLATED_BLOCK_AMOUNT;
        interpolatedZ = dz / INTERPOLATED_BLOCK_AMOUNT;

        
        for (unsigned int i = start; i < end; i++)
        {
            _interpolatedHandBlocks[i]->setPosition(irr::core::vector3df(_handBlocks[(k * 3) + 1]->getAbsolutePosition().X + (interpolatedX * (i - start)),
                                                                        _handBlocks[(k * 3) + 1]->getAbsolutePosition().Y + (interpolatedY * (i - start)),
                                                                        _handBlocks[(k * 3) + 1]->getAbsolutePosition().Z + (interpolatedZ * (i - start))));
        }
        start += INTERPOLATED_BLOCK_AMOUNT;
        end += INTERPOLATED_BLOCK_AMOUNT;
    }
}