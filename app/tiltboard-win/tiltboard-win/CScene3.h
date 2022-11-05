#pragma once

#include "chai3d.h"
#include "CGenericScene.h"

class cScene3: public cGenericScene{
    public:
        cScene3(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~cScene3() {};
    
    public:
        virtual void setStiffness(double a_stiffness);
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;
        void initWaypoints() override;

    public:

        cBulletBox* obstacles[20];

};