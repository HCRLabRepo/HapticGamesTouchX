#include "CScene3.h"
using namespace std;

cScene3::cScene3(shared_ptr<cGenericHapticDevice> a_hapticDevice):cGenericScene(a_hapticDevice){
    
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    double x = -0.013;
    double y = 0;

    for(int i =0; i<20; i++){
        int direction = i%2 == 0? 90:0;
        cout << (pow(-1,(i+3)/2))*(((i+3)/2)*0.012) << endl;
        x += (pow(-1,i/2))*(0.012 + (i/2)*0.012);
        y += (pow(-1,(i+3)/2))*(((i+3)/2)*0.012);
        //cout<< x << "," << y <<endl;
 
        
        obstacles[i] = new cBulletBox(bulletWorld, 0.005, 0.026*(i/2+1), 0.015);
        bulletWorld->addChild(obstacles[i]);
        obstacles[i]->setMaterial(matBase);
        obstacles[i]->buildDynamicModel();
        obstacles[i]->setLocalPos(x, y, -0.2);
        obstacles[i]->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(direction), C_EULER_ORDER_XYZ));
    }


}

void cScene3::setStiffness(double a_stiffness){
    bulletGround->setStiffness(a_stiffness);
    for(int i =0; i<10; i++){
        obstacles[i]->setStiffness(a_stiffness);
    }
}