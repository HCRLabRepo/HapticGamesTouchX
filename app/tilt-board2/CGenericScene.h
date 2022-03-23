#pragma once

#include "chai3d.h"
#include "CBullet.h"

using namespace chai3d;

class cGenericScene{

public:
    // Public constructor and deconstructor.
    cGenericScene(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);
    virtual ~cGenericScene(){};

public:
    // Public Methods
    virtual void init();
    virtual void updateHaptics(double timeInterval);
    virtual void updateGraphics(int a_width, int a_height);
    virtual void setStiffness(double a_stiffness){};

public:

    cBulletWorld* bulletWorld;
    cCamera* camera;
    cSpotLight* spotLight;
    cDirectionalLight *directionalLight;
    cBulletStaticPlane* bulletGround; 
    double toolRadius;
    cPrecisionClock simClock;
    bool mirroredDisplay;
    cVector3d sphereVel;

    cBulletBox* border1;
    cBulletBox* border2;
    cBulletBox* border3;
    cBulletBox* border4;
    std::shared_ptr<cGenericHapticDevice> hapticDevice;

    cShapeSphere* controlSphere;
    cBulletSphere* movingSphere;
};
