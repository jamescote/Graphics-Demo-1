#include "Curve.h"
#include "ShaderManager.h"

#define CONTROL_POINT_SIZE 5.0f

// Default Constructor
Curve::Curve(int iHeight, int iWidth, bool bQuad) : 
	Tess_Graphic(iHeight, iWidth)
{
	m_bQuadratic = bQuad;

	if (m_bQuadratic)
		setDefaultQuadratic();
	else
		setDefaultCubic();
}

// Destructor
Curve::~Curve()
{
	// Nothing to Destruct
}

// Add a control point.
void Curve::addControlPoint(GLfloat fXPoint, GLfloat fYPoint)
{
	m_fControlVerts.push_back(fXPoint);
	m_fControlVerts.push_back(fYPoint);
	m_bUpdated = true;
}

// Draw Curves if there are enough control points for each curve
void Curve::renderGraphic(const MyGeometry* pGeometry)
{
	if (m_bUpdated) // Set Vertices/Colors if something's changed
	{
		// Clear Current Storage.
		m_vVertices.clear();

		// Generate Vertices and Colors.
		for (unsigned int i = 0; i < m_fControlVerts.size(); i += 2)
		{
			if ((i + 1) < m_fControlVerts.size())
			{
				addPosVertex(m_fControlVerts[i] / m_iWidth, m_fControlVerts[i + 1] / m_iHeight);
				addColorVertex(m_fColors[0][RED], m_fColors[0][GREEN], m_fColors[0][BLUE]);
			}
		}

		// Flag for Updated.
		m_bUpdated = false;
	}

	// Refresh Screen
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (DRAW_CURVES <= m_iLevels)		// Draw the Curves if desired.
	{
		if (m_bQuadratic)		// Quadratic
			drawQuad(pGeometry);
		else                    // Cubic
			drawCubic(pGeometry);
	}

	if (DRAW_TANGENTS <= m_iLevels)		// Draw the Tangents
		drawTangents(pGeometry, m_bQuadratic);

	if (DRAW_CNTRL_POINTS <= m_iLevels)	// Draw the Control Points
		drawControlPoints(pGeometry, CONTROL_POINT_SIZE, m_bQuadratic);

	
}

/*********************************************************************\
 * Assignment Specific Functions									 *
\*********************************************************************/

// Teapot.
void Curve::setDefaultQuadratic()
{
	int iBoxWidth = 512;
	float fMaxValue = 3.0f;
	float fSlice = iBoxWidth / fMaxValue;

	m_fControlVerts.push_back(1.f  * fSlice); m_fControlVerts.push_back(1.0f  * fSlice);
	m_fControlVerts.push_back(2.0f * fSlice); m_fControlVerts.push_back(-1.0f * fSlice);
	m_fControlVerts.push_back(0.0f * fSlice); m_fControlVerts.push_back(-1.0f * fSlice);

	// (0,-1), (-2,-1), (-1,1)
	m_fControlVerts.push_back(0.0f  * fSlice); m_fControlVerts.push_back(-1.0f * fSlice);
	m_fControlVerts.push_back(-2.0f * fSlice); m_fControlVerts.push_back(-1.0f * fSlice);
	m_fControlVerts.push_back(-1.0f * fSlice); m_fControlVerts.push_back(1.0f  * fSlice);

	// (-1,1), (0,1), (1,1)
	m_fControlVerts.push_back(-1.0f * fSlice); m_fControlVerts.push_back(1.0f * fSlice);
	m_fControlVerts.push_back(0.0f  * fSlice); m_fControlVerts.push_back(1.0f * fSlice);
	m_fControlVerts.push_back(1.0f  * fSlice); m_fControlVerts.push_back(1.0f * fSlice);

	// (1.2,0.5), (2.5,1), (1.3,-0.4)
	m_fControlVerts.push_back(1.2f * fSlice); m_fControlVerts.push_back(0.5f  * fSlice);
	m_fControlVerts.push_back(2.5f * fSlice); m_fControlVerts.push_back(1.0f  * fSlice);
	m_fControlVerts.push_back(1.3f * fSlice); m_fControlVerts.push_back(-0.4f * fSlice);
}

// Supposed to look like a Fish.
void Curve::setDefaultCubic()
{
	int iBoxWidth = 512;
	float fMaxValue = 10.f;
	float fSlice = iBoxWidth / fMaxValue;

	// (1,1), (4,0), (6,2), (9,1)
	m_fControlVerts.push_back(1.0f * fSlice); m_fControlVerts.push_back(1.0f * fSlice);
	m_fControlVerts.push_back(4.0f * fSlice); m_fControlVerts.push_back(0.0f * fSlice);
	m_fControlVerts.push_back(6.0f * fSlice); m_fControlVerts.push_back(2.0f * fSlice);
	m_fControlVerts.push_back(9.0f * fSlice); m_fControlVerts.push_back(1.0f * fSlice);

	// (8,2), (0,8), (0,-2), (8,4)
	m_fControlVerts.push_back(8.0f * fSlice); m_fControlVerts.push_back(2.0f  * fSlice);
	m_fControlVerts.push_back(0.0f * fSlice); m_fControlVerts.push_back(8.0f  * fSlice);
	m_fControlVerts.push_back(0.0f * fSlice); m_fControlVerts.push_back(-2.0f * fSlice);
	m_fControlVerts.push_back(8.0f * fSlice); m_fControlVerts.push_back(4.0f  * fSlice);

	// (5,3), (3,2), (3,3), (5,2)
	m_fControlVerts.push_back(5.0f * fSlice); m_fControlVerts.push_back(3.0f * fSlice);
	m_fControlVerts.push_back(3.0f * fSlice); m_fControlVerts.push_back(2.0f * fSlice);
	m_fControlVerts.push_back(3.0f * fSlice); m_fControlVerts.push_back(3.0f * fSlice);
	m_fControlVerts.push_back(5.0f * fSlice); m_fControlVerts.push_back(2.0f * fSlice);

	// (3,2.2), (3.5,2.7), (3.5,3.3), (3,3.8)
	m_fControlVerts.push_back(3.0f * fSlice); m_fControlVerts.push_back(2.2f * fSlice);
	m_fControlVerts.push_back(3.5f * fSlice); m_fControlVerts.push_back(2.7f * fSlice);
	m_fControlVerts.push_back(3.5f * fSlice); m_fControlVerts.push_back(3.3f * fSlice);
	m_fControlVerts.push_back(3.0f * fSlice); m_fControlVerts.push_back(3.8f * fSlice);

	// (2.8,3.5), (2.4,3.8), (2.4,3.2), (2.8,3.5)
	m_fControlVerts.push_back(2.8f * fSlice); m_fControlVerts.push_back(3.5f * fSlice);
	m_fControlVerts.push_back(2.4f * fSlice); m_fControlVerts.push_back(3.8f * fSlice);
	m_fControlVerts.push_back(2.4f * fSlice); m_fControlVerts.push_back(3.2f * fSlice);
	m_fControlVerts.push_back(2.8f * fSlice); m_fControlVerts.push_back(3.5f * fSlice);
}