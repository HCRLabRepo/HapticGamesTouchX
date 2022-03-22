#pragma once

#include "chai3d.h"
#include "CGenericScene.h"

class cScene3: public cGenericScene{
    public:
        cScene3(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~cScene3() {};
    
    public:
        virtual void setStiffness(double a_stiffness);

    public:

        cBulletBox* obstaclesRow[20];
        cBulletBox* obstaclesColumns[10];
};