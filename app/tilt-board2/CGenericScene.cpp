#include <thread>
#include <chrono>
#include <unistd.h>
#include <algorithm>
#include <random>
#include "CGenericScene.h"
#include "CBullet.h"


using namespace std;

const double SPHERE_MASS        = 0.04;
const double SPHERE_STIFFNESS   = 200.0;
const double K_DAMPING          = 0.9999999999999999;
const double K_SPRING           = 80.0;
const double K_SPRING_TARGET    = 20.0;
const double HAPTIC_STIFFNESS   = 1.0;
const double WALL_GROUND = -0.2;


const double MAX_HAPTIC_FORCE = 5;

bool flagHapticsEnabled = false;
double hapticDeviceMaxStiffness;





void cGenericScene::updateTarget(){
    destination_index++;
    cout<< "updated target" << endl;
    if( destination_index == destinations.size() ){
        cout << "finished" << endl;
    }
    target->setLocalPos(destinations[shuffled_order[destination_index]]);
}






cGenericScene::cGenericScene(shared_ptr<cGenericHapticDevice> a_hapticDevice)
{   
    
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

    bulletCeilling = new cBulletStaticPlane(bulletWorld,  cVector3d(0.0, 0.0, -1.0),-planeWidth);
    bulletWorld->addChild(bulletCeilling);
    cCreatePlane(bulletCeilling, 0.6, 0.6, bulletCeilling->getPlaneConstant() * bulletCeilling->getPlaneNormal()+cVector3d(0.0,0.0, 0.01));
    bulletCeilling->createAABBCollisionDetector(toolRadius);
    bulletCeilling->setEnabled(false);

    //-----------------------------------------------------------------------
    // Widget
    //-----------------------------------------------------------------------    
    cFontPtr font = NEW_CFONTCALIBRI20();

    labelHapticDeviceModel = new cLabel(font);
    camera->m_frontLayer->addChild(labelHapticDeviceModel);
    cHapticDeviceInfo hapticDeviceInfo = a_hapticDevice->getSpecifications();
    labelHapticDeviceModel->setText(hapticDeviceInfo.m_modelName);
    
    // Create a label to display the position of haptic device
    labelHapticDevicePosition = new cLabel(font);
    camera->m_frontLayer->addChild(labelHapticDevicePosition);

    // Create a label to display the haptic and graphic rate of the simulation
    labelRates = new cLabel(font);
    labelRates->m_fontColor.setWhite();
    camera->m_frontLayer->addChild(labelRates);

    // Create bar
    controlLevel = new cLevel();
    camera->m_frontLayer->addChild(controlLevel);
    controlLevel->setLocalPos(20, 60);
    controlLevel->setRange(0.0, 1.0);
    controlLevel->setWidth(40);
    controlLevel->setNumIncrements(46);
    controlLevel->setSingleIncrementDisplay(false);
    controlLevel->setTransparencyLevel(0.5);

    hapticDevice = a_hapticDevice;
    mirroredDisplay = false;
    toolRadius = 0.0025;

    //-----------------------------------------------------------------------
    // Borders
    //-----------------------------------------------------------------------
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(10);
    matBase.setDynamicFriction(0.2);
    matBase.setStaticFriction(0.1);

    border1 = new cBulletBox(bulletWorld, 0.005, 0.6, 0.02);
    bulletWorld->addChild(border1);
    border1->createAABBCollisionDetector(toolRadius);
    border1->setMaterial(matBase);
    border1->buildDynamicModel();
    border1->setLocalPos(0.3, 0.0, -0.2+0.01);
    border1->m_bulletRigidBody->setCollisionFlags(border1->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    border1->m_bulletRigidBody->setUserPointer(border1);

    border2 = new cBulletBox(bulletWorld, 0.005, 0.6, 0.02);
    bulletWorld->addChild(border2);
    border2->createAABBCollisionDetector(toolRadius);
    border2->setMaterial(matBase);
    border2->buildDynamicModel();
    border2->setLocalPos(-0.3, 0.0, -0.2);
    border2->m_bulletRigidBody->setCollisionFlags(border2->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    border2->m_bulletRigidBody->setUserPointer(border2);

    border3 = new cBulletBox(bulletWorld, 0.6, 0.005, 0.02);
    bulletWorld->addChild(border3);
    border3->createAABBCollisionDetector(toolRadius);
    border3->setMaterial(matBase);
    border3->buildDynamicModel();
    border3->setLocalPos(0.0, 0.3, -0.2);
    border3->m_bulletRigidBody->setCollisionFlags(border3->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    border3->m_bulletRigidBody->setUserPointer(border3);

    border4 = new cBulletBox(bulletWorld, 0.6, 0.005, 0.02);
    bulletWorld->addChild(border4);
    border4->createAABBCollisionDetector(toolRadius);
    border4->setMaterial(matBase);
    border4->buildDynamicModel();
    border4->setLocalPos(0.0, -0.3, -0.2);
    border4->m_bulletRigidBody->setCollisionFlags(border4->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    border4->m_bulletRigidBody->setUserPointer(border4);

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
    controlSphere->setLocalPos(0.0,0.0, -0.20);
    controlSphere->m_material->setWhite();
    controlSphere->setEnabled(false);

    cMaterial mat;
    mat.setRedIndian();
    mat.m_specular.set(0.0, 0.0, 0.0);
    mat.setStiffness(1);
    mat.setDynamicFriction(0.9);
    mat.setStaticFriction(0.9);

    guidanceSphere = new cBulletSphere(bulletWorld, toolRadius);
    bulletWorld->addChild(guidanceSphere);
    guidanceSphere->createAABBCollisionDetector(toolRadius);
    guidanceSphere->setMaterial(mat);
    guidanceSphere->setMass(SPHERE_MASS);
    guidanceSphere->buildDynamicModel();
    guidanceSphere->setLocalPos(0.01,0.01,-0.2+toolRadius);
    guidanceSphere->setDamping(K_DAMPING, K_DAMPING);
    guidanceSphere->m_material->setBlue();
    guidanceSphere->setEnabled(false);
    guidanceSphere->m_bulletRigidBody->setUserPointer(guidanceSphere);
    guidanceSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border1->m_bulletRigidBody, true);
    guidanceSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border2->m_bulletRigidBody, true);
    guidanceSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border3->m_bulletRigidBody, true);
    guidanceSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border4->m_bulletRigidBody, true);

    negotiatedSphere = new cBulletSphere(bulletWorld, toolRadius);
    bulletWorld->addChild(negotiatedSphere);
    negotiatedSphere->createAABBCollisionDetector(toolRadius);
    negotiatedSphere->setMaterial(mat);
    negotiatedSphere->setMass(SPHERE_MASS);
    negotiatedSphere->buildDynamicModel();
    negotiatedSphere->setLocalPos(0.0,0.0,-0.2+toolRadius);
    negotiatedSphere->setDamping(K_DAMPING, K_DAMPING);
    negotiatedSphere->setEnabled(false);
    negotiatedSphere->m_bulletRigidBody->setUserPointer(negotiatedSphere);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border1->m_bulletRigidBody, true);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border2->m_bulletRigidBody, true);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border3->m_bulletRigidBody, true);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(border4->m_bulletRigidBody, true);
    negotiatedSphere->m_material->setYellow();
    //guidanceSphere->m_bulletRigidBody->setCollisionFlags(guidanceSphere->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    mainSphere = new cBulletSphere(bulletWorld, toolRadius);
    bulletWorld->addChild(mainSphere);
    mainSphere->createAABBCollisionDetector(toolRadius);
    mainSphere->setMaterial(mat);
    mainSphere->setMass(SPHERE_MASS);
    mainSphere->buildDynamicModel();
    mainSphere->setLocalPos(0.01,0.02,-0.2+toolRadius);
    mainSphere->setDamping(K_DAMPING, K_DAMPING);
    mainSphere->m_bulletRigidBody->setUserPointer(mainSphere);
    mainSphere->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    mainSphere->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);
    mainSphere->setInertia(cVector3d(0,0,0));

    // Set gravity.
    bulletWorld->setGravity(cVector3d(0.0, 0.0, -9.0));

    // retrieve the highest stiffness this device can render
    hapticDeviceMaxStiffness = hapticDeviceInfo.m_maxLinearStiffness;

    //initial the starting point for robot guidance.
    waypoints.push_back(cVector3d(0, 0.0,-0.2));
    waypointsRange.push_back(0.01);
    positionWaypoint = waypoints[0];
}



void cGenericScene::updateWaypoint(cVector3d positionSphere, cVector3d positionTarget){
    if(waypoint_index == last_waypoint_index)
    {
        waypoint_index = last_waypoint_index +1;
        return;
    }

    for(int i=max(last_waypoint_index-1,0); i < min(last_waypoint_index+2, (int)waypoints.size()); i++){
        if(cDistance(positionSphere, waypoints[i])/cDistance(waypoints[last_waypoint_index], waypoints[i])+0.7<cDistance(positionSphere, waypoints[waypoint_index])/cDistance(waypoints[last_waypoint_index], waypoints[waypoint_index])){
            waypoint_index = i;
        }
    }

    if(waypoint_index == waypoints.size()-1){
        updateTarget();
        generateWaypoints(positionSphere, target->getLocalPos());
    }

    waypoint_index = waypoint_index % waypoints.size();
    positionWaypoint = waypoints[waypoint_index];
}

void cGenericScene::updateGraphics(int a_width, int a_height){

    bulletWorld->updateShadowMaps(false, mirroredDisplay);
    camera->renderView(a_width, a_height);

    controlLevel->setValue(1-ALPHA_CONTROL);

}

void cGenericScene::updateHaptics(double timeInterval){
    bulletWorld->computeGlobalPositions(true);
     
    // Compute World Dynamics Forces.
    cVector3d hapticDevicePosition;
    cVector3d positionTarget = target->getLocalPos();
    cVector3d positionNegotiatedSphere = negotiatedSphere->getLocalPos();
    cVector3d positionMainSphere = mainSphere->getLocalPos();
    cVector3d positionGuidanceSphere = guidanceSphere->getLocalPos();
    
    hapticDevice->getPosition(hapticDevicePosition); 
    hapticDevicePosition = hapticDevicePosition *10;

    cVector3d wallForce;
    wallForce.add(cVector3d(0.0, 0.0, 50 * (WALL_GROUND + toolRadius- hapticDevicePosition.z())));
    hapticDevicePosition.z(WALL_GROUND+toolRadius);
    
    // Compute the varies forces.
    // The force of negotiatedSphere towards controlSphere and the force feedback for human operator.
    cVector3d dir01 = cNormalize(hapticDevicePosition-positionNegotiatedSphere);
    double distance01 = cDistance(hapticDevicePosition, positionNegotiatedSphere);
    cVector3d sphereForce = (K_SPRING*(ALPHA_CONTROL) * (distance01) * dir01);
    if((K_SPRING*(0.5) * (distance01) * dir01).length()<1.0){
        userInactive = true;
    }else{
        userInactive = false;
    }
    cVector3d hapticForce = -(K_SPRING*(0.5) * (distance01) * dir01) + wallForce;

    // The force of negotiatedSphere towards guidanceSphere. 
    cVector3d dir02 = cNormalize(positionGuidanceSphere-positionNegotiatedSphere);
    double distance02 = cDistance(positionGuidanceSphere, positionNegotiatedSphere);
    cVector3d guidanceForce = (K_SPRING*(1-ALPHA_CONTROL) * (distance02) * dir02);

    // The force of guidanceSphere towards waypoint.
    cVector3d dir03 = cNormalize(positionWaypoint-positionGuidanceSphere);
    double distance03 = cDistance(positionWaypoint, positionGuidanceSphere);
    cVector3d velocity03 = cVector3d(guidanceSphere->m_bulletRigidBody->getLinearVelocity().x(),guidanceSphere->m_bulletRigidBody->getLinearVelocity().y(), guidanceSphere->m_bulletRigidBody->getLinearVelocity().z());
    cVector3d targetForce = (K_SPRING_TARGET * (distance03) * dir03) - K_DAMPING_VELOCITY * velocity03.length() * dir03;

    // The force of mainSphere towards negotiatedSphere.
    cVector3d dir04 = cNormalize(positionNegotiatedSphere-positionMainSphere);
    double distance04 = cDistance(positionNegotiatedSphere, positionMainSphere);
    cVector3d velocity04 = cVector3d(mainSphere->m_bulletRigidBody->getLinearVelocity().x(),mainSphere->m_bulletRigidBody->getLinearVelocity().y(), mainSphere->m_bulletRigidBody->getLinearVelocity().z());
    cVector3d negotiatedForce = ((K_SPRING) * (distance04) * dir04) - K_DAMPING_VELOCITY * velocity04.length() * dir04;

    cVector3d finalForce = guidanceForce + sphereForce;

    if(cDistance(positionGuidanceSphere, positionNegotiatedSphere) > 0.15){
        targetForce =  -(K_SPRING*0.5 * (distance02) * dir02);
    }

    if(cDistance(positionMainSphere, positionNegotiatedSphere) > 0.15){
        finalForce -= negotiatedForce;
    }

    if(cDistance(positionMainSphere, waypoints[waypoint_index]) < waypointsRange[waypoint_index]){
        last_waypoint_index = waypoint_index;
    }
   
    updateWaypoint(positionMainSphere, target->getLocalPos());

    sphereForce.z(0);
    negotiatedSphere->addExternalForce(finalForce);
    guidanceSphere->addExternalForce(targetForce);
    mainSphere->addExternalForce(negotiatedForce);
    controlSphere->setLocalPos(hapticDevicePosition); 

    double time = simClock.getCurrentTimeSeconds();
    double nextSimInterval = cClamp(time, 0.0001, 0.001);

    /////////////////////////////////////////////////////////////////////////
    // APPLY FORCES
    /////////////////////////////////////////////////////////////////////////
    double stiffnessRatio = 1.0;
    
    if (hapticDeviceMaxStiffness < HAPTIC_STIFFNESS)
        stiffnessRatio = hapticDeviceMaxStiffness / HAPTIC_STIFFNESS;
    // Compute Force Feedback
    hapticForce = hapticForce * stiffnessRatio * 0.2;

    // Safety fuse.
    if(hapticForce.length()> MAX_HAPTIC_FORCE){
        hapticForce = cNormalize(hapticForce) * MAX_HAPTIC_FORCE;
    }
    hapticDevice->setForce(hapticForce);
  

    // Reset Clock
    simClock.reset();
    simClock.start();
    bulletWorld->updateDynamics(nextSimInterval);

    
}

void cGenericScene::init(){
    // Reset the camera, destination,
    camera->set(cVector3d(0.50, 0.00, 0.40),    
                cVector3d(0.00, 0.00, -0.20),   
                cVector3d(0.00, 0.00, 1.00));
    negotiatedSphere->setLocalPos(0.0,0.0,-0.2+toolRadius);
    destination_index = 0;
    

    // randomise the sequence of target position.
    auto rng = std::default_random_engine {};
    for(int i = 0; i < destinations.size(); i++){
        shuffled_order.push_back(i);
    }
    std::shuffle(std::begin(shuffled_order), std::end(shuffled_order), rng);
    target->setLocalPos(destinations[shuffled_order[destination_index]]);

    // Reset the position of spheres.
    negotiatedSphere->setLocalPos(0);
    controlSphere->setLocalPos(0.0,0.0, -0.20);
    negotiatedSphere->setLocalPos(0.0,0.0,-0.2+toolRadius);
    guidanceSphere->setLocalPos(0.02,0.02,-0.2+toolRadius);
    mainSphere->setLocalPos(0.01, 0.01, -0.2+toolRadius);
    waypoints.clear();
    waypointsRange.clear();
    

    // Generate the sequenece of waypooints and set the waypoint index based on the target position.
    generateWaypoints(mainSphere->getLocalPos(), target->getLocalPos());
    initWaypoints();
    
}


