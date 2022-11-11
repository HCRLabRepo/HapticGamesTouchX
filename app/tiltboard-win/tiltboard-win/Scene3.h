#pragma once

#include "chai3d.h"
#include "GenericScene.h"

class Scene3: public GenericScene{
    public:
        Scene3(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~Scene3() {};
    
    public:
        virtual void setStiffness(double a_stiffness);
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;
        void initWaypoints() override;
        void engineSetup();

    public:

        cBulletBox* obstacles[20];

};