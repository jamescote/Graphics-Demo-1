#include "STriangle.h"

#define TRI_ITER 3

// Constructor - set a Initial Base Triangle and add it.
STriangle::STriangle(int iHeight, int iWidth) : Graphic( iHeight, iWidth )
{
	sTriStruct tBaseTriangle;

	tBaseTriangle.pVertices[eBLeft].setX(-.9f);	// Bottom Left Vertex
	tBaseTriangle.pVertices[eBLeft].setY(-.9f);
	tBaseTriangle.pVertices[eTop].setX(0.f);	// Top Vertex
	tBaseTriangle.pVertices[eTop].setY(.9f);
	tBaseTriangle.pVertices[eBRight].setX(.9f);	// Bottom Right Vertex
	tBaseTriangle.pVertices[eBRight].setY(-.9f);
	copyColor(tBaseTriangle.fColor, m_fColors[0], RGB_MAX);

	m_vTriangles.push_back(tBaseTriangle);
}

STriangle::~STriangle()
{
	// Nothing to Destruct
}

// Draw every triangle (3 vertices at a time)
void STriangle::draw()
{ 
	for (int i = 0; i < (signed int)(m_vVertices.size() >> 1); i += TRI_ITER)
		glDrawArrays(GL_TRIANGLES, i, TRI_ITER);
}

// Overloaded Pure Virtual Functions
void STriangle::genVertices(const MyGeometry* pGeometry)
{
	// Locals
	int iNumTris;

	if (m_bUpdated)
	{
		clearBuffers();

		// Check expected number of Triangles
		iNumTris = (int)pow((int)eTriVerts, m_iLevels - 1);

		// See if we need to move to get to that number
		if (iNumTris > (signed int)m_vTriangles.size())
		{
			FractalUp( 2 == m_iLevels );
		}
		else if (iNumTris < (signed int)m_vTriangles.size())
		{
			FractalDown();
		}

		// Convert Triangles to Vertices & Colors
		for (int i = 0; i < (signed int)m_vTriangles.size(); ++i)
			for (int r = 0; r < eTriVerts; ++r)
			{
				addPosVertex(m_vTriangles[i].pVertices[r].getX(), m_vTriangles[i].pVertices[r].getY());
				addColorVertex(m_vTriangles[i].fColor[RED], m_vTriangles[i].fColor[GREEN], m_vTriangles[i].fColor[BLUE]);
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

/******************************************************\
 * Fractal Helper Functions							  *
\******************************************************/
// Go up a level and split all the triangles.
// bSpecialCase = Splits the First Iteration between the 3 Stored Colors
void STriangle::FractalUp( bool bSpecialCase )
{
	// Locals
	int iNumTris = (int)pow((int)eTriVerts, (m_iLevels - 1));
	vector<sTriStruct> vWorkingSet;

	// Repeat until we have the right number of triangles stored.
	while (m_vTriangles.size() != iNumTris)
	{
		// Clear current list and process via a working set.
		vWorkingSet.swap(m_vTriangles);
		m_vTriangles.clear();

		// Split each Triangle
		for (int i = 0; i < (signed int)vWorkingSet.size(); ++i)
			splitTri(&vWorkingSet[i], bSpecialCase);
	}
}

// Take a triangle, split it into 3 and store these 3 Triangles into the Triangle vector
void STriangle::splitTri(const sTriStruct *tWorkingTri, bool bSpecialCase)
{
	// Locals
	sTriStruct tNewTris[eTriVerts];
	float fColorMod = COLOR_MOD;
	
	// Bottom Left Triangle
	tNewTris[eBLeft].pVertices[eBLeft] = tWorkingTri->pVertices[eBLeft];
	//copyPos(&(tNewTris[eBLeft].pVertices[eBLeft]), &(tWorkingTri->pVertices[eBLeft]));
	mixVerts(&(tNewTris[eBLeft].pVertices[eTop]), 
			 &(tWorkingTri->pVertices[eBLeft]), 
			 &(tWorkingTri->pVertices[eTop]), 
			 .5f);
	mixVerts(&(tNewTris[eBLeft].pVertices[eBRight]),
			 &(tWorkingTri->pVertices[eBLeft]),
			 &(tWorkingTri->pVertices[eBRight]),
			 .5f);

	// Top Triangle
	tNewTris[eTop].pVertices[eTop] = tWorkingTri->pVertices[eTop];
	//copyPos(&(tNewTris[eTop].pVertices[eTop]), &(tWorkingTri->pVertices[eTop]));
	tNewTris[eTop].pVertices[eBLeft] = tNewTris[eBLeft].pVertices[eTop];
	//copyPos(&(tNewTris[eTop].pVertices[eBLeft]), &(tNewTris[eBLeft].pVertices[eTop]));
	mixVerts(&(tNewTris[eTop].pVertices[eBRight]),
		&(tWorkingTri->pVertices[eTop]),
		&(tWorkingTri->pVertices[eBRight]),
		.5f);

	// Bottom Right Triangle
	tNewTris[eBRight].pVertices[eBRight] = tWorkingTri->pVertices[eBRight];
	tNewTris[eBRight].pVertices[eTop] = tNewTris[eTop].pVertices[eBRight];
	tNewTris[eBRight].pVertices[eBLeft] = tNewTris[eBLeft].pVertices[eBRight];

	// Add Triangles to the List
	for (int i = 0; i < eTriVerts; ++i)
	{
		fColorMod *= (i + 1); // Modified by .33 .66 and .99 of the original triangle color
		if (!bSpecialCase)
			addTriangle(tNewTris[i].pVertices,
			tWorkingTri->fColor[RED] * fColorMod,
			tWorkingTri->fColor[GREEN] * fColorMod,
			tWorkingTri->fColor[BLUE] * fColorMod);
		else
			addTriangle(tNewTris[i].pVertices,
						m_fColors[i][RED],
						m_fColors[i][GREEN],
						m_fColors[i][BLUE]);
		fColorMod = COLOR_MOD;
	}
}

// Go down a level and develop all the Triangles
void STriangle::FractalDown()
{
	// Colors
	int iNumTris = (int)pow((int)eTriVerts, (m_iLevels - 1));
	vector<sTriStruct> vWorkingSet;

	// Repeat until the desired number of triangles is stored.
	while (m_vTriangles.size() != iNumTris)
	{
		// Set working Set for processing
		vWorkingSet.swap(m_vTriangles);
		m_vTriangles.clear();

		// Process every 3 triangles.  If an error occurs here, then there's a major issue.
		for (int i = 0; i < (signed int)vWorkingSet.size(); i += eTriVerts)
			combineTris(&vWorkingSet[i], &vWorkingSet[i+1], &vWorkingSet[i+2]);
	}
}

// Take in 3 Triangles and combine them into 1.
void STriangle::combineTris(const sTriStruct* tBL, const sTriStruct* tT, const sTriStruct* tBR)
{
	// New Triangle
	sTriStruct tNewTri;

	// Set Vertices
	tNewTri.pVertices[eBLeft] = tBL->pVertices[eBLeft];
	tNewTri.pVertices[eTop] = tT->pVertices[eTop];
	tNewTri.pVertices[eBRight] = tBR->pVertices[eBRight];
	/*copyPos(&(tNewTri.pVertices[eBLeft]), &(tBL->pVertices[eBLeft]));
	copyPos(&(tNewTri.pVertices[eTop]), &(tT->pVertices[eTop]));
	copyPos(&(tNewTri.pVertices[eBRight]), &(tBR->pVertices[eBRight]));*/

	// Get color from Left Triangle (.33 of desired color)
	tNewTri.fColor[RED] = tBL->fColor[RED] / COLOR_MOD;
	tNewTri.fColor[GREEN] = tBL->fColor[GREEN] / COLOR_MOD;
	tNewTri.fColor[BLUE] = tBL->fColor[BLUE] / COLOR_MOD;

	// Add Triangle
	m_vTriangles.push_back(tNewTri);
}

// Add a triangle with 3 given positions and a RGB value for the color
void STriangle::addTriangle(Vector2D *pVertices, float fRed, float fGreen, float fBlue)
{
	// New Triangle
	sTriStruct tNewTri;

	// Combine Data
	for (int i = 0; i < eTriVerts; ++i)
		tNewTri.pVertices[i] = pVertices[i];

	tNewTri.fColor[RED] = fRed;
	tNewTri.fColor[GREEN] = fGreen;
	tNewTri.fColor[BLUE] = fBlue;

	// Add to Storage
	m_vTriangles.push_back(tNewTri);
}