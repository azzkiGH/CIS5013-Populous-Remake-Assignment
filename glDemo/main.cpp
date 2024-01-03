//Includes
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "AIMesh.h"

//Removes the need to put std:: or glm:: before each function
using namespace std;
using namespace glm;


//variables that will be either set or changed
#pragma region Global variables

//Initialises time in game
GUClock* gameClock = nullptr;

//Initialises camera
ArcballCamera* mainCamera = nullptr;

// Mouse tracking for camera
bool				mouseDown = false;
double				prevMouseX, prevMouseY;

// Scene objects
CGPrincipleAxes* principleAxes = nullptr;

//Initialises object meshes
AIMesh* characterMesh = nullptr;
AIMesh* floorMesh = nullptr;
AIMesh* houseMesh = nullptr;

//Configures window height + width
const unsigned int initWidth = 512;
const unsigned int initHeight = 512;

//Default and changable positions for player
int positionx = 0.0f;
int positiony = 10.0f;
int positionz = 0.0f;

#pragma endregion

//initialising functions for later calls
#pragma region Functions
//Functions
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* window, int width, int height);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
#pragma endregion


//main
int main() {

	
	gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "CIS5013", NULL, NULL);

	//Error check if the window failed to be created
	if (window == NULL)
	{
		cout << "GLFW Window failed to be created\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	

	glfwSetFramebufferSizeCallback(window, resizeWindow); //Makes window resizable
	glfwSetKeyCallback(window, keyboardHandler); //Initialises keyboard handler to check for keys

	//Initialise glew
	glewInit();

	
	//Configure window initial size on startup
	resizeWindow(window, initWidth, initHeight);

#pragma endregion


	//Shaders and Geometry
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Make background black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	//Sets position of camera
	mainCamera = new ArcballCamera(-30.0f, -50.0f, 100.0f, 55.0f, 1.0f, 0.1f, 10000.0f);
	
	//adds positional axes
	principleAxes = new CGPrincipleAxes();

	//Adds new mesh, searches for model (.obj) file
	characterMesh = new AIMesh(string("Assets\\character\\male_character.obj"));
	if (characterMesh) {
		characterMesh->addTexture(string("Assets\\character\\outfit.bmp"), FIF_BMP); //adds bitmap texture to model
	}

	//Adds new mesh, searches for model (.obj) file
	floorMesh = new AIMesh(string("Assets\\floor\\floor_mars.obj"));
	if (floorMesh) {
		floorMesh->addTexture(string("Assets\\floor\\mars.jpg"), FIF_JPEG);//adds jpeg texture to model
	}

	//Adds new mesh, searches for model (.obj) file
	houseMesh = new AIMesh(string("Assets\\house\\house_02.obj"));
	if (houseMesh) {
		houseMesh->addTexture(string("Assets\\house\\house_texture.bmp"), FIF_BMP);//adds bitmap texture to model
	}


	while (!glfwWindowShouldClose(window)) {

		updateScene(); //updates stage on screen
		renderScene();				//renders the stage onto the screen
		glfwSwapBuffers(window);	 		

		glfwPollEvents();				
	
		//reformats the window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013 Universum: Average fps: %.0f", gameClock->averageFPS());
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate(); //removes GLFW once window is closed

	if (gameClock) { //if the game clock is still on, turn it off and record timing data

		gameClock->stop();
		gameClock->reportTimingData();
	}

	return 0;
}

//renderscene function
void renderScene()
{
	//clear the colour on the buffer window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//renders camera in position
	mat4 cameraTransform = mainCamera->projectionTransform() * mainCamera->viewTransform();

#if 1
	
	if (characterMesh) {

		//character translation
		mat4 playerTranslate = translate(identity<mat4>(), vec3(positionx, positiony, positionz));
		mat4 P = cameraTransform * playerTranslate;
		glLoadMatrixf((GLfloat*)&P); //load camera transformation + player translation

		characterMesh->preRender(); //get character ready 
		characterMesh->render(); //render character on screen
		characterMesh->postRender(); //remove character data from stage
	}

	if (floorMesh) {

	
		
		glLoadMatrixf((GLfloat*)&cameraTransform); //load camera transformation

		floorMesh->preRender(); //get floor ready
		floorMesh->render(); //render floor on screen
		floorMesh->postRender(); //remove floor data from stage
	}

	if (houseMesh) {

		mat4 houseTranslate = translate(identity<mat4>(), vec3(60.0f, 3.0f, 0.0f)); //house translation
		mat4 H = cameraTransform * houseTranslate; 
		glLoadMatrixf((GLfloat*)&H); //load camera transformation and house translation

		houseMesh->preRender(); //get house ready
		houseMesh->render(); //render house on screen
		houseMesh->postRender(); //remove house data from stage
	}
	
#endif

}

//updateScene - simulate time and animate renders on screen
void updateScene() {

	float tDelta = 0.0f;

	if (gameClock) {

		gameClock->tick();
		tDelta = (float)gameClock->gameTimeDelta();
	}
}

//resizeWindow - allows window to be resizable and for the camera to follow along with it (if screen gets wider, camera will have a wider perspective)
void resizeWindow(GLFWwindow* window, int width, int height)
{
	if (mainCamera) {

		mainCamera->setAspect((float)width / (float)height);
	}

	glViewport(0, 0, width, height);		// Draw into entire window
}

//keyboardHandler - checks for keyboard inputs
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {

		// check which key was pressed...
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break; //when ESC is pressed, the window will close

			case GLFW_KEY_D:
				positionz = positionz + 2.0f;
				break; //when D is pressed, the character mesh will move to the right

			case GLFW_KEY_S:
				positionx = positionx - 2.0f;
				break; //when S is pressed, the character mesh will move to the right

			case GLFW_KEY_A:
				positionz = positionz - 2.0f;
				break; //when A is pressed, the character mesh will move to the right

			case GLFW_KEY_W:
				positionx = positionx + 2.0f;
				break; //when W is pressed, the character mesh will move to the right


			default:
			{
			}
		}
	}
	else if (action == GLFW_RELEASE) {
		// handle key release events
		switch (key)
		{
			default:
			{
			}
		}
	}
}
