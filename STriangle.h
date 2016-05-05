#pragma once
#include "Graphic.h"

#define BASE_VERTS m_tBaseTriangle.pVertices
#define BASE_COLOR m_tBaseTriangle.fColor
#define COLOR_MOD .33f

// triangle Fractal class
class STriangle :
	public Graphic
{
public:
	STriangle(int iHeight, int iWidth);
	~STriangle();

	// Overloaded Pure Virtual Functions
	void genVertices(const MyGeometry* pGeometry);
	eGraphicsEnum getType() { return TRI_FRACTAL; }
	void draw();

private:
	// Enum for easier reading of Points
	enum eTriPoints
	{
		eBLeft = 0,
		eTop,
		eBRight,
		eTriVerts
	};

	// Triangle Strucuture contain 3 points and an RGB-Value for the Triangle
	struct sTriStruct
	{
		Vector2D pVertices[eTriVerts];
		float fColor[RGB_MAX];
	};

	// Functions for Fractalling the Image Up or Down a level.
	void FractalUp( bool bSpecialCase );
	void FractalDown();

	// Functions for Splitting or combining Triangles
	void splitTri(const sTriStruct *tWorkingTri, bool bSpecialCase);
	void combineTris(const sTriStruct* tBL, const sTriStruct* tT, const sTriStruct* tBR);

	void addTriangle(Vector2D *pVertices, float fRed, float fGreen, float fBlue);

	// Private Values
	vector<sTriStruct> m_vTriangles;
};

