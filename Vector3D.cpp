#include "Vector3D.h"
#include "Matrix3D.h"

#define X_VAL m_fVals[C_X]
#define Y_VAL m_fVals[C_Y]
#define Z_VAL m_fVals[C_Z]

// Default constructor (set Vector to (0,0)
Vector3D::Vector3D()
{
	X_VAL = Y_VAL = Z_VAL = 0.f;
}

// Overloaded Constructor with specified initialized values (fX, fY, fZ, fH)
Vector3D::Vector3D(float fX, float fY, float fZ)
{
	X_VAL = fX;
	Y_VAL = fY;
	Z_VAL = fZ;
}

Vector3D::~Vector3D()
{
	// Nothing to Destruct
}

float Vector3D::getElement( int iPos ) const
{
	if ( iPos >= C_X && iPos < NUM_VALS )
		return m_fVals[iPos];

	return 0.f;
}

void Vector3D::setElement( int iPos, float fVal )
{
	if ( iPos >= C_X && iPos < NUM_VALS )
		m_fVals[iPos] = fVal;
}

// Multiplies This Vector by a given Matrix
void Vector3D::modVect(const Matrix3D* mModifier)
{
	Vector3D vTempVec = *this;

	this->X_VAL = (mModifier->getElement(0, 0) * vTempVec.X_VAL) + 
				  (mModifier->getElement(0, 1) * vTempVec.Y_VAL) +
				  (mModifier->getElement(0, 2) * vTempVec.Z_VAL);
	this->Y_VAL = (mModifier->getElement(1, 0) * vTempVec.X_VAL) +
				  (mModifier->getElement(1, 1) * vTempVec.Y_VAL) +
				  (mModifier->getElement(1, 2) * vTempVec.Z_VAL);
	this->Z_VAL = (mModifier->getElement(2, 0) * vTempVec.X_VAL) +
				  (mModifier->getElement(2, 1) * vTempVec.Y_VAL) +
				  (mModifier->getElement(2, 2) * vTempVec.Z_VAL);
}

/****************************************************************************\
 * Operator Overloads														*
\****************************************************************************/

// Assignment Operator
void Vector3D::operator= (const Vector3D& mRHS)
{
	X_VAL = mRHS.X_VAL;
	Y_VAL = mRHS.Y_VAL;
	Z_VAL = mRHS.Z_VAL;
}

// Comparative Operator
bool Vector3D::operator== (const Vector3D& mRHS) const
{
	return ( X_VAL == mRHS.X_VAL && 
			 Y_VAL == mRHS.Y_VAL && 
			 Z_VAL == mRHS.Z_VAL );
}

// Subtract a Vector from this vector (W/O modifying this vector)
Vector3D Vector3D::operator- (const Vector3D &vRHS) const
{
	Vector3D vNewVector = *this;

	vNewVector.X_VAL -= vRHS.X_VAL;
	vNewVector.Y_VAL -= vRHS.Y_VAL;
	vNewVector.Z_VAL -= vRHS.Z_VAL;

	return vNewVector;
}

// subtract a vector to this vector
void Vector3D::operator-= (const Vector3D& vRHS)
{
	this->X_VAL -= vRHS.X_VAL;
	this->Y_VAL -= vRHS.Y_VAL;
	this->Z_VAL -= vRHS.Z_VAL;
}

// Add a vector with this vector
Vector3D Vector3D::operator+ (const Vector3D& vRHS) const
{
	Vector3D vNewVector = *this;

	vNewVector.X_VAL += vRHS.X_VAL;
	vNewVector.Y_VAL += vRHS.Y_VAL;
	vNewVector.Z_VAL += vRHS.Z_VAL;

	return vNewVector;
}

// Add a vector to this vector
void Vector3D::operator+= (const Vector3D& vRHS)
{
	this->X_VAL += vRHS.X_VAL;
	this->Y_VAL += vRHS.Y_VAL;
	this->Z_VAL += vRHS.Z_VAL;
}

// Multiply this Vector by a scalar
void Vector3D::operator*= (float fScalar)
{
	this->X_VAL *= fScalar;
	this->Y_VAL *= fScalar;
	this->Z_VAL *= fScalar;
}

// Return a new vector = this vector * a scalar
Vector3D Vector3D::operator* (float fScalar) const
{
	Vector3D vNewVector = *this;

	vNewVector.X_VAL *= fScalar;
	vNewVector.Y_VAL *= fScalar;
	vNewVector.Z_VAL *= fScalar;

	return vNewVector;
}

// Dot Product
float Vector3D::dot( const Vector3D& mRHS ) const
{
	float fReturnVal;
	fReturnVal = X_VAL *  mRHS.X_VAL;
	fReturnVal += Y_VAL * mRHS.Y_VAL;
	fReturnVal += Z_VAL * mRHS.Z_VAL;

	return fReturnVal;
}

float Vector3D::operator* ( const Vector3D& mRHS ) const
{
	return this->dot( mRHS );
}

// Cross Product
Vector3D Vector3D::cross( const Vector3D& mRHS ) const 
{
	float fVals[NUM_VALS];
	fVals[C_X] = Y_VAL*mRHS.Z_VAL - Z_VAL*mRHS.Y_VAL; // u2v3 - u3v2
	fVals[C_Y] = Z_VAL*mRHS.X_VAL - X_VAL*mRHS.Z_VAL; // u3v1 - u1v3
	fVals[C_Z] = X_VAL*mRHS.Y_VAL - Y_VAL*mRHS.X_VAL; // u1v2 - u2v1

	return Vector3D( fVals[C_X], fVals[C_Y], fVals[C_Z] );
}

// Normalize:
void Vector3D::normalize()
{
	float fLength = length();
	for ( int i = 0; i < NUM_VALS; ++i )
		m_fVals[i] /= fLength;
}

// Reflection
Vector3D Vector3D::reflect( const Vector3D* pNormal ) const
{
	Vector3D vSubtractor = ((*pNormal) * ((*this) * (*pNormal)) * 2.f); // 2(d * n)n

	return (*this) - vSubtractor;	// d - 2(d*n)n
}

