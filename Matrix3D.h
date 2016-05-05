#pragma once
#include "GraphicsHeader.h"
#include "Vector3D.h"

#define NUM_COLS 3
#define C_X 0
#define C_Y 1
#define C_Z 2

class Matrix3D
{
public:
	Matrix3D();
	Matrix3D( Vector3D pX, Vector3D pY, Vector3D pZ );
	~Matrix3D();

	void setRotation(float fDegrees, int iAxis = C_Z);
	void setScale(float fScale);
	void setTranslation(const Vector3D* vTransVect);

	// Matrix Operators
	// Assignment Operator
	void operator= (const Matrix3D* mRHS);

	// Matrix Multiplication
	void operator*= (const Matrix3D* mRHS);
	Matrix3D operator* (const Matrix3D* mRHS);

	// Get an element from the array.
	float getElement(int x, int y) const;

	// resets Matrix
	void resetMatrix();

private:
	Vector3D m_fVals[NUM_COLS]; // 3 Vector3D Columns
};

