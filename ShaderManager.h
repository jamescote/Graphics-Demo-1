#pragma once

/* INCLUDES */
#include "stdafx.h"
#include "AssignmentHeader.h"
#include "Shader.h"

// Enum for Shaders
enum eShaderType
{
	REGULAR = 0,
	TEXTURE,
	TESS_QUAD,
	TESS_CUBIC,
	MAX_SHDRS
};

// Class: Shader Manager
// Purpose: Manages all the Shaders used by every Assignment.  Manages Uniform variable
//			manipulation and properly initializes and destroys created Shaders.
// Current Issues: Uniform variables hard-coded.
// TODO: (Low Priority) Scan through all shader code for uniform variables and set up
//						management of those variables dynamically.  Too much work...
// Written by: James Coté
class ShaderManager
{
public:
	static ShaderManager* getInstance();
	~ShaderManager();

	// Initialize Shaders
	bool initializeShaders();

	// Get the specified program for using shaders for rendering
	GLuint getProgram(eShaderType eType) { return m_pShader[eType].getProgram(); }

	// Shader Manipulation (Assignment 2)
	void applyGreyScale(int iSetting = -1); // Grey Scale
	void cycleGreyScale();
	void toggleNegative();					// Negative
	void setSobelKernel(int iSobelKernel);  // Sobel Kernels
	void setSobelPasses(int iPasses);
	void incrementSobelPasses();
	void decrementSobelPasses();
	void turnOffSobel();
	void decrementBlurLevel();				// Gaussian Blurs
	void incrementBlurLevel();
	void setBlurLevel(int iBlurLevel);
	void toggleBlur();	
	
	// For resetting all shaders to default values
	void resetAllShaders();

private:
	// Singleton Implementation
	ShaderManager();
	static ShaderManager* m_pInstance;

	// Should only be initialized once.
	bool m_bInitialized;

	// Shader Variables
	Shader m_pShader[MAX_SHDRS];

	// Enum for different Uniform Shader Variables.  Hard-coded for now
	// TODO: load from file. Low Priority
	enum eA2ShaderVars
	{
		CURR_GREY_SCALE = 0,
		NEGATIVE_SETTING,
		SOBEL_INDEX,
		SOBEL_VAL,
		BLUR_LEVEL,
		APPLY_BLUR,
		MAX_A2_VARS
	};

	// Store Uniform Shader Variable Locations for ASSG_2
	GLint m_sA2ShaderVarLocations[MAX_A2_VARS];
	static const GLchar* c_sA2ShaderVarNames[MAX_A2_VARS];
};

