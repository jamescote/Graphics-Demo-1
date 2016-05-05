#pragma once
#include "GraphicsHeader.h"

#define SIZE_2D 3
#define C_X 0
#define C_Y 1
#define C_H 2

class Vector2D; // Declare Existance of Vector2D.

class Matrix2D
{
public:
	Matrix2D();
	~Matrix2D();

	// resets the Matrix to default
	void resetMatrix();

	void setRotation(float fDegrees);
	void setScale(float fScale);
	void setTranslation(const Vector2D* vTransVect);

	// Matrix Operators
	// Assignment Operator
	void operator= (const Matrix2D* mRHS);

	// Matrix Multiplication
	void operator*= (const Matrix2D& mRHS);
	Matrix2D operator* (const Matrix2D& mRHS);

	// Get an element from the array.
	float getElement(int x, int y) const;

	// transpose Matrix
	void transposeMatrix();

private:
	float m_fVals[SIZE_2D][SIZE_2D];
};

