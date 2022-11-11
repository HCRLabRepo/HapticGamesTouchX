#pragma once

#include <chai3d.h>
#include "GenericScene.h"

class Scene1: public GenericScene{
    public:
        Scene1(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~Scene1() {};
    
    public:
        virtual void setStiffness(double a_stiffness);
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;
        void engineSetup();
        void initWaypoints() override;

    public:

        cBulletBox* obstacle0;
        cBulletBox* obstacle1;
        cBulletBox* obstacle2;
        cBulletBox* obstacle3;
        cBulletBox* obstacle4;
        cBulletBox* obstacle5;
};