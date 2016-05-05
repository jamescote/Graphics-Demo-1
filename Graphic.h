#pragma once

/* INCLUDES */
#include "GraphicsHeader.h"
#include "Vector2D.h"
#include "stdafx.h"

/* DEFINES */

// Graphic Class
// Contains general functions and variables needed for child classes
// to render with OpenGL
class Graphic
{
public:
	Graphic(int iHeight, int iWidth);
	~Graphic();

	// Pure Virutal Functions for Inheritence.
	virtual void genVertices(const MyGeometry* pGeometry) {};
	virtual eGraphicsEnum getType() { return UNDEFINED; };
	virtual void draw() = 0;
	virtual void drawTangents(const MyGeometry* pGeometry) { };

	// Graphics Modification
	/// Level
	void incLevel() { setLevel(m_iLevels + 1); }
	void decLevel() { setLevel(m_iLevels - 1); }
	virtual void setLevel(int iNewLevel);
	int getLevel() { return m_iLevels; }

	/// Color
	void setColor(int iColorIndex, float fR, float fG, float fB);
	void getColor(int iColorIndex, float *fR, float *fG, float *fB);

	/// Update
	void setUpdate(bool bUpdate) { m_bUpdated = bUpdate; }

	/// Window HxW
	virtual void setAspect(int iHeight, int iWidth) { m_iHeight = iHeight; m_iWidth = iWidth; m_bUpdated = true; }
	virtual void setHeight(int iHeight) { m_iHeight = iHeight; m_bUpdated = true; }
	virtual void setWidth(int iWidth) { m_iWidth = iWidth; m_bUpdated = true; }
	static int getHeight() { return m_iHeight; }
	static int getWidth() { return m_iWidth; }
	virtual void zoomImg(float fOffset) {};

protected:
	// Variables
	bool m_bUpdated;
	int m_iLevels;
	float m_fColors[NUM_COLORS][RGB_MAX];
	vector<GLfloat> m_vVertices;			// Stores the  vertices to draw for each graphic
	vector<GLfloat> m_vColors;				// Stores the colors of each vertex to draw for each graphic
	float m_fTangentColors[RGB_MAX];
	static int m_iWidth, m_iHeight;

	// Protected Functions
	void clearBuffers() { m_vVertices.clear(); m_vColors.clear(); }
	void copyColor(float *fLHS, const float *fRHS, int iSize);

	// Structure for Graphic Position
	// Anchored to:
	// Top Left corner for Square & Diamonds
	// Center for Parametric Spiral
	// Top for Triangle Fractal
	// Top Left for Square Fractal
	Vector2D m_Position;
	
	// LERP functions
	// Linearly Interpolate between two vectors
	void mixVerts(Vector2D* pReturn, const Vector2D* pStart, const Vector2D* pEnd, float fPercent);
	void mixColors(float* fReturn, const float* fStart, const float* fEnd, float fPercent, int iSize);

	// Functions for cleaner vertex & color adding code
	void addColorVertex(GLfloat fR, GLfloat fG, GLfloat fB);
	void addPosVertex(GLfloat fX, GLfloat fY);

private:
	void setRGB(int iColorIndex, eRGBEnum eRGB, float fR);
};

