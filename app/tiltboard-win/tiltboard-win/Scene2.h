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

};