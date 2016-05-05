#pragma once
#include <math.h>
#include "stdafx.h"

class Matrix3D; // Declare existence of Matrix Class

#define NUM_VALS 3
#define C_X 0
#define C_Y 1
#define C_Z 2

class Vector3D
{
public:
	Vector3D();
	Vector3D(float fX, float fY, float fZ);
	~Vector3D();

	// Operator Overloads
	// Assignment Operator
	void operator= (const Vector3D& mRHS);

	// Comparative Operator
	bool operator== (const Vector3D& mRHS) const;

	// Scalar Multiplication
	void operator*= (float fScalar);
	Vector3D operator* (float fScalar) const;

	// DotProduct
	float dot( const Vector3D& mRHS ) const;
	float operator* (const Vector3D& mRHS) const;

	// Cross Product
	Vector3D cross( const Vector3D& mRHS ) const;

	// Vector Addition
	Vector3D operator+ (const Vector3D& vRHS) const;
	void operator+= (const Vector3D& vRHS);

	// Vector Subtraction
	Vector3D operator- (const Vector3D& vRHS) const;
	void operator-= (const Vector3D& vRHS);

	// Modify Vector by a 3D Matrix:
	//		Matrix * Vector
	void modVect(const Matrix3D* mModifier);

	// Getters/Setters
	float getElement( int iPos ) const;
	void setElement( int iPos, float fVal );

	// Normalize Vector (Vec/Length of Vec)
	void normalize();

	// Return Length of Vector
	float length() { return sqrt( (m_fVals[C_X] * m_fVals[C_X]) + (m_fVals[C_Y] * m_fVals[C_Y]) + (m_fVals[C_Z] * m_fVals[C_Z]) ); }

	// Reflect this Vector over a normal and return the new vector
	Vector3D reflect( const Vector3D* pNormal ) const;

	// Return Output for Debugging
	string getDebugOutput()
	{
		return "{" + to_string( m_fVals[C_X] ) + ", " +
					 to_string( m_fVals[C_Y] ) + ", " +
					 to_string( m_fVals[C_Z] ) + "}";
	}

private:
	float m_fVals[NUM_VALS];
};

