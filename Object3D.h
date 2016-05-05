#pragma once

#include "Vector3D.h"
#define MAX_SPECULAR_EXP 1000.f
#define REFLECTIVITY_MOD 0.0001f

class Object3D
{
public:
	~Object3D();

	// Pure Virtual Intersection Function
	// All inherited classes must implement their own
	//		logic for this function.
	virtual bool intersect( const Vector3D* pCameraPos, // Position of Camera
							const Vector3D* pRay,		// Ray from Camera
							Vector3D* r_pNormal,		// Normal of Intersection
							float* fT ) = 0;			// T-Value for Ray
	virtual string getType() = 0;
	virtual string getDebugOutput() = 0;

	// Getters/Setters
	long ID() const { return m_lID; }								// Get ID for the object
	Vector3D getSpecColor() const { return m_pSpecularColor; }	// Get Value for Specular Color
	Vector3D getColor() const { return m_pColor; }				// Get Color of Object
	float getSpecExp() const { return m_fPVal; }					// Get Specular Exponent of Object
	float getReflectivity() const { return m_fReflectivity; }		// Get Reflectivity of Object

protected:
	// Protected Variables
	Vector3D m_pPosition;
	Vector3D m_pColor, m_pSpecularColor;
	long m_lID;
	float m_fPVal, m_fReflectivity;

	// Constructors only accessable by Object Factory.
	Object3D( long lID );
	Object3D( const Vector3D* pStartPos, const Vector3D* pColor,  const Vector3D* vSpec, float fP, float fRef, long lID );
};

