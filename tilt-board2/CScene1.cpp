#include "CScene1.h"
using namespace std;

cScene1::cScene1(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){    
    
    hapticDevice = a_hapticDevice;

    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.2);
    matBase.setStaticFriction(0.1);

    // Setup static obstacles
    obstacle0 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle0);
    obstacle0->createAABBCollisionDetector(toolRadius);
    obstacle0->setMaterial(matBase);
    obstacle0->buildDynamicModel();
    obstacle0->setLocalPos(-0.20, -0.05, -0.2);


    obstacle1 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle1);
    obstacle1->createAABBCollisionDetector(toolRadius);
    obstacle1->setMaterial(matBase);
    obstacle1->buildDynamicModel();
    obstacle1->setLocalPos(-0.10, 0.05, -0.2);

    obstacle2 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle2);
    obstacle2->createAABBCollisionDetector(toolRadius);
    obstacle2->setMaterial(matBase);
    obstacle2->buildDynamicModel();
    obstacle2->setLocalPos(0.015, -0.05, -0.2);

    obstacle3 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle3);
    obstacle3->createAABBCollisionDetector(toolRadius);
    obstacle3->setMaterial(matBase);
    obstacle3->buildDynamicModel();
    obstacle3->setLocalPos(0.05, 0.05, -0.2);

    obstacle4 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle4);
    obstacle4->createAABBCollisionDetector(toolRadius);
    obstacle4->setMaterial(matBase);
    obstacle4->buildDynamicModel();
    obstacle4->setLocalPos(0.15, -0.05, -0.2);

    obstacle5 = new cBulletBox(bulletWorld, 0.005, 0.5, 0.01);
    bulletWorld->addChild(obstacle5);
    obstacle5->createAABBCollisionDetector(toolRadius);
    obstacle5->setMaterial(matBase);
    obstacle5->buildDynamicModel();
    obstacle5->setLocalPos(0.25, 0.05, -0.2);
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