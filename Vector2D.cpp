#include "Vector2D.h"
#include "Matrix2D.h"

// Default constructor (set Vector to (0,0)
Vector2D::Vector2D()
{
	m_pX = m_pY = 0.f;
	m_pH = 1.f;
}

// Overloaded Constructor with specified initialized values (fX, fY)
Vector2D::Vector2D(float fX, float fY, float fH)
{
	m_pX = fX;
	m_pY = fY;
	m_pH = fH;
}

// Destructor
Vector2D::~Vector2D()
{
	// Nothing to Destruct
}

// Multiply this vector by a given Matrix
void Vector2D::modVect( const Matrix2D* mModifier )
{
	Vector2D vReturnVec = *this;

	this->m_pX = (mModifier->getElement(0, 0) * vReturnVec.getX()) +
				 (mModifier->getElement(0, 1) * vReturnVec.getY()) +
				 (mModifier->getElement(0, 2) * vReturnVec.getH());
	this->m_pY = (mModifier->getElement(1, 0) * vReturnVec.getX()) +
				 (mModifier->getElement(1, 1) * vReturnVec.getY()) +
				 (mModifier->getElement(1, 2) * vReturnVec.getH());
	this->m_pH = (mModifier->getElement(2, 0) * vReturnVec.getX()) +
				 (mModifier->getElement(2, 1) * vReturnVec.getY()) +
				 (mModifier->getElement(2, 2) * vReturnVec.getH());
}

// Operator Overloads
// Assignment Operator
void Vector2D::operator= (const Vector2D& mRHS)
{
	m_pX = mRHS.m_pX;
	m_pY = mRHS.m_pY;
}

// Scalar Multiplication : Return new Vector
Vector2D Vector2D::operator* (float fScalar)
{
	Vector2D vNewVector = *this;

	vNewVector.m_pX *= fScalar;
	vNewVector.m_pY *= fScalar;

	return vNewVector;
}

// Scalar Multiplication : on this Vector
void Vector2D::operator*= (float fScalar)
{
	this->m_pX *= fScalar;
	this->m_pY *= fScalar;
}

// Vector Addition : Return New Vector
Vector2D Vector2D::operator+ (const Vector2D& vRHS)
{
	Vector2D vNewVector = *this;

	vNewVector.m_pX += vRHS.m_pX;
	vNewVector.m_pY += vRHS.m_pY;

	return vNewVector;
}

// Vector Addition : On this Vector
void Vector2D::operator+= (const Vector2D& vRHS)
{
	this->m_pX += vRHS.m_pX;
	this->m_pY += vRHS.m_pY;
}

// Vector Subtraction : Return New Vector
Vector2D Vector2D::operator- (const Vector2D& vRHS)
{
	Vector2D vNewVector = *this;

	vNewVector.m_pX -= vRHS.m_pX;
	vNewVector.m_pY -= vRHS.m_pY;

	return vNewVector;
}

// Vector Subtraction : On this Vector
void Vector2D::operator-= (const Vector2D& vRHS)
{
	this->m_pX -= vRHS.m_pX;
	this->m_pY -= vRHS.m_pY;
}
