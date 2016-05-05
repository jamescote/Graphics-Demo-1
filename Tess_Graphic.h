#pragma once

#include "stdafx.h"
#include "GraphicsHeader.h"
#include "ShaderManager.h"

// Tess Graphic Defines - Used internally.
#define DRAW_TANGENTS 3
#define DRAW_CNTRL_POINTS 2
#define DRAW_CURVES 1

// Tess_Graphic Class
// More control over Drawing controls.  More encapsulated and
// Modular Drawing than Graphic Controls.
class Tess_Graphic
{
public:
	Tess_Graphic(int iHeight, int iWidth);
	~Tess_Graphic();

	virtual void renderGraphic(const MyGeometry* pGeometry) = 0;

	// Graphics Modification
	/// Level
	void incLevel() { setLevel(m_iLevels + 1); }
	void decLevel() { setLevel(m_iLevels - 1); }
	virtual void setLevel(int iNewLevel);
	int getLevel() { return m_iLevels; }

	/// Color
	void setColor(int iColorIndex, float fR, float fG, float fB);
	void getColor(int iColorIndex, float *fR, float *fG, float *fB);

	// Set the Update value if needed.
	void setUpdate(bool bUpdate) { m_bUpdated = bUpdate; }

	// Window HxW Getters and Setters
	virtual void setAspect(int iHeight, int iWidth) { m_iHeight = iHeight; m_iWidth = iWidth; m_bUpdated = true; }
	virtual void setHeight(int iHeight) { m_iHeight = iHeight; m_bUpdated = true; }
	virtual void setWidth(int iWidth) { m_iWidth = iWidth; m_bUpdated = true; }
	virtual void zoomImg(float fOffset) {};
	static int getHeight() { return m_iHeight; }
	static int getWidth() { return m_iWidth; }
	
protected:
	// Shader Storage
	ShaderManager* m_pShdrMngr;

	// Different Object Drawing.
	void drawPoints(const MyGeometry* pGeometry);
	void drawLine(const MyGeometry* pGeometry, GLenum eMode);
	void drawQuad(const MyGeometry* pGeometry);
	void drawCubic(const MyGeometry* pGeometry);

	// Specific Drawing Functions (Tangents and Control Points)
	void drawTangents(const MyGeometry* pGeometry, bool bQuad);
	void drawControlPoints(const MyGeometry* pGeometry, float fPointSize, bool bQuad);

	// Protected Variables.
	bool m_bUpdated;
	int m_iLevels;
	vector<GLfloat> m_vVertices;
	vector<GLfloat> m_vColors;
	float m_fTangentColors[RGB_MAX];
	float m_fColors[NUM_COLORS][RGB_MAX];
	static int m_iWidth, m_iHeight;

	// Protected Functions
	// Functions for cleaner vertex & color adding code
	void addColorVertex(GLfloat fR, GLfloat fG, GLfloat fB);
	void addPosVertex(GLfloat fX, GLfloat fY);
	void clearBuffers() { m_vVertices.clear(); m_vColors.clear(); }
	void setRGB(int iColorIndex, eRGBEnum eRGB, float fVal);
};

