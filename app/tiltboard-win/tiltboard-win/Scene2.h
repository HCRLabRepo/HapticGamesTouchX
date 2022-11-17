#pragma once

#include "chai3d.h"
#include "GenericScene.h"

/**
* \file Scene1.h
*
* Header file for Scene 2 ("Intermediate" scene)
*
*/
class Scene2: public GenericScene{
    public:
        /**
        * Creates the scene, sets up the obstacles, destinations, checkpoints and targets.
        *
        * \param a_hapticDevice A pointer to the connected haptic device
        */
        Scene2(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

        virtual ~Scene2() {};
    
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
        * Updates the waypoint for the guidance sphere to follow if the main sphere moves closer to it
        * 
        * \param positionSphere position of the main sphere
        * \param positionTarget position of the current target
        */
        void updateWaypoint(cVector3d positionSphere, cVector3d positionTarget) override;

        /**
        * Generates and initialises all aspects of the fuzzy logic engine
        */
        void engineSetup();

};