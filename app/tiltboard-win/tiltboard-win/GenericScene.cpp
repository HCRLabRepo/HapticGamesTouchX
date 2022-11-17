#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include "GenericScene.h"
#include "CBullet.h"



using namespace std;

const double SPHERE_MASS        = 0.04;
const double SPHERE_STIFFNESS   = 200.0;
const double K_DAMPING          = 0.9999999999999999;
const double K_SPRING           = 80.0;
const double K_SPRING_TARGET    = 20.0;
const double HAPTIC_STIFFNESS   = 1.0;
const double WALL_GROUND = -0.2;

double MAX_HAPTIC_FORCE = 4;

bool flagHapticsEnabled = false;
double hapticDeviceMaxStiffness;





void GenericScene::updateTarget(){
    destination_index++;
    if (fuzzyControl) {
        time_t timetaken = difftime(time(0), startTime);
        cout << "Time: " << timetaken << " Collisions: " << collisionNum << endl;
        double result = getFuzzyOutput(timetaken, collisionNum);
        cout << "Change in alpha: " << result << endl;
        ALPHA_CONTROL += result;
        ALPHA_CONTROL = min(1.0, ALPHA_CONTROL);
        ALPHA_CONTROL = max(0.0, ALPHA_CONTROL);
        currentAlpha = ALPHA_CONTROL;
    }
    cout<< "updated target" << endl;
    if( destination_index == destinations.size() ){
        cout << "finished" << endl;
    }
    startTime = time(0);
    collisionNum = 0;
    target->setLocalPos(destinations[shuffled_order[destination_index]]);
}






GenericScene::GenericScene(shared_ptr<cGenericHapticDevice> a_hapticDevice)
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
    if(hapticDeviceInfo.m_modelName == "Touch X"){
        MAX_HAPTIC_FORCE = 4;
    }else{
        MAX_HAPTIC_FORCE = 2;
    }
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

    // Create a label to display the time taken since the last waypoint
    labelTime = new cLabel(font);
    labelTime->m_fontColor.setWhite();
    camera->m_frontLayer->addChild(labelTime);

    //Create a lebel to display the number of collisions since last checkpoint
    labelCollisions = new cLabel(font);
    labelCollisions->m_fontColor.setWhite();
    camera->m_frontLayer->addChild(labelCollisions);

    hapticDevice = a_hapticDevice;
    mirroredDisplay = false;
    toolRadius = 0.0025;

    //-----------------------------------------------------------------------
    // Borders
    //-----------------------------------------------------------------------
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(10);
    matBase.setDynamicFriction(0.5);
    matBase.setStaticFriction(0.9);

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

    borderSetup({0.005, 0.6, 0.02}, {0.3, 0.0, -0.2}, { 0,0,0 }, matBase);
    borderSetup({0.005, 0.6, 0.02}, {-0.3, 0.0, -0.2 }, { 0,0,0 }, matBase);
    borderSetup({0.6, 0.005, 0.02}, {0.0, 0.3, -0.2}, { 0,0,0 }, matBase);
    borderSetup({0.6, 0.005, 0.02}, {0.0, -0.3, -0.2 }, { 0,0,0 }, matBase);

    // Set gravity.
    bulletWorld->setGravity(cVector3d(0.0, 0.0, -9.0));

    // retrieve the highest stiffness this device can render
    hapticDeviceMaxStiffness = hapticDeviceInfo.m_maxLinearStiffness;

    //initial the starting point for robot guidance.
    waypoints.push_back(cVector3d(0, 0.0,-0.2));
    waypointsRange.push_back(0.01);
    positionWaypoint = waypoints[0];

    //Initialise the engine for fuzzy logic
    engine = new fl::Engine();
    engine->setName("Fuzzy Control Engine");
    engine->setDescription("");

    startTime = time(0);
}

void GenericScene::borderSetup(std::vector<double> size, std::vector<double> pos, std::vector<double> rot,cMaterial matBase) {
    cBulletBox* obstacle = new cBulletBox(bulletWorld, size.at(0), size.at(1), size.at(2));
    bulletWorld->addChild(obstacle);
    obstacle->createAABBCollisionDetector(toolRadius);
    obstacle->setMaterial(matBase);
    obstacle->buildDynamicModel();
    obstacle->setLocalPos(pos.at(0), pos.at(1), pos.at(2));
    obstacle->setLocalRot(cMatrix3d(cDegToRad(rot.at(0)), cDegToRad(rot.at(1)), cDegToRad(rot.at(2)), C_EULER_ORDER_XYZ));
    obstacle->m_bulletRigidBody->setCollisionFlags(obstacle->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle->m_bulletRigidBody->setUserPointer(obstacle);
    negotiatedSphere->m_bulletRigidBody->setIgnoreCollisionCheck(obstacle->m_bulletRigidBody, true);
    guidanceSphere->m_bulletRigidBody->setIgnoreCollisionCheck(obstacle->m_bulletRigidBody, true);
}

void GenericScene::updateWaypoint(cVector3d positionSphere, cVector3d positionTarget){
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

void GenericScene::updateGraphics(int a_width, int a_height){

    bulletWorld->updateShadowMaps(false, mirroredDisplay);
    camera->renderView(a_width, a_height);

    controlLevel->setValue(1-ALPHA_CONTROL);

}

void GenericScene::updateHaptics(double timeInterval){
    bulletWorld->computeGlobalPositions(true);
     
    // Compute World Dynamics Forces.
    cVector3d hapticDevicePosition;
    cVector3d positionTarget = target->getLocalPos();
    positionNegotiatedSphere = negotiatedSphere->getLocalPos();
    positionMainSphere = mainSphere->getLocalPos();
    positionGuidanceSphere = guidanceSphere->getLocalPos();
    
    hapticDevice->getPosition(hapticDevicePosition); 
    hapticDevicePosition = hapticDevicePosition *10;

    cVector3d wallForce;
    wallForce.add(cVector3d(0.0, 0.0, 50 * (WALL_GROUND + toolRadius- hapticDevicePosition.z())));
    hapticDevicePosition.z(WALL_GROUND+toolRadius);
    
    // Compute the varies forces.
    // The force of negotiatedSphere towards controlSphere and the force feedback for human operator.
    cVector3d dir01 = cNormalize(hapticDevicePosition-positionNegotiatedSphere);
    double distance01 = cDistance(hapticDevicePosition, positionNegotiatedSphere);
    sphereForce = (K_SPRING*(ALPHA_CONTROL) * (distance01) * dir01);
    userForce = (K_SPRING*(0.5) * (distance01) * dir01).length();
    if(userForce<1.0){
        userInactive = true;
    }else{
        userInactive = false;
    }
    cVector3d hapticForce = -(K_SPRING*(0.5) * (distance01) * dir01) + wallForce;

    // The force of negotiatedSphere towards guidanceSphere. 
    cVector3d dir02 = cNormalize(positionGuidanceSphere-positionNegotiatedSphere);
    double distance02 = cDistance(positionGuidanceSphere, positionNegotiatedSphere);
    guidanceForce = (K_SPRING*(1-ALPHA_CONTROL) * (distance02) * dir02);

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
    hapticForce = hapticForce * stiffnessRatio * 0.3;

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

void GenericScene::init(){
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

double GenericScene::getFuzzyOutput(int timein, int collisionsin) {
    engine->inputVariables()[0]->setValue(timein);
    engine->inputVariables()[1]->setValue(collisionsin);
    engine->process();
    double output = stod(fl::Op::str(engine->outputVariables()[0]->getValue()));
    return output;
}

