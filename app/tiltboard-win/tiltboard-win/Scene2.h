#pragma once

#include "chai3d.h"
#include "GenericScene.h"

class Scene2: public GenericScene{
    public:
        Scene2(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~Scene2() {};
    
    public:
        virtual void setStiffness(double a_stiffness);
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;
        void updateWaypoint(cVector3d positionSphere, cVector3d positionTarget) override;
        void engineSetup();


    public:

        cBulletBox* obstacle0;
        cBulletBox* obstacle1;
        cBulletBox* obstacle2;
        cBulletBox* obstacle3;
        cBulletBox* obstacle4;
        cBulletBox* obstacle5;
        cBulletBox* obstacle6;
        cBulletBox* obstacle7;
};