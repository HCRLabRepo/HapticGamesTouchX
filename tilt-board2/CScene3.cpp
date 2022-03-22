#include "CScene3.h"
using namespace std;

cScene3::cScene3(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){
    
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(1500);

    for(int i =0; i<20; i++){
        int direction = i%2 == 0? -1:1;

        obstaclesRow[i] = new cBulletBox(bulletWorld, 0.005, 0.020*(i+1), 0.01);
        bulletWorld->addChild(obstaclesRow[i]);
        obstaclesRow[i]->setMaterial(matBase);
        obstaclesRow[i]->buildDynamicModel();
        obstaclesRow[i]->setLocalPos(-0.013 + i*0.015 * direction, -0.00 + 0.005 * direction, -0.2);
    }
    for(int i =0; i<2; i++){
        int direction = i%2 == 0? -1:1;
        obstaclesColumns[i] = new cBulletBox(bulletWorld, 0.019*(i+1), 0.005, 0.01);
        bulletWorld->addChild(obstaclesColumns[i]);
        obstaclesColumns[i]->setMaterial(matBase);
        obstaclesColumns[i]->buildDynamicModel();
        obstaclesColumns[i]->setLocalPos(-0.005 + 0.005 * direction, -0.009 + i*0.015 * direction, -0.2);
    }

}

void cScene3::setStiffness(double a_stiffness){
    bulletGround->setStiffness(a_stiffness);
    for(int i =0; i<10; i++){
        obstaclesRow[i]->setStiffness(a_stiffness);
    }
}