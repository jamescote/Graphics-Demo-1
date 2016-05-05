#include "Tess_Graphic.h"

//-------------------------------------------
// Defines
#define NUM_QUAD_POINTS 3
#define NUM_CUBIC_POINTS 4

// Initialize Static HxW
int Tess_Graphic::m_iHeight = 0;
int Tess_Graphic::m_iWidth = 0;

// Constant Variables - Control Point Colors
const float m_f3PControlPointColors[NUM_QUAD_POINTS][RGB_MAX] = {
	{ 1.f, 0.f, 0.f },
	{ 0.f, 1.f, 0.f },
	{ 0.f, 0.f, 1.f } };
const float m_f4PControlPointColors[NUM_CUBIC_POINTS][RGB_MAX] = {
	{ 1.f, 0.f, 0.f },
	{ 0.f, 1.f, 0.f },
	{ 0.f, 1.f, 0.f },
	{ 0.f, 0.f, 1.f } };

// Constructor
Tess_Graphic::Tess_Graphic(int iHeight, int iWidth)
{
	// Grab Shader Manager Singleton.
	m_pShdrMngr = ShaderManager::getInstance();

	// Protected Variables Set.
	m_iHeight = iHeight;
	m_iWidth = iWidth;
	m_iLevels = 3;
	m_bUpdated = true;

	// Set all to 0.0f
	for (int i = 0; i < NUM_COLORS; ++i)
		for (int r = 0; r < RGB_MAX; ++r)
			m_fColors[i][r] = 0.f;

	// Set default to Red, Gree, Blue
	for (int i = 0; i < RGB_MAX; ++i)
	{
		m_fColors[i][i] = 1.f;
		m_fTangentColors[i] = 1.f;
	}
}

// Destructor
Tess_Graphic::~Tess_Graphic()
{
	// Nothing to Destruct
}

/*************************************************************\
 * Tess_Graphic - Helper Functions                           *
\*************************************************************/

// Sets the # of levels to the given integer
// Minimum = 0
// Maximum = 3
// Used to dictate if Tangents, Control Points and/or Curves are drawn.
void Tess_Graphic::setLevel(int iNewLevel)
{
	m_bUpdated = iNewLevel >= 0 && iNewLevel <= DRAW_TANGENTS;
	m_iLevels = m_bUpdated ? iNewLevel : m_iLevels;
}

// Sets a given color to a give value.
void Tess_Graphic::setColor(int iColorIndex, float fR, float fG, float fB)
{
	if (iColorIndex >= 0 && iColorIndex < NUM_COLORS)
	{
		setRGB(iColorIndex, RED, fR);
		setRGB(iColorIndex, GREEN, fG);
		setRGB(iColorIndex, BLUE, fB);
	}
}

// Helper function to set a specific RGB index.
void Tess_Graphic::setRGB(int iColorIndex, eRGBEnum eRGB, float fVal)
{
	if (fVal >= 0.f && fVal <= 1.f)
	{
		m_fColors[iColorIndex][eRGB] = fVal;
		m_bUpdated = true;
	}
}

// Gets a stored color at a given index.
void Tess_Graphic::getColor(int iColorIndex, float *fR, float *fG, float *fB)
{
	*fR = m_fColors[iColorIndex][RED];
	*fG = m_fColors[iColorIndex][BLUE];
	*fB = m_fColors[iColorIndex][GREEN];
}

// Add a Color Vertex to the Color Buffer
void Tess_Graphic::addColorVertex(GLfloat fR, GLfloat fG, GLfloat fB)
{
	m_vColors.push_back(fR);
	m_vColors.push_back(fG);
	m_vColors.push_back(fB);
}

// Add a Position to the Position Buffer
void Tess_Graphic::addPosVertex(GLfloat fX, GLfloat fY) {
	m_vVertices.push_back(fX);
	m_vVertices.push_back(fY);
}

/*******************************************************************\
* Protected Drawing Functions                                      *
\*******************************************************************/

// Draw a point
// Assume Nothing has been done in terms of GL set up
// Except: Screen Cleared.
// Other Assumptions:
//		m_vVertices and m_vColors have been set and just need to be drawn.
void Tess_Graphic::drawPoints(const MyGeometry* pGeometry)
{
	glUseProgram(m_pShdrMngr->getProgram(REGULAR));
	glBindVertexArray(pGeometry->vertexArray);

	// Set Vertex and Colour Buffer for drawing.
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

	// Draw the Point
	glDrawArrays(GL_POINTS, 0, m_vVertices.size() >> 1);

	// Lose the Program
	glBindVertexArray(0);
	glUseProgram(0);
}

// Draw a Line, assuming m_vVertices and m_vColors have been properly set.
void Tess_Graphic::drawLine(const MyGeometry* pGeometry, GLenum eMode)
{
	glUseProgram(m_pShdrMngr->getProgram(REGULAR));
	glBindVertexArray(pGeometry->vertexArray);

	// Set Vertex and Colour Buffer for drawing.
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

	// Draw the Line(s)
	glDrawArrays(eMode, 0, m_vVertices.size() >> 1);

	// Lose the Program
	glBindVertexArray(0);
	glUseProgram(0);
}

// Draws a Quadratic Spline using Tessellation Shader.
// Assumes 4 Vertices have been set.
// Doesn't draw Tangents or Control Points.
void Tess_Graphic::drawQuad(const MyGeometry* pGeometry)
{
	// Set up Program and Vertex Array (Using ASSG_3 Shader for Tessellation)
	glUseProgram(m_pShdrMngr->getProgram(TESS_QUAD));
	glBindVertexArray(pGeometry->vertexArray);

	// Set Vertex and Colour Buffer for drawing.
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

	// Draw the Curve
	glPatchParameteri(GL_PATCH_VERTICES, NUM_QUAD_POINTS);
	glDrawArrays(GL_PATCHES, 0, m_vVertices.size() >> 1);

	// Lose the Program
	glBindVertexArray(0);
	glUseProgram(0);
}

// Draws a Cubic Spline
// Assumes all Vertices and Colors have been set to m_vVertices and m_vColors respectfully.
void Tess_Graphic::drawCubic(const MyGeometry* pGeometry)
{
	// Set Tessellation Shader Program and bind Vertex Array.
	glUseProgram(m_pShdrMngr->getProgram(TESS_CUBIC));
	glBindVertexArray(pGeometry->vertexArray);

	// Set Vertex and Colour Buffer for drawing.
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

	// Set Patch Size and Draw
	glPatchParameteri(GL_PATCH_VERTICES, NUM_CUBIC_POINTS);
	glDrawArrays(GL_PATCHES, 0, m_vVertices.size() >> 1);

	// Lose the Program
	glBindVertexArray(0);
	glUseProgram(0);
}

/**********************************************************************\
 * Specific Drawing Functions, universal to Splines and Bezier Curves *
\**********************************************************************/

// Assume Vertices have been set, however, need to add proper color.
void Tess_Graphic::drawTangents(const MyGeometry* pGeometry, bool bQuad)
{
	// Locals - Store the current values for m_vColors
	vector<GLfloat> pColorCopy;
	int iCount = bQuad ? NUM_QUAD_POINTS : NUM_CUBIC_POINTS;

	if (m_vVertices.size() > 0)
	{
		// Swap Colors and add Tangent Colors
		m_vColors.swap(pColorCopy);
		for (unsigned int i = 0; i < (m_vVertices.size() >> 1); ++i)
			addColorVertex(m_fTangentColors[RED], m_fTangentColors[GREEN], m_fTangentColors[BLUE]);

		// Draw
		glUseProgram(m_pShdrMngr->getProgram(REGULAR));
		glBindVertexArray(pGeometry->vertexArray);

		// Set Vertex and Colour Buffer for drawing.
		glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

		// Draw the Line(s)
		for (unsigned int i = 0; i < m_vVertices.size() >> 1; i += iCount)
			glDrawArrays(GL_LINE_STRIP, i, iCount);

		// Lose the Program
		glBindVertexArray(0);
		glUseProgram(0);

		// Swap Colors Back.
		m_vColors.swap(pColorCopy);
	}
}

// Draw the set points in m_vVertices as 
void Tess_Graphic::drawControlPoints(const MyGeometry* pGeometry, float fPointSize, bool bQuad)
{
	// Locals - Store the current values for m_vColors
	vector<GLfloat> pColorCopy;
	int iCPMod;
	int iCount = bQuad ? NUM_QUAD_POINTS : NUM_CUBIC_POINTS;

	if (m_vVertices.size() > 0)
	{
		// Swap Colors and add Control Point Colors
		m_vColors.swap(pColorCopy);

		for (unsigned int i = 0; i < (m_vVertices.size() >> 1); ++i)
		{
			iCPMod = (i % iCount);
			if (bQuad)
				addColorVertex(m_f3PControlPointColors[iCPMod][RED],
							   m_f3PControlPointColors[iCPMod][GREEN],
							   m_f3PControlPointColors[iCPMod][BLUE]);
			else
				addColorVertex(m_f4PControlPointColors[iCPMod][RED],
							   m_f4PControlPointColors[iCPMod][GREEN],
							   m_f4PControlPointColors[iCPMod][BLUE] );
		}

		// Draw Points.
		glUseProgram(m_pShdrMngr->getProgram(REGULAR));
		glBindVertexArray(pGeometry->vertexArray);

		// Set Vertex and Colour Buffer for drawing.
		glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
		glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);

		// Set Size and Draw the Point
		glPointSize(fPointSize);
		for (unsigned int i = 0; i < m_vVertices.size() >> 1; i += iCount )
			glDrawArrays(GL_POINTS, i, iCount);
		glPointSize(1);

		// Lose the Program
		glBindVertexArray(0);
		glUseProgram(0);

		// Swap Colors Back
		m_vColors.swap(pColorCopy);
	}
}
