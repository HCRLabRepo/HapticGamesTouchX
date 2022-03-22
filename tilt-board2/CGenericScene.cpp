#include "CGenericScene.h"
#include "CBullet.h"
using namespace std;

const double SPHERE_MASS        = 0.04;
const double SPHERE_STIFFNESS   = 100.0;
const double K_DAMPING          = 0.98;
const double K_SPRING           = 5.0;


cGenericScene::cGenericScene(shared_ptr<cGenericHapticDevice> a_hapticDevice)
{   hapticDevice = a_hapticDevice;
    mirroredDisplay = false;
    toolRadius = 0.0025;

    //-----------------------------------------------------------------------
    // WORLD - CAMERA - LIGHTING
    //-----------------------------------------------------------------------
 
    // Create the world.
    bulletWorld = new cBulletWorld();
    bulletWorld->m_backgroundColor.setBlack();

    // Create the camera.
    camera = new cCamera(bulletWorld);
    bulletWorld->addChild(camera);
    camera->setSphericalReferences(cVector3d(0.00, 0.00, -0.20),cVector3d(0.00, 0.00, 1.00), cVector3d(1.00, 0.00, 0.00));
    camera->set(cVector3d(0.50, 0.00, 0.40),    // Camera position (eye)
                cVector3d(0.00, 0.00, -0.20),    // Lookat position (target)
                cVector3d(0.00, 0.00, 1.00));    // Direction of the (up) vector
    camera->setClippingPlanes(0.01, 10.0); // Set near and far clipping planes of the camera
    camera->setStereoEyeSeparation(0.01);
    camera->setStereoFocalLength(0.5);
    camera->setMirrorVertical(mirroredDisplay);
    
    //Create a spot light.
    spotLight = new cSpotLight(bulletWorld);
    bulletWorld->addChild(spotLight);
    spotLight->setEnabled(true); // Enable light source
    spotLight->setLocalPos(0.4, 0.4, 0.3); // Posision the light source
    spotLight->setDir(-0.4, -0.4, -0.3); // Define the direction of the light beam
    spotLight->setSpotExponent(0.0); // Set uniform concentration of light
    spotLight->setShadowMapEnabled(true); // Enable this light source to generate shadows
    spotLight->m_shadowMap->setQualityVeryHigh(); // Set the resolution of the shadow map
    spotLight->m_ambient.set(0.6, 0.6, 0.6);
    spotLight->m_diffuse.set(0.8, 0.8, 0.8);
    spotLight->m_specular.set(0.8, 0.8, 0.8);
    spotLight->setCutOffAngleDeg(45); // Set the light cone half angle

    // Create a directional light.
    directionalLight = new cDirectionalLight(bulletWorld);
    bulletWorld->addChild(directionalLight);                   // attach light to camera
    directionalLight->setEnabled(true);                    // enable light source
    directionalLight->setDir(0.0, 0.2, -1.0);             // define the direction of the light beam
    directionalLight->m_ambient.set(0.3, 0.3, 0.3);
    directionalLight->m_diffuse.set(0.6, 0.6, 0.6);
    directionalLight->m_specular.set(0.0, 0.0, 0.0);

    // Create the ground.
    double planeWidth = 0.2;
    bulletGround = new cBulletStaticPlane(bulletWorld,  cVector3d(0.0, 0.0, 1.0),-planeWidth);
    bulletWorld->addChild(bulletGround);
    cCreatePlane(bulletGround, 0.6, 0.6, bulletGround->getPlaneConstant() * bulletGround->getPlaneNormal());

    // Create boarders
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.2);
    matBase.setStaticFriction(0.1);

    border1 = new cBulletBox(bulletWorld, 0.005, 0.6, 0.01);
    bulletWorld->addChild(border1);
    border1->createAABBCollisionDetector(toolRadius);
    border1->setMaterial(matBase);
    border1->buildDynamicModel();
    border1->setLocalPos(0.3, 0.0, -0.2);

    border2 = new cBulletBox(bulletWorld, 0.005, 0.6, 0.01);
    bulletWorld->addChild(border2);
    border2->createAABBCollisionDetector(toolRadius);
    border2->setMaterial(matBase);
    border2->buildDynamicModel();
    border2->setLocalPos(-0.3, 0.0, -0.2);

    border3 = new cBulletBox(bulletWorld, 0.6, 0.005, 0.01);
    bulletWorld->addChild(border3);
    border3->createAABBCollisionDetector(toolRadius);
    border3->setMaterial(matBase);
    border3->buildDynamicModel();
    border3->setLocalPos(0.0, 0.3, -0.2);

    border4 = new cBulletBox(bulletWorld, 0.6, 0.005, 0.01);
    bulletWorld->addChild(border4);
    border4->createAABBCollisionDetector(toolRadius);
    border4->setMaterial(matBase);
    border4->buildDynamicModel();
    border4->setLocalPos(0.0, -0.3, -0.2);

    //-----------------------------------------------------------------------
    // SETUP GROUND AND TOOL
    //-----------------------------------------------------------------------
    // Read the scale factor between the physical workspace of the haptic
    // device and the virtual workspace defined for the tool
    // Setting the ground
    cMaterial matGround;
    matGround.setStiffness(1);
    matGround.setDynamicFriction(0.10);
    matGround.setStaticFriction(0.20);
    matGround.setBlueRoyal();
    matGround.m_emission.setGrayLevel(0.3);
    bulletGround->setMaterial(matGround);
    bulletGround->createAABBCollisionDetector(toolRadius);

    controlSphere = new cShapeSphere(toolRadius);
    bulletWorld->addChild(controlSphere);
    controlSphere->setLocalPos(0.0,0.0, -0.15);
    controlSphere->m_material->setWhite();

    cMaterial mat;
    mat.setRedIndian();
    mat.m_specular.set(0.0, 0.0, 0.0);
    mat.setStiffness(1000);
    mat.setDynamicFriction(0.9);
    mat.setStaticFriction(0.9);

    movingSphere = new cBulletSphere(bulletWorld, toolRadius);
    bulletWorld->addChild(movingSphere);
    movingSphere->createAABBCollisionDetector(toolRadius);
    movingSphere->setMaterial(mat);
    movingSphere->setMass(SPHERE_MASS);
    movingSphere->buildDynamicModel();
    movingSphere->setLocalPos(0.0,0.0,0.0);
    
    // Set gravity.
    bulletWorld->setGravity(cVector3d(0.0, 0.0, 0.0));
}

void cGenericScene::updateGraphics(int a_width, int a_height){

    bulletWorld->updateShadowMaps(false, mirroredDisplay);
    camera->renderView(a_width, a_height);

}

void cGenericScene::updateHaptics(double timeInterval){

    bulletWorld->computeGlobalPositions(true);

    cVector3d hapticDevicePosition;
    cVector3d positionSphere = movingSphere->getLocalPos();
    hapticDevice->getPosition(hapticDevicePosition);

    
    cVector3d dir01 = cNormalize(hapticDevicePosition-positionSphere);
    double distance = cDistance(hapticDevicePosition, positionSphere);
    cVector3d sphereForce = (K_SPRING * (distance) * dir01);
    cVector3d stringForce = sphereForce;
    if (positionSphere.z() < -0.2)
        {
            sphereForce.add(cVector3d(0.0, 0.0, SPHERE_STIFFNESS * (-0.2 - positionSphere.z())));
        }
    cVector3d sphereAcc = (sphereForce / SPHERE_MASS) + cVector3d(0.0, 0.0, -9.8);
    sphereVel = K_DAMPING * (sphereVel + timeInterval * sphereAcc);
    cVector3d spherePos = movingSphere->getLocalPos() + timeInterval * sphereVel + cSqr(timeInterval) * sphereAcc;
    movingSphere->setLocalPos(spherePos); 

    double time = simClock.getCurrentTimeSeconds();
    double nextSimInterval = cClamp(time, 0.0001, 0.001);

    // Reset Clock
    simClock.reset();
    simClock.start();
    bulletWorld->updateDynamics(nextSimInterval);

    
}

void cGenericScene::init(){
    camera->set(cVector3d(0.50, 0.00, 0.40),    
                cVector3d(0.00, 0.00, -0.20),   
                cVector3d(0.00, 0.00, 1.00));
    movingSphere->setLocalPos(0.0,0.0,0.2);
}


