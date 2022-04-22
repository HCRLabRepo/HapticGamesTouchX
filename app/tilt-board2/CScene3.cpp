#include "CScene3.h"
using namespace std;

cScene3::cScene3(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){
    
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.5);
    matBase.setStaticFriction(0.9);
    double x = -0.013;
    double y = 0;

    for(int i =0; i<20; i++){
        int direction = i%2 == 0? 90:0;
        // cout << (pow(-1,(i+3)/2))*(((i+3)/2)*0.012) << endl;
        x += (pow(-1,i/2))*(0.013 + (i/2)*0.013);
        y += (pow(-1,(i+3)/2))*(((i+3)/2)*0.013);
        //cout<< x << "," << y <<endl;
 
        
        obstacles[i] = new cBulletBox(bulletWorld, 0.006, 0.026*(i/2+1.5), 0.015);
        bulletWorld->addChild(obstacles[i]);
        obstacles[i]->setMaterial(matBase);
        obstacles[i]->buildDynamicModel();
        obstacles[i]->setLocalPos(x, y, -0.2);
        obstacles[i]->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(direction), C_EULER_ORDER_XYZ));
        obstacles[i]->m_bulletRigidBody->setCollisionFlags(obstacles[i]->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        obstacles[i]->m_bulletRigidBody->setUserPointer(obstacles[i]);
        obstacles[i]->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
        obstacles[i]->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);
    }

    
    destinations.push_back(cVector3d(0.00,0.00,-0.2+0.00025));
    destinations.push_back(cVector3d(-0.28,-0.28,-0.2+0.00025));


    checkpoints.push_back(cVector3d(-0.029, 0.00, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.029, -0.028, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.028, -0.028, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.028, 0.052, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.053, 0.052, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.052, -0.051, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.055, -0.051, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.055, 0.077, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.080, 0.077, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.080, -0.080, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.080, -0.080, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.080, 0.105, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.105, 0.105, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.105, -0.105, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.105, -0.105, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.105, 0.130, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.130, 0.130, -0.20025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.130, -0.140, -0.20025));
    checkpointsRange.push_back(0.015);

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

void cScene3::setStiffness(double a_stiffness){
    bulletGround->setStiffness(a_stiffness);
    for(int i =0; i<10; i++){
        obstacles[i]->setStiffness(a_stiffness);
    }
}

void cScene3::initWaypoints(){
    last_waypoint_index = 0;
    waypoint_index = 0;
}
void cScene3::generateWaypoints(cVector3d positionSphere, cVector3d positionTarget){
    
    if(positionTarget.equals(destinations[1]))
    {   
        for(int i = 0; i < checkpoints.size(); i++){
            waypoints.push_back(checkpoints[i]);
            waypointsRange.push_back(checkpointsRange[i]);
        }
        waypoints.push_back(positionTarget);
        waypointsRange.push_back(0.01);
        waypoints.push_back(checkpoints[checkpoints.size()-1]);
        waypointsRange.push_back(checkpointsRange[checkpoints.size()-1]);
    }
    else if(positionTarget.equals(destinations[0]))
    {
        for(int i = checkpoints.size()-1; i >= 0; i--){
            waypoints.push_back(checkpoints[i]);
            waypointsRange.push_back(checkpointsRange[i]);
        }
        waypoints.push_back(positionTarget);
        waypointsRange.push_back(0.01);
        waypoints.push_back(checkpoints[0]);
        waypointsRange.push_back(checkpointsRange[0]);

    }
}