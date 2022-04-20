#include "CScene1.h"
#include "CBullet.h"
using namespace std;

cScene1::cScene1(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){    

    hapticDevice = a_hapticDevice;

    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.2);
    matBase.setStaticFriction(0.1);

    // Setup static obstacles
    obstacle0 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle0);
    obstacle0->createAABBCollisionDetector(toolRadius);
    obstacle0->setMaterial(matBase);
    obstacle0->buildDynamicModel();
    obstacle0->setLocalPos(-0.20, -0.05, -0.2+toolRadius);
    obstacle0->m_bulletRigidBody->setCollisionFlags(obstacle0->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle0->m_bulletRigidBody->setUserPointer(obstacle0);
    obstacle0->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle0->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle1 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle1);
    obstacle1->createAABBCollisionDetector(toolRadius);
    obstacle1->setMaterial(matBase);
    obstacle1->buildDynamicModel();
    obstacle1->setLocalPos(-0.10, 0.05, -0.2);
    obstacle1->m_bulletRigidBody->setCollisionFlags(obstacle1->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle1->m_bulletRigidBody->setUserPointer(obstacle1);
    obstacle1->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle1->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    obstacle2 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle2);
    obstacle2->createAABBCollisionDetector(toolRadius);
    obstacle2->setMaterial(matBase);
    obstacle2->buildDynamicModel();
    obstacle2->setLocalPos(0.015, -0.05, -0.2);
    obstacle2->m_bulletRigidBody->setCollisionFlags(obstacle2->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle2->m_bulletRigidBody->setUserPointer(obstacle2);
    obstacle2->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle2->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    obstacle3 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle3);
    obstacle3->createAABBCollisionDetector(toolRadius);
    obstacle3->setMaterial(matBase);
    obstacle3->buildDynamicModel();
    obstacle3->setLocalPos(0.05, 0.05, -0.2);
    obstacle3->m_bulletRigidBody->setCollisionFlags(obstacle3->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle3->m_bulletRigidBody->setUserPointer(obstacle3);
    obstacle3->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle3->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    obstacle4 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle4);
    obstacle4->createAABBCollisionDetector(toolRadius);
    obstacle4->setMaterial(matBase);
    obstacle4->buildDynamicModel();
    obstacle4->setLocalPos(0.15, -0.05, -0.2);
    obstacle4->m_bulletRigidBody->setCollisionFlags(obstacle4->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle4->m_bulletRigidBody->setUserPointer(obstacle4);
    obstacle4->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle4->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    obstacle5 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.02);
    bulletWorld->addChild(obstacle5);
    obstacle5->createAABBCollisionDetector(toolRadius);
    obstacle5->setMaterial(matBase);
    obstacle5->buildDynamicModel();
    obstacle5->setLocalPos(0.25, 0.05, -0.2);
    obstacle5->m_bulletRigidBody->setCollisionFlags(obstacle5->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle5->m_bulletRigidBody->setUserPointer(obstacle5);
    obstacle5->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle5->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    destinations.push_back(cVector3d(0.28,0.28,-0.2+0.00025));
    destinations.push_back(cVector3d(-0.28,-0.28,-0.2+0.00025));

    checkpoints.push_back(cVector3d(-0.25,0.23, -0.20025));
    checkpointsRange.push_back(0.07);
    checkpoints.push_back(cVector3d(-0.15,0.23, -0.20025));
    checkpointsRange.push_back(0.07);
    checkpoints.push_back(cVector3d(-0.14,-0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(-0.07,-0.22, -0.20025));
    checkpointsRange.push_back(0.07);
    checkpoints.push_back(cVector3d(-0.06,0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(0.03,0.22, -0.20025));
    checkpointsRange.push_back(0.02);
    checkpoints.push_back(cVector3d(0.03,-0.22, -0.20025));
    checkpointsRange.push_back(0.02);
    checkpoints.push_back(cVector3d(0.10,-0.22, -0.20025));
    checkpointsRange.push_back(0.05);
    checkpoints.push_back(cVector3d(0.1,0.22, -0.20025));
    checkpointsRange.push_back(0.07);
    checkpoints.push_back(cVector3d(0.2,0.22, -0.20025));
    checkpointsRange.push_back(0.08);
    checkpoints.push_back(cVector3d(0.2,-0.22, -0.20025));
    checkpointsRange.push_back(0.06);
    checkpoints.push_back(cVector3d(0.27,-0.21, -0.20025));
    checkpointsRange.push_back(0.04);




    // set up target
    target = new cBulletCylinder(bulletWorld, 0.0005, toolRadius*1.5);
    bulletWorld->addChild(target);
    target->createAABBCollisionDetector(toolRadius);
    target->setMaterial(matBase);
    target->buildDynamicModel();
    target->m_material->setGreen();
    target->m_bulletRigidBody->setCollisionFlags(target->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    target->m_bulletRigidBody->setUserPointer(target);

}

void cScene1::setStiffness(double a_stiffness){
    bulletGround->setStiffness(a_stiffness);

    obstacle0->setStiffness(a_stiffness);
    obstacle1->setStiffness(a_stiffness);
    obstacle2->setStiffness(a_stiffness);
    obstacle3->setStiffness(a_stiffness);
    obstacle4->setStiffness(a_stiffness);
    obstacle5->setStiffness(a_stiffness);
}

void cScene1::initWaypoints(){
    if((target->getLocalPos()).equals(destinations[0])){
        last_waypoint_index = 3;
        waypoint_index = 4;
    }else if((target->getLocalPos()).equals(destinations[1])){
        last_waypoint_index = 7; 
        waypoint_index = 8;
    }
}
void cScene1::generateWaypoints(cVector3d positionSphere, cVector3d positionTarget){
if(positionTarget.equals(destinations[0]))
    {   waypoints.push_back(checkpoints[0]);
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
        

    }else if(positionTarget.equals(destinations[1])){
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