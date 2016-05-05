#include "SDiamonds.h"

#define INITIAL_SIDELENGTH 1.5f
#define VERTEX_ITER 4

// Initial values for Square & Diamond pattern
SDiamonds::SDiamonds(int iHeight, int iWidth) : Graphic(iHeight, iWidth)
{
	m_Position = { -0.75f, 0.75f }; // Set Starting Position
}

SDiamonds::~SDiamonds()
{
	// Nothing to destruct
}

// Drawing Algorithm, Specifically for Square & Diamond Pattern
void SDiamonds::draw()
{
	for (int i = 0; i < (signed int)(m_vVertices.size() >> 1); i += VERTEX_ITER)
		glDrawArrays(GL_LINE_LOOP, i, VERTEX_ITER);
}

// genVertices
// Generates vertices to be drawn to the screen
void SDiamonds::genVertices(const MyGeometry* pGeometry)
{
	float fSideLength = INITIAL_SIDELENGTH;
	Vector2D pCurrPos = m_Position;
	Vector2D pNextPos;
	float fIntensity = 1.f;
	float fIteration = 0.175f;

	if (m_bUpdated)
	{
		clearBuffers();

		for (int i = 0; i < m_iLevels; ++i)
		{
			// Generate Square, set up for Diamond
			genSquare(&pCurrPos, fSideLength, fIntensity);
			pNextPos.setX( pCurrPos.getX() + fSideLength );
			pNextPos.setY( pCurrPos.getY() );
			mixVerts(&pCurrPos, &pCurrPos, &pNextPos, 0.5f);
			fSideLength = fSideLength / 2.f;

			// Generate Diamond, set up for next iteration
			genDiamond(&pCurrPos, fSideLength, fIntensity);
			pNextPos.setX(pCurrPos.getX() - fSideLength);
			pNextPos.setY(pCurrPos.getY() - fSideLength);
			mixVerts(&pCurrPos, &pCurrPos, &pNextPos, 0.5f);

			// Reduce Intensity of next level
			fIntensity -= fIteration;
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

// Generates 4 vertices:
// (x,y)---------------(x+lngth,y)
//   |						   |
//   |						   |
//   |						   |
//   |						   |
//   |						   |
//   |						   |
// (x,y-lngth)---------(x+lngth,y-lngth)
void SDiamonds::genSquare(const Vector2D *pStart, float fLngth, float fCIntensity)
{
	addPosVertex(pStart->getX(), pStart->getY());
	addPosVertex(pStart->getX() + fLngth, pStart->getY());
	addPosVertex(pStart->getX() + fLngth, pStart->getY() - fLngth);
	addPosVertex(pStart->getX(), pStart->getY() - fLngth);
	
	for (int i = 0; i < 4; ++i)
		for (int r = 0; r < RGB_MAX; ++r)
			m_vColors.push_back(m_fColors[0][r] * fCIntensity);
}

// Generates 4 vertices:
//				(x,y)
//				 / \
//				/   \
//			   /     \
//			  /       \
//		     /         \
// (x-lngth,y-length) (x+lngth,y-lngth)
//			 \         /
//            \       /
//             \     /
//              \   /
//               \ /
//              (x,y-2*lngth)
void SDiamonds::genDiamond(const Vector2D *pStart,  float fLngth, float fCIntensity)
{
	addPosVertex(pStart->getX(), pStart->getY());
	addPosVertex(pStart->getX() + fLngth, pStart->getY() - fLngth);
	addPosVertex(pStart->getX(), pStart->getY() - 2 * fLngth);
	addPosVertex(pStart->getX() - fLngth, pStart->getY() - fLngth);

	for (int i = 0; i < 4; ++i)
		for (int r = 0; r < RGB_MAX; ++r)
			m_vColors.push_back(m_fColors[1][r] * fCIntensity);
}
