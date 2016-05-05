#include "PSpiral.h"

// Constructor Set up necessary Spiral member along with Graphic Members
PSpiral::PSpiral(int iHeight, int iWidth) : Graphic(iHeight, iWidth)
{
	m_Position		= { 0.0f, 0.f };				// Set Starting Position to Orgin
	m_fSampleRate	= (float)(.1f / PI);						// Iteration for each vertex
	m_fLimit		= (float)(LIMIT_MULTIPLIER * m_iLevels);	// Limit to stop Sampling
	m_fRatio		= m_iHeight / m_fLimit;			// Ratio to match screen size
}

PSpiral::~PSpiral()
{
	// Nothing to Destruct
}

// Draw Spiral in a line, key is on the sample rate of the vertex generation
void PSpiral::draw()
{
	glDrawArrays(GL_LINE_STRIP, 0, m_vVertices.size() >> 1);
}

void PSpiral::genVertices(const MyGeometry* pGeometry)
{
	// Local Vars
	float fCurrSample	= 0;
	Vector2D pTempVertex		= { 0.0f, 0.0f };
	float fRatioSmple	= 0;
	float fBlendColor[RGB_MAX];

	if (m_bUpdated)
	{
		clearBuffers();

		// Calculate Spiral Vertices.
		while (fCurrSample <= m_fLimit)
		{
			// Set Ratio based on Sample and Screen Height
			fRatioSmple = fCurrSample * m_fRatio;

			// Calculate Parametric Coordinates for this sample
			pTempVertex.setX( fRatioSmple * cos(fCurrSample) );
			pTempVertex.setY( fRatioSmple * sin(fCurrSample) );

			// Convert Positions to OpenGL Coordinates
			pTempVertex.setX( pTempVertex.getX() / m_iHeight );
			pTempVertex.setY( pTempVertex.getY() / m_iHeight );

			// Add Vertex, LERP Colors
			addPosVertex(pTempVertex.getX(), pTempVertex.getY());
			mixColors(fBlendColor, m_fColors[0], m_fColors[1], (fCurrSample / m_fLimit), RGB_MAX);
			addColorVertex(fBlendColor[RED], fBlendColor[GREEN], fBlendColor[BLUE]);

			// Next Sample
			fCurrSample += m_fSampleRate;
		}

		m_bUpdated = false;
	}

	// Bind Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vVertices.size()*sizeof(float), &m_vVertices[0]);

	// Bind Color Buffer
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->colourBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vColors.size()*sizeof(float), &m_vColors[0]);
}

/****************************************************\
 * Inherited Functions							    *
\****************************************************/

// Set the Level and re-calculate the Limit
void PSpiral::setLevel(int iNewLevel)
{
	Graphic::setLevel(iNewLevel);
	m_fLimit = (float)(LIMIT_MULTIPLIER * m_iLevels);
	m_fRatio = m_iHeight / m_fLimit;
}

// Set the Height and re-calculate ratio
void PSpiral::setHeight( int iHeight )
{
	Graphic::setHeight(iHeight);
	m_fRatio = m_iHeight / m_fLimit;
}

