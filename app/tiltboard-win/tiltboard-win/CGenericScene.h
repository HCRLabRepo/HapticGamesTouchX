#pragma once

#include "chai3d.h"
#include "CBullet.h"
#include "fl/Headers.h"
#include <time.h>

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
    virtual void updateWaypoint(cVector3d positionSphere, cVector3d positionTarget);
    virtual void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget){};
    double getFuzzyOutput(int timein, int collisionsin);
    virtual void initWaypoints(){};
    void updateTarget();
    

public:

    cBulletWorld* bulletWorld;
    cCamera* camera;
    cSpotLight* spotLight;
    cDirectionalLight *directionalLight;
    cBulletStaticPlane* bulletGround; 
    cBulletStaticPlane* bulletCeilling;
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
    cBulletSphere* negotiatedSphere;
    cBulletSphere* guidanceSphere;
    cBulletSphere* mainSphere;
    cBulletCylinder* target;

    std::vector<cVector3d> destinations;
    std::vector<int> shuffled_order;

    std::vector<cVector3d> waypoints;
    std::vector<double> waypointsRange;

    cVector3d positionWaypoint;

    std::vector<cVector3d> checkpoints;
    std::vector<double> checkpointsRange;
    
    int destination_index = 0;
    int waypoint_index = 0;
    int last_waypoint_index = 1;
    double ALPHA_CONTROL = 1;
    double K_DAMPING_VELOCITY = 0.2;
    bool userInactive = false;
    double userForce = 0.0;
    int collisionNum = 0;
    time_t startTime;
    bool fuzzyControl = false;
   

    cLabel* labelHapticDeviceModel;
    cLabel* labelHapticDevicePosition;
    cLabel* labelRates;
    cLabel* labelTime;
    cLabel* labelCollisions;
    cLevel* controlLevel;

    cVector3d positionNegotiatedSphere;
    cVector3d positionMainSphere;
    cVector3d positionGuidanceSphere;
    cVector3d sphereForce;
    cVector3d guidanceForce;

    fl::Engine* engine;

};
