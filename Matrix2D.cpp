#include "Matrix2D.h"
#include "Vector2D.h"
#include "stdafx.h"

#define PI 3.14159265f

// NOTE: Matrix designed to only have one transform at a time.  In order to have multiple transforms on one Matrix, multiply Matrices together.

// constructor :: Initialize to Identity Matrix
Matrix2D::Matrix2D()
{
	resetMatrix();
}

// Destructor
Matrix2D::~Matrix2D()
{
	// Nothing to Destruct
}

// resets Matrix to identity matrix
void Matrix2D::resetMatrix()
{
	m_fVals[C_X][C_X] = 1.f; m_fVals[C_X][C_Y] = 0.f; m_fVals[C_X][C_H] = 0.f;
	m_fVals[C_Y][C_X] = 0.f; m_fVals[C_Y][C_Y] = 1.f; m_fVals[C_Y][C_H] = 0.f;
	m_fVals[C_H][C_X] = 0.f; m_fVals[C_H][C_Y] = 0.f; m_fVals[C_H][C_H] = 1.f;
}

// Set up Matrix as a Rotation Matrix
void Matrix2D::setRotation(float fDegrees)
{
	float fTheta = fDegrees / PI;
	float fCos = cos(fTheta);
	float fSin = sin(fTheta);

	m_fVals[C_X][C_X] = fCos;
	m_fVals[C_X][C_Y] = -fSin;
	m_fVals[C_Y][C_X] = fSin;
	m_fVals[C_Y][C_Y] = fCos;
}

// Set 2D Scale on Matrix
void Matrix2D::setScale(float fScale)
{
	fScale *= SCALE_MOD;
	m_fVals[C_X][C_X] += fScale;
	m_fVals[C_Y][C_Y] += fScale;
}

// add Direction to Homogeneous coordinates
// | 00 01 vx |
// | 10 11 vy |
// | 20 21  1 |
void Matrix2D::setTranslation(const Vector2D* vTransVect)
{
	m_fVals[C_X][C_H] += vTransVect->getX();
	m_fVals[C_Y][C_H] += vTransVect->getY();
}


// Set This Matrix to another using the '=' operator
void Matrix2D::operator= (const Matrix2D* mRHS)
{
	for (int i = 0; i < SIZE_2D; ++i)
		for (int j = 0; j < SIZE_2D; ++j)
			this->m_fVals[i][j] = mRHS->m_fVals[i][j];
}

// Multiply 2 Matrices.
Matrix2D Matrix2D::operator*(const Matrix2D& mRHS)
{
	Matrix2D newMatrix = (*this);

	for (int i = 0; i < SIZE_2D; ++i)
		for (int j = 0; j < SIZE_2D; ++j)
		{
			newMatrix.m_fVals[i][j] =  (this->m_fVals[i][C_X] * mRHS.m_fVals[C_X][j]);
			newMatrix.m_fVals[i][j] += (this->m_fVals[i][C_Y] * mRHS.m_fVals[C_Y][j]);
			newMatrix.m_fVals[i][j] += (this->m_fVals[i][C_H] * mRHS.m_fVals[C_H][j]);
		}

	return newMatrix;
}

// Multiply a Matrix with this Matrix.
void Matrix2D::operator*=(const Matrix2D& mRHS)
{
	Matrix2D mTempMatrix = (*this);

	for (int i = 0; i < SIZE_2D; ++i)
		for (int j = 0; j < SIZE_2D; ++j)
		{
			this->m_fVals[i][j] =  (mTempMatrix.m_fVals[i][C_X] * mRHS.m_fVals[C_X][j]);
			this->m_fVals[i][j] += (mTempMatrix.m_fVals[i][C_Y] * mRHS.m_fVals[C_Y][j]);
			this->m_fVals[i][j] += (mTempMatrix.m_fVals[i][C_H] * mRHS.m_fVals[C_H][j]);
		}							
}

// Get an element from the array.
float Matrix2D::getElement(int x, int y) const
{
	float fReturnVal = 0.f;
	if (x >= 0 && x < SIZE_2D && y >= 0 && y < SIZE_2D)
		fReturnVal = m_fVals[x][y];

	return fReturnVal;
}

// Transpose Matrix
void Matrix2D::transposeMatrix() 
{
	float fTemp;

	for (int x = C_X; x < SIZE_2D; ++x)
		for (int y = C_X; y < SIZE_2D; ++y)
		{
			fTemp = m_fVals[x][y];
			m_fVals[x][y] = m_fVals[y][x];
			m_fVals[y][x] = fTemp;
		}
}
