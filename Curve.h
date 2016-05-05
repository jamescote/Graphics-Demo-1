#pragma once
#include "Tess_Graphic.h"
#include "stdafx.h"

// Bezier Curves
class Curve :
	public Tess_Graphic
{
public:
	Curve(int iHeight, int iWidth, bool bQuad);
	~Curve();
	
	// Overwritten Virtual Functions
	void renderGraphic(const MyGeometry* pGeometry);

	// Add Control Points
	void addControlPoint(GLfloat fXPoint, GLfloat fYPoint);

private:
	vector<GLfloat> m_fControlVerts;
	bool m_bQuadratic;

	// Assignment Specific - sets Starting Control Points
	void setDefaultQuadratic();
	void setDefaultCubic();
};

