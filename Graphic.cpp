#include "Graphic.h"

int Graphic::m_iHeight = 0;
int Graphic::m_iWidth = 0;

// Constructor for all Graphics
Graphic::Graphic(int iHeight, int iWidth)
{
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

	m_bUpdated = true;
	m_iLevels = 1;
	m_iHeight = iHeight;
	m_iWidth = iWidth;
}

// Destructor
Graphic::~Graphic()
{
	// nothing to clear
}

// Sets the # of levels to the given integer
// Minimum = 1
// Maximum = INT_MAX
void Graphic::setLevel( int iNewLevel )
{
	m_bUpdated = iNewLevel >= 1 && iNewLevel <= INT_MAX;
	m_iLevels = m_bUpdated ? iNewLevel : m_iLevels;
}

// Sets a given color to a give value.
void Graphic::setColor( int iColorIndex, float fR, float fG, float fB )
{
	if (iColorIndex >= 0 && iColorIndex < NUM_COLORS)
	{
		setRGB(iColorIndex, RED, fR);
		setRGB(iColorIndex, GREEN, fG);
		setRGB(iColorIndex, BLUE, fB);
	}
}

// Helper function to set a specific RGB index.
void Graphic::setRGB(int iColorIndex, eRGBEnum eRGB, float fVal)
{
	if (fVal >= 0.f && fVal <= 1.f)
	{
		m_fColors[iColorIndex][eRGB] = fVal;
		m_bUpdated = true;
	}
}

// Gets a stored color at a given index.
void Graphic::getColor(int iColorIndex, float *fR, float *fG, float *fB)
{
	*fR = m_fColors[iColorIndex][RED];
	*fG = m_fColors[iColorIndex][BLUE];
	*fB = m_fColors[iColorIndex][GREEN];
}

// Copy a Color array into another.
void Graphic::copyColor(float *fLHS, const float *fRHS, int iSize)
{
	for (int i = 0; i < iSize; ++i)
		fLHS[i] = fRHS[i];
}

/**********************************************************\
 * Protected Functions									  *
\**********************************************************/

// Linearly Interpolate between 2 vectors by a given percent
void Graphic::mixVerts(Vector2D* pReturn, const Vector2D* pStart, const Vector2D* pEnd, float fPercent)
{
	pReturn->setX( ((1.f - fPercent) * pStart->getX()) + (fPercent * pEnd->getX()) );
	pReturn->setY( ((1.f - fPercent) * pStart->getY()) + (fPercent * pEnd->getY()) );
}

// Linearly Interpolate betweeen two Colors
void Graphic::mixColors(float* fReturn, const float* fStart, const float* fEnd, float fPercent, int iSize)
{
	for (int i = 0; i < iSize; ++i)
		fReturn[i] = ((1.f - fPercent) * fStart[i]) + (fPercent * fEnd[i]);
}

// Add a Color Vertex to the Color Buffer
void Graphic::addColorVertex(GLfloat fR, GLfloat fG, GLfloat fB) 
{
	m_vColors.push_back(fR);
	m_vColors.push_back(fG);
	m_vColors.push_back(fB);
}

// Add a Position to the Position Buffer
void Graphic::addPosVertex(GLfloat fX, GLfloat fY) {
	m_vVertices.push_back(fX);
	m_vVertices.push_back(fY);
}

