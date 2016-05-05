#pragma once

#include "GraphicsHeader.h"
#include "Graphic.h"
#include "Vector2D.h" 
#include "Matrix2D.h"

#define NUM_TEXTURE_COORDS 4

class Image :
	public Graphic
{
public:
	Image(int iHeight, int iWidth, const string& sImage);
	~Image();

	void genVertices(const MyGeometry* pGeometry);
	void genTextureVerts(float** pVStart, int pVSize);
	void draw();

	void bindTextData(GLsizeiptr* iPtr, void** data);
	//void bindSubData();
	GLuint getTextureName() { return m_pTexture.textureName; }

	// Functions to perform on Image:
	/// Rotate
	void rotateImg(const Vector2D* vDirection);

	/// Move
	void translateImg(const Vector2D* vDirection);

	/// Zoom
	void zoomImg(float fOffset);

	/// Reset
	void resetImg();

private:
	MyTexture m_pTexture;
	Vector2D m_pVerts[NUM_TEXTURE_COORDS];
	Matrix2D m_pModMatrix;
	Matrix2D m_pModelSpace;
	GLuint m_pTextCoords[NUM_TEXTURE_COORDS][2];

	void normalizeWindowPos(float *fX, float *fY);
};

