#include "Image.h"
#include "ImageReader.h"

#define ROTATION_VELOCITY 5

// Definitions for Coordinates of Object
#define LOWER_RIGHT 3
#define UPPER_RIGHT 2
#define UPPER_LEFT 1
#define LOWER_LEFT 0

// Constructor: init the texture, set starting positions for the object and set texture coordinates
//	Call base constructor to initialize inherited vars
Image::Image(int iHeight, int iWidth, const string& sImage) : Graphic(iHeight, iWidth)
{
	InitializeTexture(&m_pTexture, sImage);

	// Set initial geometry coords
	resetImg();

	m_pTextCoords[0][0] = 0;
	m_pTextCoords[0][1] = 0;
	m_pTextCoords[1][0] = 0;
	m_pTextCoords[1][1] = m_pTexture.height;
	m_pTextCoords[2][0] = m_pTexture.width;
	m_pTextCoords[2][1] = m_pTexture.height;
	m_pTextCoords[3][0] = m_pTexture.width;
	m_pTextCoords[3][1] = 0;
}

Image::~Image()
{
	// Nothing to Destruct
}

// Fan out triangles over 4 vertices.  Hopefully this isn't depreciated.
void Image::draw()
{
	glDrawArrays(GL_TRIANGLE_FAN, 0, m_vVertices.size() >> 1);
}

void Image::genVertices(const MyGeometry* pGeometry)
{
	// Apply Transformations and Return new positions.
	// Also parse Vertices into Vertex Vector for 2 Triangles
	float fX, fY;
	Matrix2D mTemp;

	if (m_bUpdated)
	{
		m_vVertices.clear();

		// Draw Space:
		// p2----p3
		//  |	 |
		//  |	 |
		// p1----p4
		for (int i = 0; i < NUM_TEXTURE_COORDS; ++i)
		{
			// Apply transforms, add new coordinates
			m_pVerts[i].modVect(&m_pModMatrix);
			fX = m_pVerts[i].getX();
			fY = m_pVerts[i].getY();
			normalizeWindowPos(&fX, &fY); // Transform from Pixel space to Window Space
			addPosVertex(fX, fY);
		}

		m_pModMatrix.resetMatrix(); // Reset all transforms

		m_bUpdated = false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*m_vVertices.size(), &m_vVertices[0]);
	glBindBuffer(GL_ARRAY_BUFFER, pGeometry->textureBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_pTextCoords), m_pTextCoords);
}

void Image::normalizeWindowPos(float *fX, float *fY)
{
	*fX = (2 * (*fX) / m_iWidth);
	*fY = (2 * (*fY) / m_iHeight);
}

// Binds the texture to the geometry
void Image::bindTextData(GLsizeiptr* iPtr, void** data)
{
	*iPtr = sizeof(m_pTextCoords);
	*data = m_pTextCoords;
}

/******************************************************************************************\
 * Functions to perform on Image:														  *
\******************************************************************************************/
/// Rotate
void Image::rotateImg(const Vector2D* vDirection)
{
	float fDegrees;
	float fImgHeight = (m_pVerts[UPPER_LEFT] - m_pVerts[LOWER_LEFT]).length();

	m_bUpdated = true;

	// calculate tan(theta) = Opposite / adjacent; solve for theta.  Rotation is inversed.
	fDegrees = -atan( (vDirection->getX() * ROTATION_VELOCITY) / fImgHeight);
	m_pModMatrix.setRotation(fDegrees);
}

/// Move
void Image::translateImg(const Vector2D* vDirection)
{
	m_bUpdated = true;

	m_pModMatrix.setTranslation(vDirection);
}

/// Zoom
void Image::zoomImg(float fOffset)
{
	m_bUpdated = true;

	m_pModMatrix.setScale(fOffset);
}

/// Reset
void Image::resetImg()
{
	float fWidthHalf, fHeightHalf;

	fWidthHalf = (float)(m_pTexture.width >> 1);
	fHeightHalf = (float)(m_pTexture.height >> 1);
	/* Set up Texture Coords:
	1------------------------------------2
	|									 |
	|									 |
	|									 |
	|									 |
	0------------------------------------3
	*/
	// 0
	m_pVerts[LOWER_LEFT].setX(-fWidthHalf);
	m_pVerts[LOWER_LEFT].setY(fHeightHalf);
	m_pVerts[LOWER_LEFT].setH(1.f);				// Homogeneous Coord.
	// 1
	m_pVerts[UPPER_LEFT].setX(-fWidthHalf);
	m_pVerts[UPPER_LEFT].setY(-fHeightHalf);
	m_pVerts[UPPER_LEFT].setH(1.f);				// Homogeneous Coord.
	// 2
	m_pVerts[UPPER_RIGHT].setX(fWidthHalf);
	m_pVerts[UPPER_RIGHT].setY(-fHeightHalf);
	m_pVerts[UPPER_RIGHT].setH(1.f);			// Homogeneous Coord.
	// 3
	m_pVerts[LOWER_RIGHT].setX(fWidthHalf);
	m_pVerts[LOWER_RIGHT].setY(fHeightHalf);
	m_pVerts[LOWER_RIGHT].setH(1.f);			// Homogeneous Coord.

	// Trigger Update
	m_bUpdated = true;
}
