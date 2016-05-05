#include "Matrix3D.h"
#include "Vector3D.h"
#include "stdafx.h"

#define PI 3.14159265f

// constructor :: Initialize to Identity Matrix
Matrix3D::Matrix3D()
{
	resetMatrix();
}

// Overloaded Constructor to set starting Values of Matrix;
Matrix3D::Matrix3D( Vector3D pX, Vector3D pY, Vector3D pZ )
{
	m_fVals[C_X] = pX;
	m_fVals[C_Y] = pY;
	m_fVals[C_Z] = pZ;
}

// Destructor
Matrix3D::~Matrix3D()
{
	// Nothing to Destruct
}

// resets Matrix
void Matrix3D::resetMatrix()
{
	m_fVals[C_X] = Vector3D(1.f, 0.f, 0.f); 
	m_fVals[C_Y] = Vector3D(0.f, 1.f, 0.f); 
	m_fVals[C_Z] = Vector3D(0.f, 0.f, 1.f); 
}

// add a rotation matrix about the specified axis
void Matrix3D::setRotation(float fDegrees, int iAxis)
{	 
	float fTheta = fDegrees / PI;
	float fCos = cos(fTheta);
	float fSin = sin(fTheta);

	switch (iAxis)
	{
	case C_X:
		m_fVals[C_Y] = Vector3D( getElement( C_Y, C_X ), fCos, -fSin );
		m_fVals[C_Z] = Vector3D( getElement( C_Z, C_X ), fSin, fCos );
		break;
	case C_Y:
		m_fVals[C_X] = Vector3D( fCos, getElement( C_X, C_Y ), fSin );
		m_fVals[C_Z] = Vector3D( -fSin, getElement( C_Z, C_Y ), fCos );
		break;
	case C_Z:
		m_fVals[C_X] = Vector3D( fCos, -fSin, getElement( C_X, C_Z ) );
		m_fVals[C_Y] = Vector3D( fSin, fCos, getElement( C_Y, C_Z ) );
		break;
	default:
		cout << "Error: Unable to rotate 3D Matrix by the " << iAxis << " degree." << endl;
		break;
	}
}	 
	 
// add scale matrix.
void Matrix3D::setScale(float fScale)
{	
	fScale *= SCALE_MOD;
	m_fVals[C_X] *= fScale;
	m_fVals[C_Y] *= fScale;
	m_fVals[C_Z] *= fScale;
}	 
	 
// add Direction to Z
void Matrix3D::setTranslation(const Vector3D* vTransVect)
{
	m_fVals[C_Z] += *vTransVect;
}	

// Set This Matrix to another using the '=' operator
void Matrix3D::operator= (const Matrix3D* mRHS)
{
	for ( int i = C_X; i < NUM_COLS; ++i )
		m_fVals[i] = mRHS->m_fVals[i];
}

// Multiply 2 Matrices.
void Matrix3D::operator*=(const Matrix3D* mRHS)
{
	Matrix3D mTempMatrix = *this;
	Vector3D mTempVect;

	for ( int i = 0; i < NUM_COLS; ++i )
	{
		mTempVect = Vector3D( m_fVals[C_X].getElement( i ), m_fVals[C_Y].getElement( i ), m_fVals[C_Z].getElement( i ) );
		for ( int j = 0; j < NUM_COLS; ++j )
			m_fVals[j].setElement( i, (mTempVect * mRHS->m_fVals[j]) );
	}
}

// Multiply 2 Matrices.
Matrix3D Matrix3D::operator*(const Matrix3D* mRHS)
{
	Matrix3D mNewMatrix = (*this);
	Vector3D mTempVect;
	for ( int i = 0; i < NUM_COLS; ++i )
	{
		mTempVect = Vector3D( m_fVals[C_X].getElement( i ), m_fVals[C_Y].getElement( i ), m_fVals[C_Z].getElement( i ) );
		for ( int j = 0; j < NUM_COLS; ++j )
			mNewMatrix.m_fVals[j].setElement( i, (mTempVect * mRHS->m_fVals[j]) );
	}

	return mNewMatrix;
}

// Get an element from the array.
// Assumes NxN matrix
float Matrix3D::getElement(int x, int y) const
{
	float fReturnVal = 0.f;
	if (x >= 0 && x < NUM_COLS && y >= 0 && y < NUM_COLS)
		fReturnVal = m_fVals[x].getElement(y);

	return fReturnVal;
}