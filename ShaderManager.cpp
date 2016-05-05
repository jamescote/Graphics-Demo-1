#include "ShaderManager.h"

/////////////
// Defines //
/////////////

// Uniform variable locations - ASSG_2
#define CURR_GREY_SCALE_LOC		m_sA2ShaderVarLocations[CURR_GREY_SCALE]
#define NEGATIVE_SETTING_LOC	m_sA2ShaderVarLocations[NEGATIVE_SETTING]
#define SOBEL_INDEX_LOC			m_sA2ShaderVarLocations[SOBEL_INDEX]
#define SOBEL_VAL_LOC			m_sA2ShaderVarLocations[SOBEL_VAL]
#define BLUR_LEVEL_LOC			m_sA2ShaderVarLocations[BLUR_LEVEL]
#define APPLY_BLUR_LOC			m_sA2ShaderVarLocations[APPLY_BLUR]

// Uniform variable locations - ASSG_3
#define QUADRATIC_FLAG_LOC		m_sA3ShaderVarLocations[FLAG_QUAD]

// Uniform variable Mins/Maxs/Defaults
#define GREY_SCALE_MAX 3
#define MAX_SOBEL_PASSES 5
#define MAX_SOBEL_TYPES 3

// Singleton Variable initialization
ShaderManager* ShaderManager::m_pInstance = NULL;

// Different Vertex Shaders required for each assignment
const string m_sVShaderNames[MAX_SHDRS] = {
	"vertex_A1.glsl",
	"vertex_A2.glsl",
	"vertex_A3.glsl",
	"vertex_A3.glsl"
};

// Different Fragment Shaders required for each assignment
const string m_sFShaderNames[MAX_SHDRS] = {
	"fragment_A1.glsl",
	"fragment_A2.glsl",
	"fragment_A3.glsl",
	"fragment_A3.glsl"
};

// Different Tessellation Control Shaders required for each assignment.
const string m_sTCShaderNames[MAX_SHDRS] = {
	"tess_Control_A1.glsl",
	"tess_Control_A2.glsl",
	"tess_Control_A3_Quad.glsl",
	"tess_Control_A3_Cubic.glsl"
};

// Different Tessellation Control Shaders required for each assignment.
const string m_sTEShaderNames[MAX_SHDRS] = {
	"tess_Eval_A1.glsl",
	"tess_Eval_A2.glsl",
	"tess_Eval_A3_Quad.glsl",
	"tess_Eval_A3_Cubic.glsl"
};

// Assignment 2 Shader Variable Names.
const GLchar* ShaderManager::c_sA2ShaderVarNames[MAX_A2_VARS] = { "iCurrGreyScale",
															      "bNegativeSetting",
															      "iSobelIndex",
															      "iSobelVal",
															      "iBlurLevel",
															      "bApplyBlur" };

// Public - Not a singleton
// Designed mainly to manage different shaders between assignments.  
// Could make Assignment Classes that each have their own shader managers: TODO.
ShaderManager::ShaderManager()
{
	m_bInitialized = false;
}

// Get the Singleton ShaderManager Object.  Initialize it if NULL.
ShaderManager* ShaderManager::getInstance()
{
	if (NULL == m_pInstance)
		m_pInstance = new ShaderManager();

	return m_pInstance;
}

// Destructor - Kill any shaders that we've been using.
ShaderManager::~ShaderManager()
{
	// unbind any shader programs
	glUseProgram(0);
}

/*******************************************************************\
 * Set up Shaders												   *
\*******************************************************************/

// Inializes shaders. 
bool ShaderManager::initializeShaders()
{
	m_bInitialized = m_pShader[REGULAR].initializeShader(m_sVShaderNames[REGULAR], m_sFShaderNames[REGULAR]);
	m_bInitialized &= m_pShader[TEXTURE].initializeShader(m_sVShaderNames[TEXTURE], m_sFShaderNames[TEXTURE]);
	m_bInitialized &= m_pShader[TESS_QUAD].initializeShader(m_sVShaderNames[TESS_QUAD], m_sFShaderNames[TESS_QUAD],
															m_sTCShaderNames[TESS_QUAD], m_sTEShaderNames[TESS_QUAD]);
	m_bInitialized &= m_pShader[TESS_CUBIC].initializeShader(m_sVShaderNames[TESS_CUBIC], m_sFShaderNames[TESS_CUBIC],
															 m_sTCShaderNames[TESS_CUBIC], m_sTEShaderNames[TESS_CUBIC]);

	if ( m_bInitialized )
		for (unsigned int i = 0; i < MAX_A2_VARS; i++)
			m_sA2ShaderVarLocations[i] = m_pShader[TEXTURE].fetchVarLocation(c_sA2ShaderVarNames[i]);

	// return False if not all Shaders Initialized Properly
	return m_bInitialized;
}


/*******************************************************************************\
* Assignment 2 Functions - Shader manipulation                                 *
\*******************************************************************************/

void ShaderManager::resetAllShaders()
{
	// Turn off Grey Scale
	applyGreyScale();

	// Reset Blur Level
	setBlurLevel(1);

	// Turn off Negative
	glUseProgram(m_pShader[TEXTURE].getProgram());
	glUniform1i(NEGATIVE_SETTING_LOC, (GLint)false);
	glUseProgram(0);

	// Turn off Blur
	glUseProgram(m_pShader[TEXTURE].getProgram());
	glUniform1i(APPLY_BLUR_LOC, (GLint)false);
	glUseProgram(0);

	// Turn off Sobel
	turnOffSobel();
}

// Sets the Grey scale flag in the fragment shader.
void ShaderManager::applyGreyScale(int iSetting)
{
	if (ERR_CODE != CURR_GREY_SCALE_LOC && iSetting >= -1 && iSetting < GREY_SCALE_MAX)
	{
		// Set the Grey Scale variable
		glUseProgram(m_pShader[TEXTURE].getProgram());
		glUniform1i(CURR_GREY_SCALE_LOC, (GLint)iSetting);
		glUseProgram(0);

		CheckGLErrors();
	}
}

// Sets the Grey scale flag in the fragment shader.
void ShaderManager::cycleGreyScale()
{
	GLint iCurrGreyScale;

	if (ERR_CODE != CURR_GREY_SCALE_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), CURR_GREY_SCALE_LOC, &iCurrGreyScale);

		iCurrGreyScale = (iCurrGreyScale + 1) % GREY_SCALE_MAX;

		glUseProgram(m_pShader[TEXTURE].getProgram());
		glUniform1i(CURR_GREY_SCALE_LOC, iCurrGreyScale);
		glUseProgram(0);
	}
}


// Toggle the Uniform variable that controls the negative color setting on the fragment
// shader.
void ShaderManager::toggleNegative()
{
	GLint bCurrSetting;

	if (ERR_CODE != NEGATIVE_SETTING_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), NEGATIVE_SETTING_LOC, &bCurrSetting);

		bCurrSetting = !bCurrSetting;

		glUseProgram(m_pShader[TEXTURE].getProgram());
		glUniform1i(NEGATIVE_SETTING_LOC, bCurrSetting);
		glUseProgram(0);
	}
}

// Sets the type of Sobel s.t. there is only one type active at any one time.
void ShaderManager::setSobelKernel(int iSobelKernel)
{
	GLint iCurrKernel;

	if (ERR_CODE != SOBEL_INDEX_LOC && iSobelKernel >= 0 && iSobelKernel < MAX_SOBEL_TYPES)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), SOBEL_INDEX_LOC, &iCurrKernel);

		if (iSobelKernel == iCurrKernel)
			incrementSobelPasses();
		else
		{
			glUseProgram(m_pShader[TEXTURE].getProgram());
			glUniform1i(SOBEL_INDEX_LOC, iSobelKernel);
			glUseProgram(0);

			setSobelPasses(1);
		}
	}
}

// Set the amount of times to apply the Sobel Kernel
// In order to turn Sobel off --> set this to 0
void ShaderManager::setSobelPasses(int iPasses)
{
	if (ERR_CODE != SOBEL_VAL_LOC && iPasses >= 0 && iPasses < MAX_SOBEL_PASSES)
	{
		glUseProgram(m_pShader[TEXTURE].getProgram());
		glUniform1i(SOBEL_VAL_LOC, iPasses);
		glUseProgram(0);
	}
}

// Increment the number of passes by 1.  Wraps around the Max value.
void ShaderManager::incrementSobelPasses()
{
	GLint iCurrSetting;

	if (ERR_CODE != SOBEL_VAL_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), SOBEL_VAL_LOC, &iCurrSetting);

		iCurrSetting = (iCurrSetting + 1) % MAX_SOBEL_PASSES;

		setSobelPasses(iCurrSetting);
	}
}

// Decrement the number of passes by 1.  Clamps at 0.
void ShaderManager::decrementSobelPasses()
{
	GLint iCurrSetting;

	if (ERR_CODE != SOBEL_VAL_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), SOBEL_VAL_LOC, &iCurrSetting);

		iCurrSetting -= 1;
		iCurrSetting = iCurrSetting < 0 ? 0 : iCurrSetting;

		setSobelPasses(iCurrSetting);
	}
}

// sets Sobel Passes to 0.  Can also call setSobelPasses(0), this is just easier to read.
void ShaderManager::turnOffSobel()
{
	setSobelPasses(0);
}

// Decrease Blur level by 2, down to minimum of 1.
void ShaderManager::decrementBlurLevel()
{
	GLint iCurrSetting;

	if (ERR_CODE != BLUR_LEVEL_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), BLUR_LEVEL_LOC, &iCurrSetting);

		iCurrSetting -= 2;
		iCurrSetting = iCurrSetting < 1 ? 1 : iCurrSetting;

		setBlurLevel(iCurrSetting);
	}
}

// Increment Blur level by 2.  Currently no Maximum (hasn't been tested)
void ShaderManager::incrementBlurLevel()
{
	GLint iCurrSetting;

	if (ERR_CODE != BLUR_LEVEL_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), BLUR_LEVEL_LOC, &iCurrSetting);

		iCurrSetting += 2;

		setBlurLevel(iCurrSetting);
	}
}

// Set a Blur level, ensuring that the level is odd.
void ShaderManager::setBlurLevel(int iBlurLevel)
{
	GLint bCurrSetting;

	if (ERR_CODE != BLUR_LEVEL_LOC && iBlurLevel >= 1 && (iBlurLevel % 2) != 0)
	{
		if (ERR_CODE != APPLY_BLUR_LOC)
		{
			glGetUniformiv(m_pShader[TEXTURE].getProgram(), APPLY_BLUR_LOC, &bCurrSetting);

			if (bCurrSetting)
			{
				glUseProgram(m_pShader[TEXTURE].getProgram());
				glUniform1i(BLUR_LEVEL_LOC, iBlurLevel);
				glUseProgram(0);
			}
		}
	}
}

// Flip switch to turn on or off the blur.
void ShaderManager::toggleBlur()
{
	GLint bCurrSetting;

	if (ERR_CODE != APPLY_BLUR_LOC)
	{
		glGetUniformiv(m_pShader[TEXTURE].getProgram(), APPLY_BLUR_LOC, &bCurrSetting);

		bCurrSetting = !bCurrSetting;

		glUseProgram(m_pShader[TEXTURE].getProgram());
		glUniform1i(APPLY_BLUR_LOC, bCurrSetting);
		glUseProgram(0);
	}
}