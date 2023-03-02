#include <chrono>
#include <thread>
#include <string>
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif
#include "tilt-board2.h"
using namespace chai3d;
using namespace std;



uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void mySleep(int sleepMs)
{
    #ifdef LINUX
        usleep(sleepMs * 1000);
    #endif
    #ifdef WINDOWS
        Sleep(sleepMs);
    #endif
}

void recoverColor(const btCollisionObjectWrapper* obj1)
{   mySleep(500);
    ((cBulletMesh*)(obj1->getCollisionObject()->getUserPointer()))->m_material->setGrayLevel(0.3);

}
bool callbackFunc(btManifoldPoint& cp, const btCollisionObjectWrapper* obj1, int id1, int index1, const btCollisionObjectWrapper* obj2, int id2, int index2)
{   cColorf Red;
    cColorf Gray;
    cColorf Green;
    cColorf Blue;
    
    Red.setRed();
    Gray.setGrayLevel(0.3);
    Green.setGreen();
    Blue.setBlue();

    if( (((cBulletMesh*)(obj1->getCollisionObject()->getUserPointer()))->m_material->m_diffuse == Gray) && (((cBulletMesh*)(obj2->getCollisionObject()->getUserPointer()))->m_material->m_diffuse != Blue)){
        ((cBulletMesh*)(obj1->getCollisionObject()->getUserPointer()))->m_material->setRed();
        ((cBulletMesh*)(obj1->getCollisionObject()->getUserPointer()))->m_material->setGrayLevel(0.3);
        //std::thread t(recoverColor,obj1);
        ballfile << timeSinceEpochMillisec() << ", " << "Collision" <<endl;
        main_scene->collisionNum += 1;
        main_scene->collisionsLastSec += 1;
        //t.join();
    }
    if( (((cBulletMesh*)(obj2->getCollisionObject()->getUserPointer()))->m_material->m_diffuse == Gray) && (((cBulletMesh*)(obj1->getCollisionObject()->getUserPointer()))->m_material->m_diffuse != Blue) ){
        ((cBulletMesh*)(obj2->getCollisionObject()->getUserPointer()))->m_material->setRed();
        ((cBulletMesh*)(obj2->getCollisionObject()->getUserPointer()))->m_material->setGrayLevel(0.3);
        //std::thread t(recoverColor,obj2);
        ballfile << timeSinceEpochMillisec() << ", " << "Collision" <<endl;
        main_scene->collisionNum += 1;
        main_scene->collisionsLastSec += 1;
        //t.join();
    }

    return false;
}

int main(int argc, char* argv[]){
    gContactAddedCallback=callbackFunc;
    //---------------------------------------------------------------------------
    // Retreat Experiment Settings
    //---------------------------------------------------------------------------
    std::ifstream settingsfile("ExperimentSettings.txt");
    if (argc >= 2) {
        subject_num = argv[1];
        game_scene = atoi(argv[2]);
        control_mode = atoi(argv[3]);
        if (control_mode == 10) {
            fuzzy_params = argv[4];
            cout << "resources/fuzzy/" + fuzzy_params + ".fis" << endl;
            fuzzy = fl::FisImporter().fromFile("resources/fuzzy/" + fuzzy_params + ".fis");
        }
        else {
            fuzzy_params = "";
        }
    }
    else if(argc == 0 && settingsfile.is_open()){
        settingsfile >> subject_num;
        settingsfile >> game_scene;
        settingsfile >> control_mode;
        if (control_mode == 10) {
            settingsfile >> fuzzy_params;
            cout << "resources/fuzzy/" + fuzzy_params + ".fis" << endl;
            fuzzy = fl::FisImporter().fromFile("resources/fuzzy/" + fuzzy_params + ".fis");
        }

        settingsfile.close();
    }
    else{
        cout << "Settings File Not Found!" << endl;
        return 0;
    }
    string resultPath = "results/S" + subject_num + "/control_" + to_string(control_mode);
    string ballfilename = resultPath + fuzzy_params + "/ball.csv";
    string conductancefilename = resultPath + fuzzy_params + "/conductance.csv";
    string HIPfilename = resultPath + fuzzy_params + "/position_HIP.csv";
    string CIPfilename = resultPath + fuzzy_params + "/position_CIP.csv";
    string NIPfilename = resultPath + fuzzy_params + "/position_NIP.csv";
    string HIPforcefilename = resultPath + fuzzy_params + "/force_HIP.csv";
    string CIPforcefilename = resultPath + fuzzy_params + "/force_CIP.csv";
    //---------------------------------------------------------------------------
    // Initial Print Message
    //---------------------------------------------------------------------------
    cout << endl;
    cout << "-----------------------------------" << endl;
    cout << "Tilt Board" << endl;
    cout << "-----------------------------------" << endl << endl << endl;
    cout << "Keyboard Options:" << endl << endl;
    cout << "[0] - Select Debug Scene" << endl;
    cout << "[1] - Select Scene 1" << endl;
    cout << "[2] - Select Scene 2" << endl;
    cout << "[3] - Select Scene 3" << endl;
    cout << "[f] - Enable/Disable full screen mode" << endl;
    cout << "[m] - Enable/Disable vertical mirroring" << endl;
    cout << "[q] - Exit application" << endl;
    cout << endl << endl;
    //--------------------------------------------------------------------------
    // OPEN GL - WINDOW DISPLAY
    //--------------------------------------------------------------------------
    
    // Initialize GLFW library
    if(!glfwInit()){
        cout << "Failed Initialization" << endl;
        cSleepMs(1000);
        return 1;
    }
    glfwSetErrorCallback(errorCallback);
    // Compute desired size of window
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int w = 0.8 * mode->height;
    int h = 0.5 * mode->height;
    int x = 0.5 * (mode->width -w); 
    int y = 0.5 * (mode->height - h);

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    
    // Set active stereo mode
    if(stereoMode == C_STEREO_ACTIVE){
        glfwWindowHint(GLFW_STEREO, GL_TRUE);
    }else{
        glfwWindowHint(GLFW_STEREO, GL_FALSE);
    }

    // Create display context
    window = glfwCreateWindow(w, h, "CHAI3D", NULL, NULL);
    if(!window){
        cout << "Failed to create window" << endl;
        cSleepMs(1000);
        glfwTerminate();
        return 1;
    }

    glfwGetWindowSize(window, &width, &height);// Get width and heigth of window
    glfwSetWindowPos(window, x, y); // Set position of window
    glfwSetKeyCallback(window, keyCallback); // Set key callback
    glfwSetCursorPosCallback(window, mouseMotionCallback); // set mouse position callback
    glfwSetMouseButtonCallback(window, mouseButtonCallback);// set mouse button callback
    glfwSetScrollCallback(window, mouseScrollCallback);    // set mouse scroll callback
    glfwSetWindowSizeCallback(window, windowSizeCallback); // Set resize callback
    glfwMakeContextCurrent(window); // Set current display context
    glfwSwapInterval(swapInterval); // Sets the swap interval for the current display context

    // Initialize GLEW library
#ifdef GLEW_VERSION
    if(glewInit()!= GLEW_OK){
        cout << "failed to initialize GLEW library" << endl;
        glfwTerminate();
        return 1;
    }
#endif

    //-----------------------------------------------------------------------
    // HAPTIC DEVICES / TOOLS
    //-----------------------------------------------------------------------
    handler = new cHapticDeviceHandler();
    handler->getDevice(hapticDevice, 0); // Get access to the first available haptic device

    cHapticDeviceInfo hapticDeviceInfo = hapticDevice->getSpecifications();

    //-----------------------------------------------------------------------
    // SETUP BULLET WORLD AND OBJECTS
    //-----------------------------------------------------------------------

    debugScene = new DebugScene(hapticDevice);
    scene1 = new Scene1(hapticDevice);
    scene2 = new Scene2(hapticDevice);
    scene3 = new Scene3(hapticDevice);
    // Read the scale factor between the physical workspace of the haptic
    // device and the virtual workspace defined for the tool
    double maxStiffness = 1000;
    if(tool!=NULL)
    {
        double workspaceScaleFactor = tool->getWorkspaceScaleFactor();
        maxStiffness = cMin(maxStiffness,hapticDeviceInfo.m_maxLinearStiffness/workspaceScaleFactor);
    }
    // Stiffness properties
    debugScene->camera->setStereoMode(stereoMode);
    scene1->camera->setStereoMode(stereoMode);
    scene2->camera->setStereoMode(stereoMode);
    scene3->camera->setStereoMode(stereoMode);

    debugScene->setStiffness(maxStiffness);
    scene1->setStiffness(maxStiffness);
    scene2->setStiffness(maxStiffness);
    scene3->setStiffness(maxStiffness);

    //--------------------------------------------------------------------------
    // WIDGETS
    //--------------------------------------------------------------------------
    main_scene = new GenericScene(hapticDevice);
    
    if(game_scene==1){
        initScene1();
    }
    else if(game_scene == 2){
        initScene2();
    }
    else if(game_scene == 3){
        initScene3();
    }
    else if (game_scene == 0) {
        initDebugScene();
    }
    ballfile.open(ballfilename);
    conductancefile.open(conductancefilename);
    HIPfile.open(HIPfilename);
    CIPfile.open(CIPfilename);
    NIPfile.open(NIPfilename);
    HIPforcefile.open(HIPforcefilename);
    CIPforcefile.open(CIPforcefilename);
    s = new SensorData();
    //--------------------------------------------------------------------------
    // START SIMULATION
    //--------------------------------------------------------------------------
    
    // Create a thread which starts the main haptics rendering loop
    hapticsThread = new cThread();
    hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);
    sensorThread = new cThread();
    sensorThread->start(getSensorData, CTHREAD_PRIORITY_GRAPHICS);
    
    
    // Setup callback when application exits
    atexit(close);

    //--------------------------------------------------------------------------
    // MAIN GRAPHIC LOOP
    //--------------------------------------------------------------------------
    // Call window size callback at initialization
    windowSizeCallback(window, width, height);
    // Main graphic loop
    while(!glfwWindowShouldClose(window)){
        // Get width and height of window
        glfwGetWindowSize(window, &width, &height);
        updateGraphics();
        glfwSwapBuffers(window);
        glfwPollEvents();
        freqCounterGraphics.signal(1);
    }

    glfwDestroyWindow(window); // Close window
    glfwTerminate(); //terminate GLFW library
    
    


    return 0;
}

void initScene1(){
    main_scene = scene1;
    main_scene->init();
}

void initScene2(){
    main_scene = scene2;
    main_scene->init();
}

void initScene3(){
    main_scene = scene3;
    main_scene->init();
}

void initDebugScene(){
    main_scene = debugScene;
    debugScene->init();
}

void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    // filter calls that only include a key press
    if ((a_action != GLFW_PRESS) && (a_action != GLFW_REPEAT))
    {
        return;
    }

    else if((a_key == GLFW_KEY_ESCAPE) || (a_key == GLFW_KEY_Q))
    {
        glfwSetWindowShouldClose(a_window, GLFW_TRUE);
    }
    
    else if(a_key == GLFW_KEY_F)
    {
        fullscreen = !fullscreen;
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode  = glfwGetVideoMode(monitor);

        if(fullscreen)
        {
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            glfwSwapInterval(swapInterval);
        }
        else
        {
            int w = 0.8 * mode->height;
            int h = 0.5 * mode->height;
            int x = 0.5 * (mode->width - w);
            int y = 0.5 * (mode->height - h);
            glfwSetWindowMonitor(window, NULL, x, y, w, h, mode->refreshRate);
            glfwSwapInterval(swapInterval);
        }
    }

    else if (a_key == GLFW_KEY_M)
    {
        mirroredDisplay = !mirroredDisplay;
        scene1->camera->setMirrorVertical(mirroredDisplay);
        scene2->camera->setMirrorVertical(mirroredDisplay);
        scene3->camera->setMirrorVertical(mirroredDisplay);
        debugScene->camera->setMirrorVertical(mirroredDisplay);
        scene1->mirroredDisplay = mirroredDisplay;
        scene2->mirroredDisplay = mirroredDisplay;
        scene3->mirroredDisplay = mirroredDisplay;
        debugScene->mirroredDisplay = mirroredDisplay;
    }
    else if(a_key == GLFW_KEY_S){
        main_scene->controlSphere->setEnabled(!(main_scene->controlSphere->getEnabled()));
    }

    else if(a_key == GLFW_KEY_G){
        main_scene->guidanceSphere->setEnabled(!(main_scene->guidanceSphere->getEnabled()));
    }
    else if(a_key == GLFW_KEY_N){
        main_scene->negotiatedSphere->setEnabled(!(main_scene->negotiatedSphere->getEnabled()));
    }
    /*else if (a_key == GLFW_KEY_W) {
        main_scene->visualizeWaypoints();
    }*/
    else if (a_key == GLFW_KEY_1)
    {
        initScene1();
    }

    else if (a_key == GLFW_KEY_2)
    {
        initScene2();
    }
    
    else if (a_key == GLFW_KEY_3)
    {
        initScene3();
    }

    else if (a_key == GLFW_KEY_0)
    {
        initDebugScene();
    }
}

void close(void)
{
    // stop the simulation
    simulationRunning = false;

    // wait for graphics and haptics loops to terminate
    while (!simulationFinished) { cSleepMs(100); }

    // delete resources
    delete hapticsThread;
    delete sensorThread;
    delete scene1;
    delete scene2;
    delete scene3;
    delete debugScene;
    delete handler;
}

void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
    // update window size
    width = a_width;
    height = a_height;
    // update position of label
    main_scene->labelHapticDeviceModel->setLocalPos(20, height - 40, 0);
    // update position of label
    main_scene->labelHapticDevicePosition->setLocalPos(20, height - 60, 0);
}


void errorCallback(int a_error, const char* a_description)
{
    cout << "Error: " << a_description << endl;
}

void mouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
    if (a_button == GLFW_MOUSE_BUTTON_RIGHT && a_action == GLFW_PRESS)
    {
        // store mouse position
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // update mouse state
        mouseState = MOUSE_MOVE_CAMERA;
    }

    else
    {
        // update mouse state
        mouseState = MOUSE_IDLE;
    }
}

void mouseMotionCallback(GLFWwindow* a_window, double a_posX, double a_posY)
{
    if (mouseState == MOUSE_MOVE_CAMERA)
    {   
        
        // compute mouse motion
        int dx = a_posX - mouseX;
        int dy = a_posY - mouseY;
        mouseX = a_posX;
        mouseY = a_posY;
        
        // compute new camera angles
        double azimuthDeg = main_scene->camera->getSphericalAzimuthDeg() - 0.5 * dx;
        double polarDeg = main_scene->camera->getSphericalPolarDeg() - 0.5 * dy;
        
        // assign new angles
        main_scene->camera->setSphericalAzimuthDeg(azimuthDeg);
        main_scene->camera->setSphericalPolarDeg(polarDeg);

        // oriente tool with camera
        cVector3d cameraPos = main_scene->camera->getLocalPos();
        cameraPos.x(0);
    }
}

void mouseScrollCallback(GLFWwindow* a_window, double a_offsetX, double a_offsetY)
{
    double r = main_scene->camera->getSphericalRadius();
    r = cClamp(r + 0.1 * -a_offsetY, 0.5, 3.0);
    main_scene->camera->setSphericalRadius(r);
}

void updateGraphics(void){
    /////////////////////////////////////////////////////////////////////
    // UPDATE WIDGETS
    /////////////////////////////////////////////////////////////////////

    main_scene->labelHapticDeviceModel->setLocalPos(20, height - 40, 0);
    // update position of label
    main_scene->labelHapticDevicePosition->setLocalPos(20, height - 60, 0);

    //Update the text information.

    main_scene->labelHapticDevicePosition->setText(hapticDevicePosition.str(3));
    main_scene->labelRates->setText(cStr(freqCounterGraphics.getFrequency(), 0) + " Hz /" + 
                        cStr(freqCounterHaptics.getFrequency(),0) + " Hz");
    
    // update position of label
    main_scene->labelRates->setLocalPos((int)(0.5 * (width - main_scene->labelRates->getWidth())), 15);

    main_scene->labelTime->setText("Time since last waypoint: " + to_string(difftime(time(0), main_scene->startTime)));
    main_scene->labelTime->setLocalPos((int)(width - main_scene->labelTime->getWidth()), 15);

    main_scene->labelCollisions->setText("Collisions: "+ to_string(main_scene->collisionNum));
    main_scene->labelCollisions->setLocalPos((int)(width - main_scene->labelCollisions->getWidth()), 30);
    /////////////////////////////////////////////////////////////////////
    // RENDER SCENE
    /////////////////////////////////////////////////////////////////////

    // Update shadow maps
    main_scene->updateGraphics(width,height);

    // wait until all GL commands are completed
    glFinish();

    // check for any OpenGL errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) printf("Error:  %s\n", gluErrorString(err));
}

void updateHaptics(void){
    // Flags
    simulationRunning = true;
    simulationFinished = false;

    // Reset Clock
    cPrecisionClock clock;
    clock.reset();
    hapticDevice->open();
    hapticDevice->calibrate();

    

    while(simulationRunning){
        /////////////////////////////////////////////////////////////////////
        // SIMULATION TIME    
        /////////////////////////////////////////////////////////////////////
        hapticDevice->getPosition(hapticDevicePosition);
        // Stop the simulation clock
        clock.stop();
        // Read the time increment in seconds
        double timeInterval = cMin(0.001, clock.getCurrentTimeSeconds());
        timeInterval=0.001;
        bool button0, button1;
        button0 = false;
        button1 = false;



        hapticDevice->getUserSwitch(0, button0);
        hapticDevice->getUserSwitch(1, button1);
        // Human Control Mode
        if(control_mode == 1){
            main_scene->ALPHA_CONTROL = 1;
        }
        // Shared Control Mode (Human operator should always able to take back control)
        else if(control_mode == 2){
            if (button0){
                // gradually change the control to human.
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 2;
            }
            else if (button1 || main_scene->userInactive){
                // gradually change the control to robot.
                main_scene->ALPHA_CONTROL -= 0.001;
                main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
            else{
                // gradually change to equal control.
                double difference = main_scene->ALPHA_CONTROL - 0.5;
                main_scene->ALPHA_CONTROL -= copysign(0.001, difference);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
        }
        // Variable Control by Physiological Signal
        else if(control_mode == 3){
            if (button0){
                // gradually change the control constant.
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 2;
            }
            else if (button1){
                // gradually change the control constant.
                main_scene->ALPHA_CONTROL -= 0.001;
                main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
            else{
                string prediction = "0";
                string prediction_file_name = "S" + subject_num + "/prediction" + "/S" + subject_num + "_" + subject_sex + to_string(subject_age) + "_" + to_string(game_scene) + "_" + to_string(control_mode) + "_live" + ".csv";
                std::ifstream live_prediction_file(prediction_file_name);
                if(live_prediction_file.is_open()){
                    live_prediction_file >> prediction;
                }

                // gradually change to control determines by physiology data prediction.
                if(prediction == "0"){
                    main_scene->ALPHA_CONTROL += 0.001;
                    main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                    main_scene->K_DAMPING_VELOCITY = 2;
                }
                else if(prediction == "1"){
                    double difference = main_scene->ALPHA_CONTROL - 0.5;
                    main_scene->ALPHA_CONTROL -= copysign(0.001, difference);
                    main_scene->K_DAMPING_VELOCITY = 0.10;
                }
                else{
                    main_scene->ALPHA_CONTROL -= 0.001;
                    main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                    main_scene->K_DAMPING_VELOCITY = 0.10;
                }
            }
        }
        // Ideal Control (Robot Control for best score reference, not part of user experiment but to produce the standard score.)
        else if(control_mode == 4){
            main_scene->ALPHA_CONTROL = 0;
            main_scene->K_DAMPING_VELOCITY = 0.10;
        }
        // Force-based proportional human control. Alpha is proportional to force exerted by user
        else if(control_mode == 5){
            //Manual Override
            if (button0){
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 2;
            }
            else{
                main_scene->ALPHA_CONTROL = 0.1*(main_scene->userForce);
                main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
        }
        // Performance-based proportional human control. Alpha is proportional to how well the user is performing the task.
        // Collision data and time taken are both taken into account for this.
        else if(control_mode == 6){
            main_scene->fuzzyControl = true;
            //Manual Override
            if (button0){
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 2;
            }
            else {
                main_scene->ALPHA_CONTROL = main_scene->currentAlpha;
            }
            
        }
        else if (control_mode == 7) {
            using namespace std::chrono;
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            milliseconds ms = duration_cast<milliseconds>(t1 - (main_scene->inactiveTime));
            
            //Manual Override
            if (button0) {
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 2;
            }
            else if (ms.count() > 500 && (main_scene->userForce < 3.0)) {
                main_scene->ALPHA_CONTROL -= 0.001;
                main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.1);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
            else {
                main_scene->ALPHA_CONTROL += 0.001;
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 0.10;
            }
        }
        else if (control_mode == 8) {
            using namespace std::chrono;
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            milliseconds ms = duration_cast<milliseconds>(t1 - (main_scene->recordTime));
            if (ms.count() >= 500) {
                float delta = 0.1 * (main_scene->collisionsLastSec);
                if (delta > 0) {
                    main_scene->ALPHA_CONTROL -= delta;
                    main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                    main_scene->collisionsLastSec = 0;
                    main_scene->recordTime = high_resolution_clock::now();
                    main_scene->K_DAMPING_VELOCITY = 0.10;
                }
                else {
                    main_scene->ALPHA_CONTROL += 0.1;
                    main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                    main_scene->collisionsLastSec = 0;
                    main_scene->recordTime = high_resolution_clock::now();
                    main_scene->K_DAMPING_VELOCITY = 0.10;
                }
            }
        }
        else if (control_mode == 9) {
            float delta = (s->conductance - previousConductance);
            main_scene->ALPHA_CONTROL -= 0.1*delta;
            main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
            main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
            previousConductance = s->conductance;
        }
        main_scene->updateHaptics(timeInterval);
        if(main_scene->destination_index == main_scene->destinations.size()){
            break;        
        }
        else if (control_mode == 10) {
            using namespace std::chrono;
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            milliseconds ms = duration_cast<milliseconds>(t1 - (main_scene->recordTime));
            forceLastSec.push_back(main_scene->userForce);
            conductanceLastSec.push_back(s->conductance);
            if (ms.count() >= 500) {
                if (fuzzy_params.find("coll") != string::npos) {
                    fl::InputVariable* colls = fuzzy->getInputVariable("collisions");
                    colls->setValue(main_scene->collisionsLastSec);
                }
                if (fuzzy_params.find("force") != string::npos) {
                    fl::InputVariable* force = fuzzy->getInputVariable("force");
                    force->setValue((std::accumulate(forceLastSec.begin(), forceLastSec.end(), 0.0))/forceLastSec.size());
                    forceLastSec.clear();
                }
                if (fuzzy_params.find("SCL") != string::npos) {
                    fl::InputVariable* SCL = fuzzy->getInputVariable("SCL");
                    cout << "Conductance: " << s->conductance << endl;
                    SCL->setValue((std::accumulate(conductanceLastSec.begin(), conductanceLastSec.end(), 0.0)) / conductanceLastSec.size());
                    conductanceLastSec.clear();
                }
                fuzzy->process();
                double output = stod(fl::Op::str(fuzzy->outputVariables()[0]->getValue()));
                if (isnan(output)) {
                    continue;
                }
                cout << output << endl;
                main_scene->ALPHA_CONTROL += output;
                main_scene->ALPHA_CONTROL = max(main_scene->ALPHA_CONTROL, 0.0);
                main_scene->ALPHA_CONTROL = min(main_scene->ALPHA_CONTROL, 1.0);
                main_scene->K_DAMPING_VELOCITY = 1;
                main_scene->collisionsLastSec = 0;
                main_scene->recordTime = high_resolution_clock::now();
            }
        }
        // Reset the simulation clock
        clock.reset();
        clock.start();

        // Signal frequency counter
        freqCounterHaptics.signal(1);
        ballfile <<  timeSinceEpochMillisec() << ", " << main_scene->positionMainSphere <<endl;
        conductancefile << timeSinceEpochMillisec() << ", " << s->conductance << endl;
        HIPfile << timeSinceEpochMillisec() << ", " << hapticDevicePosition << endl;
        CIPfile << timeSinceEpochMillisec() << ", " << main_scene->positionGuidanceSphere << endl;
        NIPfile << timeSinceEpochMillisec() << ", " << main_scene->positionNegotiatedSphere << endl;
        HIPforcefile << timeSinceEpochMillisec() << ", " << main_scene->sphereForce << endl;
        CIPforcefile << timeSinceEpochMillisec() << ", " << main_scene->guidanceForce << endl;


    }
    ballfile.close();
    HIPfile.close();
    CIPfile.close();
    NIPfile.close();
    HIPforcefile.close();
    CIPforcefile.close();
    hapticDevice->close();
    simulationFinished = true;
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void getSensorData() {
    s->getData();
}