#pragma once

#include "chai3d.h"
#include "CGenericScene.h"

class cScene2: public cGenericScene{
    public:
        cScene2(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~cScene2() {};
    
    public:
        virtual void setStiffness(double a_stiffness);
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;
        void updateWaypoint(cVector3d positionSphere, cVector3d positionTarget) override;


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