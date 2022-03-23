#pragma once

#include "chai3d.h"
#include "CGenericScene.h"

class cScene1: public cGenericScene{
    public:
        cScene1(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~cScene1() {};
    
    public:
        virtual void setStiffness(double a_stiffness);

    public:

        cBulletBox* obstacle0;
        cBulletBox* obstacle1;
        cBulletBox* obstacle2;
        cBulletBox* obstacle3;
        cBulletBox* obstacle4;
        cBulletBox* obstacle5;
};