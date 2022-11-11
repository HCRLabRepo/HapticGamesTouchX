#include "Scene2.h"
using namespace std;

Scene2::Scene2(shared_ptr<cGenericHapticDevice> a_hapticDevice):GenericScene(a_hapticDevice){
    
    cMaterial matBase;
    matBase.setGrayLevel(0.3);
    matBase.setStiffness(100);
    matBase.setDynamicFriction(0.5);
    matBase.setStaticFriction(0.9);

    // Setup static obstacles
    obstacle0 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle0);
    obstacle0->createAABBCollisionDetector(toolRadius);
    obstacle0->setMaterial(matBase);
    obstacle0->buildDynamicModel();
    obstacle0->setLocalPos(0.16, 0.18, -0.2);
    obstacle0->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));
    obstacle0->m_bulletRigidBody->setCollisionFlags(obstacle0->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle0->m_bulletRigidBody->setUserPointer(obstacle0);
    obstacle0->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle0->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);
    


    obstacle1 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle1);
    obstacle1->createAABBCollisionDetector(toolRadius);
    obstacle1->setMaterial(matBase);
    obstacle1->buildDynamicModel();
    obstacle1->setLocalPos(0.18, 0.16, -0.2);
    obstacle1->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));
    obstacle1->m_bulletRigidBody->setCollisionFlags(obstacle1->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle1->m_bulletRigidBody->setUserPointer(obstacle1);
    obstacle1->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle1->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    obstacle2 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle2);
    obstacle2->createAABBCollisionDetector(toolRadius);
    obstacle2->setMaterial(matBase);
    obstacle2->buildDynamicModel();
    obstacle2->setLocalPos(-0.18, -0.16, -0.2);
    obstacle2->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));
    obstacle2->m_bulletRigidBody->setCollisionFlags(obstacle2->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle2->m_bulletRigidBody->setUserPointer(obstacle2);
    obstacle2->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle2->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle3 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle3);
    obstacle3->createAABBCollisionDetector(toolRadius);
    obstacle3->setMaterial(matBase);
    obstacle3->buildDynamicModel();
    obstacle3->setLocalPos(-0.16, -0.18, -0.2);
    obstacle3->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(-45), C_EULER_ORDER_XYZ));
    obstacle3->m_bulletRigidBody->setCollisionFlags(obstacle3->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle3->m_bulletRigidBody->setUserPointer(obstacle3);
    obstacle3->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle3->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle4 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle4);
    obstacle4->createAABBCollisionDetector(toolRadius);
    obstacle4->setMaterial(matBase);
    obstacle4->buildDynamicModel();
    obstacle4->setLocalPos(0.18, -0.16, -0.2);
    obstacle4->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));
    obstacle4->m_bulletRigidBody->setCollisionFlags(obstacle4->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle4->m_bulletRigidBody->setUserPointer(obstacle4);
    obstacle4->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle4->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle5 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle5);
    obstacle5->createAABBCollisionDetector(toolRadius);
    obstacle5->setMaterial(matBase);
    obstacle5->buildDynamicModel();
    obstacle5->setLocalPos(0.16, -0.18, -0.2);
    obstacle5->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));
    obstacle5->m_bulletRigidBody->setCollisionFlags(obstacle5->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle5->m_bulletRigidBody->setUserPointer(obstacle5);
    obstacle5->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle5->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle6 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle6);
    obstacle6->createAABBCollisionDetector(toolRadius);
    obstacle6->setMaterial(matBase);
    obstacle6->buildDynamicModel();
    obstacle6->setLocalPos(-0.16, 0.18, -0.2);
    obstacle6->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));
    obstacle6->m_bulletRigidBody->setCollisionFlags(obstacle6->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle6->m_bulletRigidBody->setUserPointer(obstacle6);
    obstacle6->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle6->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);


    obstacle7 = new cBulletBox(bulletWorld, 0.005, 0.33, 0.015);
    bulletWorld->addChild(obstacle7);
    obstacle7->createAABBCollisionDetector(toolRadius);
    obstacle7->setMaterial(matBase);
    obstacle7->buildDynamicModel();
    obstacle7->setLocalPos(-0.18, 0.16, -0.2);
    obstacle7->setLocalRot(cMatrix3d(cDegToRad(0), cDegToRad(0), cDegToRad(45), C_EULER_ORDER_XYZ));
    obstacle7->m_bulletRigidBody->setCollisionFlags(obstacle7->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    obstacle7->m_bulletRigidBody->setUserPointer(obstacle7);
    obstacle7->m_bulletRigidBody->setIgnoreCollisionCheck(guidanceSphere->m_bulletRigidBody, true);
    obstacle7->m_bulletRigidBody->setIgnoreCollisionCheck(negotiatedSphere->m_bulletRigidBody, true);

    destinations.push_back(cVector3d(-0.28, 0.28, -0.2+0.00025));
    destinations.push_back(cVector3d(0.28, 0.28, -0.2+0.00025));
    destinations.push_back(cVector3d(-0.28, -0.28, -0.2+0.00025));
    destinations.push_back(cVector3d(0.28, -0.28, -0.2+0.00025));
    destinations.push_back(cVector3d(0, -0.25, -0.2+0.00025));
    destinations.push_back(cVector3d(0, 0.25, -0.2+0.00025));
    destinations.push_back(cVector3d(-0.25, 0.0, -0.2+0.00025));
    destinations.push_back(cVector3d(0.25, 0.0, -0.2+0.00025));

    checkpoints.push_back(cVector3d(-0.05, 0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.05, 0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(-0.05, -0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0.05, -0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.015);
    checkpoints.push_back(cVector3d(0, -0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.04);
    checkpoints.push_back(cVector3d(0, 0.05, -0.2+0.00025));
    checkpointsRange.push_back(0.04);
    checkpoints.push_back(cVector3d(-0.05, 0.0, -0.2+0.00025));
    checkpointsRange.push_back(0.04);
    checkpoints.push_back(cVector3d(0.05, 0.0, -0.2+0.00025));
    checkpointsRange.push_back(0.04);


    //middle point
    checkpoints.push_back(cVector3d(-0.00,-0.0, -0.20025));
    checkpointsRange.push_back(0.05);

    // set up target
    target = new cBulletCylinder(bulletWorld, 0.0005, toolRadius*1.5);
    bulletWorld->addChild(target);
    target->createAABBCollisionDetector(toolRadius);
    target->setMaterial(matBase);
    target->buildDynamicModel();
    target->m_material->setGreen();
    target->m_bulletRigidBody->setCollisionFlags(target->m_bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    target->m_bulletRigidBody->setUserPointer(target);

    //set up fuzzy engine
    engineSetup();

}

void Scene2::setStiffness(double a_stiffness){
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

void Scene2::generateWaypoints(cVector3d positionSphere, cVector3d positionTarget){
    for(int i = 0; i < destinations.size();i++){
        if(positionTarget.equals(destinations[i]))
        {
            waypoints.push_back(checkpoints[i]);
            waypointsRange.push_back(checkpointsRange[i]);
            waypoints.push_back(destinations[i]);
            waypointsRange.push_back(0.005);
            waypoints.push_back(checkpoints[i]);
            waypointsRange.push_back(checkpointsRange[i]);
            break;
        }
       
    }
}
void Scene2::updateWaypoint(cVector3d positionSphere, cVector3d positionTarget){
    if(waypoint_index == waypoints.size()-3){
        if(waypoint_index == last_waypoint_index)
        {
            cout<< "waypoint upodated forward" << endl;
            waypoint_index = last_waypoint_index +1;
            return;
        }else if(cDistance(positionTarget,positionSphere) + cDistance(waypoints[waypoint_index], positionSphere) > cDistance(waypoints[waypoint_index],positionTarget)+0.03){
            double min_distance =  cDistance(waypoints[waypoint_index], positionSphere);
            for(int i = 4; i < checkpoints.size(); i++){
                if(cDistance(checkpoints[i], positionSphere)< min_distance){
                    min_distance = cDistance(checkpoints[i], positionSphere);
                    positionWaypoint = checkpoints[i];
                }
            }
        }
        if(cDistance(positionWaypoint,positionSphere)<0.04){
            positionWaypoint = waypoints[waypoint_index];
        }
    }
    else if(waypoint_index == waypoints.size()-2){
         if(waypoint_index == last_waypoint_index)
        {
            cout<< "waypoint upodated forward" << endl;
            waypoint_index = last_waypoint_index +1;
            return;

        }else if(cDistance(positionTarget,positionSphere) + cDistance(waypoints[waypoint_index-1], positionSphere) > cDistance(waypoints[waypoint_index-1],positionTarget)+0.09)
        {
            last_waypoint_index = last_waypoint_index -1;
            waypoint_index = waypoint_index-1;
        }
        positionWaypoint = waypoints[waypoint_index];            
    }
    else
    {
        if(waypoint_index == last_waypoint_index)
        {
            cout<< "waypoint upodated forward" << endl;
            waypoint_index = last_waypoint_index +1;
        }

        for(int i=max(last_waypoint_index-1,0); i < min(last_waypoint_index+2, (int)waypoints.size()); i++){
            if(cDistance(positionSphere, waypoints[i])/cDistance(waypoints[last_waypoint_index], waypoints[i])+0.7<cDistance(positionSphere, waypoints[waypoint_index])/cDistance(waypoints[last_waypoint_index], waypoints[waypoint_index])){
                cout << "waypoint updated backward" << endl;
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
}

void Scene2::engineSetup() {
    using namespace fl;

    InputVariable* time = new InputVariable;
    time->setName("time");
    time->setDescription("");
    time->setEnabled(true);
    time->setRange(0.000, 120.000);
    time->setLockValueInRange(false);
    time->addTerm(new Bell("acceptable", 0.000, 40.000, 5.000));
    time->addTerm(new Gaussian("slow", 70.000, 15.000));
    time->addTerm(new Bell("veryslow", 120.000, 30.000, 5.000));
    engine->addInputVariable(time);

    InputVariable* collisions = new InputVariable;
    collisions->setName("collisions");
    collisions->setDescription("");
    collisions->setEnabled(true);
    collisions->setRange(0.000, 5000.000);
    collisions->setLockValueInRange(false);
    collisions->addTerm(new Trapezoid("flawless", 0.000, 0.000, 500.000, 1000.000));
    collisions->addTerm(new Trapezoid("good", 0.000, 1000.000, 1500.000, 2000.000));
    collisions->addTerm(new Trapezoid("OK", 1000.000, 2000.000, 2500.000, 3000.000));
    collisions->addTerm(new Trapezoid("bad", 2000.000, 3000.000, 3500.000, 4000.000));
    collisions->addTerm(new Trapezoid("terrible", 3500.000, 4000.000, 5000.000, 99999.000));
    engine->addInputVariable(collisions);

    OutputVariable* alphachange = new OutputVariable;
    alphachange->setName("alphachange");
    alphachange->setDescription("");
    alphachange->setEnabled(true);
    alphachange->setRange(-1.000, 1.000);
    alphachange->setLockValueInRange(false);
    alphachange->setAggregation(new Maximum);
    alphachange->setDefuzzifier(new Centroid(100));
    alphachange->setDefaultValue(fl::nan);
    alphachange->setLockPreviousValue(false);
    alphachange->addTerm(new Trapezoid("greatdeduction", -1.000, -1.000, -0.800, -0.600));
    alphachange->addTerm(new Trapezoid("smalldeduction", -0.800, -0.600, -0.400, -0.200));
    alphachange->addTerm(new Gaussian("menialchange", 0.000, 0.100));
    alphachange->addTerm(new Trapezoid("smallincrease", 0.200, 0.400, 0.600, 0.800));
    alphachange->addTerm(new Trapezoid("greatincrease", 0.600, 0.800, 1.000, 1.000));
    engine->addOutputVariable(alphachange);

    RuleBlock* ruleBlock = new RuleBlock;
    ruleBlock->setName("");
    ruleBlock->setDescription("");
    ruleBlock->setEnabled(true);
    ruleBlock->setConjunction(new Minimum);
    ruleBlock->setDisjunction(new Maximum);
    ruleBlock->setImplication(new Minimum);
    ruleBlock->setActivation(new General);
    ruleBlock->addRule(Rule::parse("if time is acceptable and collisions is flawless then alphachange is greatincrease", engine));
    ruleBlock->addRule(Rule::parse("if time is acceptable and collisions is good then alphachange is smallincrease", engine));
    ruleBlock->addRule(Rule::parse("if time is acceptable and collisions is OK then alphachange is menialchange", engine));
    ruleBlock->addRule(Rule::parse("if time is acceptable and collisions is bad then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is acceptable and collisions is terrible then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is slow and collisions is flawless then alphachange is smallincrease", engine));
    ruleBlock->addRule(Rule::parse("if time is slow and collisions is good then alphachange is menialchange", engine));
    ruleBlock->addRule(Rule::parse("if time is slow and collisions is OK then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is slow and collisions is bad then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is slow and collisions is terrible then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is veryslow and collisions is flawless then alphachange is menialchange", engine));
    ruleBlock->addRule(Rule::parse("if time is veryslow and collisions is good then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is veryslow and collisions is OK then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is veryslow and collisions is bad then alphachange is smalldeduction", engine));
    ruleBlock->addRule(Rule::parse("if time is veryslow and collisions is terrible then alphachange is greatdeduction", engine));
    engine->addRuleBlock(ruleBlock);
}