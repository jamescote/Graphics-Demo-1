#pragma once
#include "stdafx.h"
#include "Vector3D.h"
#include "Matrix3D.h"
#include "EnvironmentManager.h"
#include "ImageBuffer.h"

// Camera Class
// Positions a camera in the world that can render
//	what it sees.
class Camera
{
public:
	Camera(int iHeight, int iWidth);
	~Camera();

	// Generate rays for the image and stores ray values
	//	into a buffer to use for drawing.
	void renderScene();
	void snapShotScene();

	// Updating Functions
	void updateHxW( int iHeight, int iWidth );
	void update() { m_bUpdated = true; }

private:
	float m_iViewPortTheta[2];
	float m_fDistToVP, m_fHghtAdj, m_fWdthAdj;
	int m_iHeight, m_iWidth, m_iHlfHght, m_iHlfWdth;
	Vector3D m_pPosition;
	Matrix3D m_pCameraSpace;
	EnvironmentManager* m_pEnvMngr;
	ImageBuffer m_pImgBuffr;
	bool m_bUpdated;

	void calcVPDistance();
	void evaluatePixel( float fX, float fY );

	vector<GLfloat> m_pPixelPositions, m_pPixelColors;
};

