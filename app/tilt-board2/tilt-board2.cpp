#include "chai3d.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <chrono>

using namespace chai3d;
using namespace std;

#include "CGenericScene.h"
#include "CScene1.h"
#include "CScene2.h"
#include "CScene3.h"

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
//---------------------------------------------------------------------------
// DISPLAY SETTINGS
//---------------------------------------------------------------------------
cStereoMode stereoMode = C_STEREO_DISABLED;
bool fullscreen = false;
bool mirroredDisplay = false;

enum MouseStates
{
    MOUSE_IDLE,
    MOUSE_MOVE_CAMERA
};
MouseStates mouseState = MOUSE_IDLE; // Mouse state
double mouseX, mouseY; // Last mouse position

//---------------------------------------------------------------------------
// CHAI3D VARIABLES
//---------------------------------------------------------------------------
cCamera* camera;
cSpotLight *light;
cHapticDeviceHandler* handler;
shared_ptr<cGenericHapticDevice> hapticDevice;
cGenericTool* tool;

cLabel* labelHapticDeviceModel;
cLabel* labelHapticDevicePosition;
cLabel* labelRates;
cVector3d hapticDevicePosition;

//---------------------------------------------------------------------------
// OBJECTS
//---------------------------------------------------------------------------
cGenericScene* main_scene;
cScene1* scene1;
cScene2* scene2;
cScene3* scene3;

//------------------------------------------------------------------------------
// DECLARED CONSTANTS
//------------------------------------------------------------------------------
const double SPHERE_RADIUS = 0.007;

//---------------------------------------------------------------------------
// GENERAL VARIABLES
//---------------------------------------------------------------------------
bool simulationRunning = false; // A flag to indicate if the simulation running
bool simulationFinished = true; // A flag to indicate if the simulation finished
cFrequencyCounter freqCounterGraphics; // A frequency counter to measure the smiluation graphic rate
cFrequencyCounter freqCounterHaptics; // A frequency counter to measure the simulation haptic rate
cThread* hapticsThread; // Haptic Thread
GLFWwindow* window = NULL; // A handle to window display context

int width = 0; // Current width of the window
int height = 0; // Current height of the window
int swapInterval = 1;// swap interval for the display context (vertical synchronization)

//---------------------------------------------------------------------------
// DECLARED FUNCTIONS
//---------------------------------------------------------------------------
void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height); // Callback when the window display is resized
void errorCallback(int error, const char* a_description); // Callback when an GLFW error occurs
void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods); // Callback when a key is pressed

void mouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods); // Callback when mouse is click
void mouseMotionCallback(GLFWwindow* a_window, double a_posX, double a_posY);
void mouseScrollCallback(GLFWwindow* a_window, double a_offsetX, double a_offsetY);

void updateGraphics(void); // This function renders the scene
void updateHaptics(void); // This function contains the main haptics simulation loop
void close(void); // This function closes the application

void initScene1();
void initScene2();
void initScene3();

std::ofstream myfile;

int main(int argc, char* argv[]){
    //---------------------------------------------------------------------------
    // Initial Print Message
    //---------------------------------------------------------------------------
    cout << endl;
    cout << "-----------------------------------" << endl;
    cout << "Tilt Board" << endl;
    cout << "-----------------------------------" << endl << endl << endl;
    cout << "Keyboard Options:" << endl << endl;
    cout << "[1] - Select Scene 1" << endl;
    cout << "[2] - Select Scene 2" << endl;
    cout << "[3] - Select Scene 3" << endl;
    cout << "[4] - Select Demo 4" << endl;
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

    scene1 = new cScene1(hapticDevice);
    scene2 = new cScene2(hapticDevice);
    scene3 = new cScene3(hapticDevice);

    // Read the scale factor between the physical workspace of the haptic
    // device and the virtual workspace defined for the tool
    double maxStiffness = 10;
    if(tool!=NULL)
    {
        double workspaceScaleFactor = tool->getWorkspaceScaleFactor();
        maxStiffness = cMin(maxStiffness,hapticDeviceInfo.m_maxLinearStiffness/workspaceScaleFactor);
    }
    
    // Stiffness properties
    scene1->camera->setStereoMode(stereoMode);
    scene2->camera->setStereoMode(stereoMode);
    scene3->camera->setStereoMode(stereoMode);

    scene1->setStiffness(maxStiffness);
    scene2->setStiffness(maxStiffness);
    scene3->setStiffness(maxStiffness);


    //--------------------------------------------------------------------------
    // WIDGETS
    //--------------------------------------------------------------------------
    cFontPtr font = NEW_CFONTCALIBRI20();
    main_scene = new cGenericScene(hapticDevice);

    // Create a label to display the haptic device model
    labelHapticDeviceModel = new cLabel(font);
    main_scene->camera->m_frontLayer->addChild(labelHapticDeviceModel);
    scene1->camera->m_frontLayer->addChild(labelHapticDeviceModel);
    scene2->camera->m_frontLayer->addChild(labelHapticDeviceModel);
    scene3->camera->m_frontLayer->addChild(labelHapticDeviceModel);
    labelHapticDeviceModel->setText(hapticDeviceInfo.m_modelName);
    cout <<hapticDeviceInfo.m_modelName<< endl;
    // Create a label to display the position of haptic device
    labelHapticDevicePosition = new cLabel(font);
    main_scene->camera->m_frontLayer->addChild(labelHapticDevicePosition);
    //scene1->camera->m_frontLayer->addChild(labelHapticDevicePosition);
    scene2->camera->m_frontLayer->addChild(labelHapticDevicePosition);
    scene3->camera->m_frontLayer->addChild(labelHapticDevicePosition);

    // Create a label to display the haptic and graphic rate of the simulation
    labelRates = new cLabel(font);
    labelRates->m_fontColor.setWhite();
    main_scene->camera->m_frontLayer->addChild(labelRates);
    scene1->camera->m_frontLayer->addChild(labelRates);
    scene2->camera->m_frontLayer->addChild(labelRates);
    scene3->camera->m_frontLayer->addChild(labelRates);
    
    
    initScene1();
    myfile.open("movement.csv");

    //--------------------------------------------------------------------------
    // START SIMULATION
    //--------------------------------------------------------------------------

    // Create a thread which starts the main haptics rendering loop
    hapticsThread = new cThread();
    hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

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
    main_scene->camera->m_frontLayer->addChild(labelHapticDeviceModel);
    main_scene->camera->m_frontLayer->addChild(labelRates);
}

void initScene2(){
    main_scene = scene2;
    main_scene->init();
}

void initScene3(){
    main_scene = scene3;
    main_scene->init();
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
        scene1->mirroredDisplay = mirroredDisplay;
        scene2->mirroredDisplay = mirroredDisplay;
        scene3->mirroredDisplay = mirroredDisplay;
    }
    else if(a_key == GLFW_KEY_S){
        main_scene->controlSphere->setEnabled(!(main_scene->controlSphere->getEnabled()));
    }

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
}

void close(void)
{
    // stop the simulation
    simulationRunning = false;

    // wait for graphics and haptics loops to terminate
    while (!simulationFinished) { cSleepMs(100); }

    // delete resources
    delete hapticsThread;
    delete scene1;
    delete scene2;
    delete scene3;
    delete handler;
}

void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
    // update window size
    width = a_width;
    height = a_height;
    // update position of label
    labelHapticDeviceModel->setLocalPos(20, height - 40, 0);

    // update position of label
    labelHapticDevicePosition->setLocalPos(20, height - 60, 0);
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
        cout<<cameraPos<<endl;
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

    // Update the text information.
    labelHapticDevicePosition->setText(hapticDevicePosition.str(3));
    labelRates->setText(cStr(freqCounterGraphics.getFrequency(), 0) + " Hz /" + 
                        cStr(freqCounterHaptics.getFrequency(),0) + " Hz");

    // Update the position of the label (the coordination of middle is changing.)
    labelRates->setLocalPos((int)(0.5*(width-labelRates->getWidth())),15);

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

        main_scene->updateHaptics(timeInterval);
        
        // Reset the simulation clock
        clock.reset();
        clock.start();

        // Signal frequency counter
        freqCounterHaptics.signal(1);
        myfile <<  timeSinceEpochMillisec() << ", "<< hapticDevicePosition <<endl;

    }
    myfile.close();
    hapticDevice->close();
    simulationFinished = true;
}