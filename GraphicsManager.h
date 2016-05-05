#pragma once

/* INCLUDES */
#include "stdafx.h"
#include "GraphicsHeader.h"
#include "AssignmentHeader.h"
#include "SDiamonds.h"
#include "PSpiral.h"
#include "STriangle.h"
#include "Image.h"
#include "Curve.h"
#include "Text.h"
#include "ScrollingText.h"
#include "Camera.h"

#define CURR_GRAPHIC	m_pGraphicsSet[iCurrGraphic]
#define CURR_IMAGE		m_pImageSet[iCurrImage]
#define CURR_A3_SCENE	m_pFontCurveScenes[iCurrA3Scene]

// Sobel Flags for Switch Sobel Kernels
#define V_SOBEL_FLAG 0
#define H_SOBEL_FLAG 1
#define U_MASK_FLAG 2

// Forward Declarations
class ShaderManager;
class GeometryManager;

// Class: Graphics Manager
// Purpose: Acts as the Sinew between all moving parts that are required for drawing
//			with openGL.
// Current Issues: Seems to do too much.  Need to offload some functionality for less
//					convolution.
// TODO: Offload Assignment Management to a seperate Manager. 
//		 Set-up a Manager for Geometry (Geometry may be expanded on later).
// Written by: James Coté
class GraphicsManager
{
public:
	static GraphicsManager* getInstance(GLFWwindow *rWindow);
	~GraphicsManager();

	// Resets current object to defaults (Currently only works for Assignment 2+)
	void resetObject();

	// Image Updating (Assignment 2)
	void translateImage(float fTargetX, float fTargetY, const Vector2D* vInitialPos);
	void applyScroll(float fOffset);
	void rotateImage(float fTargetX, float fTargetY, const Vector2D* vInitialPos);
	void loadImage(string sImgName);
	void deleteImage();
	void cycleImage();

	// Graphics Application
	bool initializeGraphics();
	bool renderGraphics();

	// Assignment Modification
	void switchAssg() { m_eAssgState = (eAssignmentState)((m_eAssgState + 1) % MAX_ASSG); }

	// Graphics Modification - Assignment 1
	/// Level
	void incrementLevel();
	void decrementLevel();
	void setLevel(int iNewLevel);
	int getLevel() { return CURR_GRAPHIC->getLevel(); }

	/// Color
	void setColor(int iColorIndex, float fR, float fG, float fB);
	void getColor(int iColorIndex, float* fR, float* fG, float* fB);

	/// Graphic type
	void changeScene(int iDirection);
	void setType(eGraphicsEnum eNewGraphic);

	/// HxW Settings
	void resizedWindow(int iHeight, int iWidth) { CURR_GRAPHIC->setAspect(iHeight, iWidth);
												  CURR_IMAGE.setAspect(iHeight, iWidth);
												  m_pCamera->updateHxW( iHeight, iWidth ); }

	// Text Modification - Assignment 3
	void changeFont();

	// Camera Functions - Assignment 4
	void snapShotCamera();

private:
	// For Singleton Implementation
	GraphicsManager(GLFWwindow* rWindow); 
	static GraphicsManager* m_pInstance;

	// Window Reference
	GLFWwindow* m_pWindow;

	// Set of Graphic objects
	Graphic *m_pGraphicsSet[GPX_SIZE];		// Assignment 1
	int iCurrGraphic;
	vector<Image> m_pImageSet;				// Assignment 2
	int iCurrImage;

	// Assignment 3 Scenes
	Tess_Graphic* m_pFontCurveScenes[A3_SIZE];	// Assignment 3
	Text* m_pTextDisplay;
	ScrollingText* m_pScrollText;
	int iCurrA3Scene;

	// Assignment 4 Camera(s)
	Camera* m_pCamera;
	bool bA4Rendered;	// Only want to render this on command (Temp...).
	int iCurrA4Scene;

	// Render Functions
	// TODO: Offload to Assignment Manager
	void RenderScene();
	void renderAssg1();
	void renderAssg2();
	void renderAssg3();
	void renderAssg4();
	
	// For Legacy Assignments
	eAssignmentState m_eAssgState;

	// Manages Shaders for all assignments
	ShaderManager* m_pShaderMngr;
	GeometryManager* m_pGeometryMngr;

	bool initializeGeometry(); // Bind Textures
};

