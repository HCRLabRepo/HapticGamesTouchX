#include "DebugScene.h"
#include "CBullet.h"

DebugScene::DebugScene(std::shared_ptr<cGenericHapticDevice> a_hapticDevice):GenericScene(a_hapticDevice){
	
    hapticDevice = a_hapticDevice;

    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.5);
    matBase.setStaticFriction(0.9);

    destinations.push_back(cVector3d(0.28, 0.28, -0.2 + 0.00025));
    destinations.push_back(cVector3d(-0.28, -0.28, -0.2 + 0.00025));

    checkpoints.push_back(cVector3d(-0.25, 0.23, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(-0.15, 0.23, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(-0.14, -0.22, -0.20025));
    checkpointsRange.push_back(0.05);
    checkpoints.push_back(cVector3d(-0.07, -0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(-0.06, 0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(0.03, 0.22, -0.20025));
    checkpointsRange.push_back(0.02);
    checkpoints.push_back(cVector3d(0.03, -0.22, -0.20025));
    checkpointsRange.push_back(0.02);
    checkpoints.push_back(cVector3d(0.10, -0.22, -0.20025));
    checkpointsRange.push_back(0.05);
    checkpoints.push_back(cVector3d(0.1, 0.22, -0.20025));
    checkpointsRange.push_back(0.07);
    checkpoints.push_back(cVector3d(0.2, 0.22, -0.20025));
    checkpointsRange.push_back(0.08);
    checkpoints.push_back(cVector3d(0.2, -0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(0.27, -0.21, -0.20025));
    checkpointsRange.push_back(0.04);




    // set up target
    target = new cBulletCylinder(bulletWorld, 0.0005, toolRadius * 1.5);
    bulletWorld->addChild(target);
    target->createAABBCollisionDetector(toolRadius);
    target->setMaterial(matBase);
    target->buildDynamicModel();
    target->m_material->setGreen();
    target->m_bulletRigidBody->setCollisionFlags(target->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    target->m_bulletRigidBody->setUserPointer(target);
}

void DebugScene::setStiffness(double a_stiffness) {
    bulletGround->setStiffness(a_stiffness);
}

void DebugScene::initWaypoints() {
    if ((target->getLocalPos()).equals(destinations[0])) {
        last_waypoint_index = 3;
        waypoint_index = 4;
    }
    else if ((target->getLocalPos()).equals(destinations[1])) {
        last_waypoint_index = 7;
        waypoint_index = 8;
    }
}
void DebugScene::generateWaypoints(cVector3d positionSphere, cVector3d positionTarget) {
    if (positionTarget.equals(destinations[0]))
    {
        waypoints.push_back(checkpoints[0]);
        waypointsRange.push_back(checkpointsRange[0]);
        waypoints.push_back(checkpoints[1]);
        waypointsRange.push_back(checkpointsRange[1]);
        waypoints.push_back(checkpoints[2]);
        waypointsRange.push_back(checkpointsRange[2]);
        waypoints.push_back(checkpoints[3]);
        waypointsRange.push_back(checkpointsRange[4]);
        waypoints.push_back(checkpoints[4]);
        waypointsRange.push_back(checkpointsRange[4]);
        waypoints.push_back(checkpoints[5]);
        waypointsRange.push_back(checkpointsRange[5]);
        waypoints.push_back(checkpoints[6]);
        waypointsRange.push_back(checkpointsRange[6]);
        waypoints.push_back(checkpoints[7]);
        waypointsRange.push_back(checkpointsRange[7]);
        waypoints.push_back(checkpoints[8]);
        waypointsRange.push_back(checkpointsRange[8]);
        waypoints.push_back(checkpoints[9]);
        waypointsRange.push_back(checkpointsRange[9]);
        waypoints.push_back(checkpoints[10]);
        waypointsRange.push_back(checkpointsRange[10]);
        waypoints.push_back(checkpoints[11]);
        waypointsRange.push_back(checkpointsRange[11]);

        waypoints.push_back(destinations[0]);
        waypointsRange.push_back(0.005);
        waypoints.push_back(checkpoints[11]);
        waypointsRange.push_back(checkpointsRange[11]);


    }
    else if (positionTarget.equals(destinations[1])) {
        waypoints.push_back(checkpoints[11]);
        waypointsRange.push_back(checkpointsRange[11]);
        waypoints.push_back(checkpoints[10]);
        waypointsRange.push_back(checkpointsRange[10]);
        waypoints.push_back(checkpoints[9]);
        waypointsRange.push_back(checkpointsRange[9]);
        waypoints.push_back(checkpoints[8]);
        waypointsRange.push_back(checkpointsRange[8]);
        waypoints.push_back(checkpoints[7]);
        waypointsRange.push_back(checkpointsRange[7]);
        waypoints.push_back(checkpoints[6]);
        waypointsRange.push_back(checkpointsRange[6]);
        waypoints.push_back(checkpoints[5]);
        waypointsRange.push_back(checkpointsRange[5]);
        waypoints.push_back(checkpoints[4]);
        waypointsRange.push_back(checkpointsRange[4]);
        waypoints.push_back(checkpoints[3]);
        waypointsRange.push_back(checkpointsRange[3]);
        waypoints.push_back(checkpoints[2]);
        waypointsRange.push_back(checkpointsRange[2]);
        waypoints.push_back(checkpoints[1]);
        waypointsRange.push_back(checkpointsRange[1]);
        waypoints.push_back(checkpoints[0]);
        waypointsRange.push_back(checkpointsRange[0]);

        waypoints.push_back(destinations[1]);
        waypointsRange.push_back(0.005);
        waypoints.push_back(checkpoints[0]);
        waypointsRange.push_back(checkpointsRange[0]);


    }

}

