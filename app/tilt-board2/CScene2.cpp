#include "CScene2.h"
using namespace std;

cScene2::cScene2(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){
    
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(1500);

    // Setup static obstacles
    obstacle0 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle0);
    obstacle0->createAABBCollisionDetector(toolRadius);
    obstacle0->setMaterial(matBase);
    obstacle0->buildDynamicModel();
    obstacle0->setLocalPos(0.16, 0.18, -0.2);
    obstacle0->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));


    obstacle1 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle1);
    obstacle1->createAABBCollisionDetector(toolRadius);
    obstacle1->setMaterial(matBase);
    obstacle1->buildDynamicModel();
    obstacle1->setLocalPos(0.18, 0.16, -0.2);
    obstacle1->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));

    obstacle2 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle2);
    obstacle2->createAABBCollisionDetector(toolRadius);
    obstacle2->setMaterial(matBase);
    obstacle2->buildDynamicModel();
    obstacle2->setLocalPos(-0.18, -0.16, -0.2);
    obstacle2->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));

    obstacle3 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle3);
    obstacle3->createAABBCollisionDetector(toolRadius);
    obstacle3->setMaterial(matBase);
    obstacle3->buildDynamicModel();
    obstacle3->setLocalPos(-0.16, -0.18, -0.2);
    obstacle3->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));

    obstacle4 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle4);
    obstacle4->createAABBCollisionDetector(toolRadius);
    obstacle4->setMaterial(matBase);
    obstacle4->buildDynamicModel();
    obstacle4->setLocalPos(0.18, -0.16, -0.2);
    obstacle4->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));

    obstacle5 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle5);
    obstacle5->createAABBCollisionDetector(toolRadius);
    obstacle5->setMaterial(matBase);
    obstacle5->buildDynamicModel();
    obstacle5->setLocalPos(0.16, -0.18, -0.2);
    obstacle5->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));

    obstacle6 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle6);
    obstacle6->createAABBCollisionDetector(toolRadius);
    obstacle6->setMaterial(matBase);
    obstacle6->buildDynamicModel();
    obstacle6->setLocalPos(-0.16, 0.18, -0.2);
    obstacle6->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));

    obstacle7 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle7);
    obstacle7->createAABBCollisionDetector(toolRadius);
    obstacle7->setMaterial(matBase);
    obstacle7->buildDynamicModel();
    obstacle7->setLocalPos(-0.18, 0.16, -0.2);
    obstacle7->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));
}

void cScene2::setStiffness(double a_stiffness){
    bulletGround->setStiffness(a_stiffness);

    obstacle0->setStiffness(a_stiffness);
    obstacle1->setStiffness(a_stiffness);
    obstacle2->setStiffness(a_stiffness);
    obstacle3->setStiffness(a_stiffness);
    obstacle4->setStiffness(a_stiffness);
    obstacle5->setStiffness(a_stiffness);
    obstacle6->setStiffness(a_stiffness);
    obstacle7->setStiffness(a_stiffness);
}