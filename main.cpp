/* INCLUDES */
#include "stdafx.h"
#include "GraphicsHeader.h"
#include "GraphicsManager.h"
#include "ShaderManager.h"
#include "CmdHandler.h"
#include "Mouse_Handler.h"
#include "Object_Factory.h"
#include "EnvironmentManager.h"

#ifdef USING_LINUX
#include <Magick++.h>
#endif

/* DEFINES */
#define INPUT_SIZE		128
#define START_HEIGHT	512
#define START_WIDTH		512

// Function Prototypes
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void WindowResizeCallback(GLFWwindow* window, int iWidth, int iHeight);
void mouseMovecallback(GLFWwindow* window, double x, double y);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
bool initializeWindow(GLFWwindow** rWindow, int iHeight, int iWidth, const char* cTitle);

// Main entry point for the Graphics System
int main()
{
	int iRunning = glfwInit();
	char c_Input[INPUT_SIZE] = {};
	GLFWwindow* m_Window = 0;
	GraphicsManager* m_GpxMngr = 0;
	ShaderManager* m_ShdrMngr = 0;
	Mouse_Handler* m_MseHndlr;

	// Initialize GL and a window
	if (0 == iRunning)
		cout << "Error: GLFW failed to intialize, ending program." << endl;
	else
	{
		#ifdef USING_LINUX
		// Initialze Magick - LINUX only
	    Magick::InitializeMagick("");
		#endif

		// Set Error Callback
		glfwSetErrorCallback(ErrorCallback);
		iRunning = initializeWindow(&m_Window, START_HEIGHT, START_WIDTH, "CPSC 453 - Graphics");

		#ifdef USING_WINDOWS
		// Initialize GLAD Windows
		gladLoadGL();
		#endif

		// Bind window to graphics Manager
		if (1 == iRunning) 
			m_GpxMngr = GraphicsManager::getInstance(m_Window);

		// Initialize the Mouse Handler.
		m_MseHndlr = Mouse_Handler::getInstance(m_Window);

		// Initialize Graphics
		iRunning = !m_GpxMngr->initializeGraphics();

		// Main loop
		while (iRunning)
		{
			// do Graphics Loop
			iRunning = m_GpxMngr->renderGraphics();
		}

		// Clean up!
		if (m_GpxMngr != NULL)
			delete m_GpxMngr;

		if( m_MseHndlr != NULL )
		  delete m_MseHndlr;
		
		glfwDestroyWindow(m_Window);
	}

	glfwTerminate();

	cout << "Finished Program, au revoir!" << endl;

	return 0;
}

// For reporting GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW Error : " << error << ":" << endl;
	cout << description << endl;
}

bool initializeWindow(GLFWwindow** rWindow, int iHeight, int iWidth, const char* cTitle)
{
	bool bSuccess = true;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	(*rWindow) = glfwCreateWindow(iWidth, iHeight, cTitle, 0, 0);

	if (!*rWindow)
	{
		cout << "Program failed to create GLFW window" << endl;
		bSuccess = false;
	}
	else
	{
		glfwSetKeyCallback(*rWindow, KeyCallback);
		glfwSetWindowSizeCallback(*rWindow, WindowResizeCallback);
		glfwSetMouseButtonCallback(*rWindow, mouseButtonCallback);
		glfwSetCursorPosCallback(*rWindow, mouseMovecallback);
		glfwSetScrollCallback(*rWindow, mouseScrollCallback);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwMakeContextCurrent(*rWindow);
	}

	return bSuccess;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GraphicsManager* pGPXMngr = GraphicsManager::getInstance(window);
	ShaderManager* pShdrMngr = ShaderManager::getInstance();

	if (GLFW_PRESS == action)
	{
		switch (key)
		{
		case (GLFW_KEY_ESCAPE):											// Exit
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case(GLFW_KEY_UP):												// UP - Increase Level (Assg 1)
			pGPXMngr->incrementLevel();
			break;
		case(GLFW_KEY_DOWN):											// DOWN - Decrease Level (Assg 1)
			pGPXMngr->decrementLevel();
			break;
		case(GLFW_KEY_LEFT):												// LEFT - Switch Between different Graphics/Images
			pGPXMngr->changeScene(-1);
			break;
		case(GLFW_KEY_RIGHT):											// RIGHT - Switch Between different Graphics/Images
			pGPXMngr->changeScene(1);
			break;
		case(GLFW_KEY_ENTER):											// ENTER - Trigger Console Commands
			{
				CmdHandler* pCmdHndlr = CmdHandler::getInstance( window );
				char cInput[INPUT_SIZE] = {};
				char c_FirstWord[CMD_SIZE] = {};

				// Hide Window for Input
				glfwHideWindow(window);

				cout << "Enter Commands: ";
				cin.getline(cInput, INPUT_SIZE);

				// Show Window for results
				glfwShowWindow(window);

				pCmdHndlr->load_Input(cInput, INPUT_SIZE);
				pCmdHndlr->get_Next_Word(c_FirstWord, CMD_SIZE);
				pCmdHndlr->process_Input(c_FirstWord);
			}
			break;
		case(GLFW_KEY_A):												// A - Cycle Assignments
			pGPXMngr->switchAssg();
			break;
		case(GLFW_KEY_R):												// R - Reset Image
			pGPXMngr->resetObject();
			break;
		case(GLFW_KEY_G):												// G - Cycle GreyScale
			{
				if (mods == GLFW_MOD_SHIFT)
					pShdrMngr->applyGreyScale(-1);
				else
					pShdrMngr->cycleGreyScale();
			}
			break;
		case(GLFW_KEY_N):												// N - Toggle Negative
			pShdrMngr->toggleNegative();
			break;
		case(GLFW_KEY_V) :												// V - Set and or increment Vertial Sobel Kernel
		{
			if (mods == GLFW_MOD_SHIFT)
				pShdrMngr->setSobelPasses(0);
			else
				pShdrMngr->setSobelKernel(V_SOBEL_FLAG);
		}
																		break;
		case(GLFW_KEY_H) :												// H - Set and or increment Horizontal Sobel Kernel
		{
			if (mods == GLFW_MOD_SHIFT)
				pShdrMngr->setSobelPasses(0);
			else
				pShdrMngr->setSobelKernel(H_SOBEL_FLAG);
		}
																		break;
		case(GLFW_KEY_M) :												// M - Set and or increment Unsharp Mask Kernel
			{
				if (mods == GLFW_MOD_SHIFT)
					pShdrMngr->setSobelPasses(0);
				else
					pShdrMngr->setSobelKernel(U_MASK_FLAG);
			}				
			break;
		case(GLFW_KEY_B) :
			pShdrMngr->toggleBlur();									// B - Turn the Blur on or off
			break;
		case(GLFW_KEY_F) :												// F - Change Font for Assg 3
			pGPXMngr->changeFont();
			break;
		case(GLFW_KEY_S) :
			pGPXMngr->snapShotCamera();
			break;
		}
	}
}

// handles Window Resize events
void WindowResizeCallback(GLFWwindow* window, int iWidth, int iHeight)
{
	GraphicsManager* pGPXMngr = GraphicsManager::getInstance(window);
	glViewport(0, 0, iWidth, iHeight);
	pGPXMngr->resizedWindow(iHeight, iWidth);
}

// Mouse Button Callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Mouse_Handler* mMouseHndlr = Mouse_Handler::getInstance( window );
	double fX, fY;

	if (GLFW_MOUSE_BUTTON_1 == button)
	{
		glfwGetCursorPos(window, &fX, &fY);
		if (GLFW_PRESS == action)
			mMouseHndlr->mouseTStart();
		else if (GLFW_RELEASE == action)
			mMouseHndlr->mouseTEnd();
	}
	if (GLFW_MOUSE_BUTTON_2 == button)
	{
		glfwGetCursorPos(window, &fX, &fY);
		if (GLFW_PRESS == action)
			mMouseHndlr->mouseRStart();
		else if (GLFW_RELEASE == action)
			mMouseHndlr->mouseREnd();
	}
}

// Handles input from Mouse Moves.
void mouseMovecallback(GLFWwindow* window, double x, double y)
{
	Mouse_Handler* mMouseHndlr = Mouse_Handler::getInstance(window);

	mMouseHndlr->updateMouse((float)x, (float)y);
}

// Handle scroll wheel callbacks
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	GraphicsManager* pGPXMngr = GraphicsManager::getInstance(window);

	pGPXMngr->applyScroll((float)yoffset);

}
