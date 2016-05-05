#pragma once
#include <math.h>

class Matrix2D; // Declare existance of Matrix2D.

class Vector2D
{
public:
	Vector2D();
	Vector2D(float fX, float fY, float fH = 1.f);
	~Vector2D();

	// Operator Overloads
	/// Assignment Operator
	void operator= (const Vector2D& mRHS);

	/// Scalar Multiplication
	void operator*= (float fScalar);
	Vector2D operator* (float fScalar);

	/// Vector Addition
	Vector2D operator+ (const Vector2D& vRHS);
	void operator+= (const Vector2D& vRHS);
	
	// Vector Subtraction
	Vector2D operator- (const Vector2D& vRHS);
	void operator-= (const Vector2D& vRHS);

	// Modify Vector by a 2D Matrix:
	//		Matrix * Vector
	void modVect(const Matrix2D* mModifier);

	// Getters/Setters
	float getX() const { return m_pX; }
	void setX(float fX) { m_pX = fX; }
	float getY() const { return m_pY; }
	void setY(float fY) { m_pY = fY; }
	float getH() const { return m_pH; }
	void setH(float fH) { m_pH = fH; }

	// Calculate Length of Vector
	float length() { return sqrt((m_pX*m_pX) + (m_pY* m_pY)); }

private: 
	// [X, Y, H]
	float m_pX, m_pY, m_pH;
};

