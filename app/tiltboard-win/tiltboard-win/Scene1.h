#pragma once

#include <chai3d.h>
#include "GenericScene.h"
/**
* \file Scene1.h
*
* Header file for Scene 1 ("Easy" scene)
* 
*/
class Scene1: public GenericScene{
    public:
        /**
        * Creates the scene, sets up the obstacles, destinations, checkpoints and targets.
        * 
        * \param a_hapticDevice A pointer to the connected haptic device
        */
        Scene1(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~Scene1() {};
    
    public:
        /**
        * Sets the stiffness of the ground
        * 
        * \param a_stifness The stiffness level to be applied to the ground
        */
        virtual void setStiffness(double a_stiffness);

        /**
        * Generates the list of waypoints based on what destination the main sphere is closest to
        * 
        * \param positionSphere position of the main sphere
        * \param positionTarget position of the current target
        */
        void generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) override;

        /**
        * Generates and initialises all aspects of the fuzzy logic engine
        */
        void engineSetup();
        
        /**
        * Decides which waypoint to follow based on the main sphere's current position
        */
        void initWaypoints() override;

};