#pragma once
#include "Object3D.h"

class Sphere :
	public Object3D
{
public:
	~Sphere();

	// Overridden Intersect function
	bool intersect( const Vector3D* pCameraPos, 
					const Vector3D* pRay,
					Vector3D* r_pNormal,
					float* fT );

	// Overridden Type Output
	string getType() { return "Sphere"; }

	// Overridden Debug Output
	string getDebugOutput();
	
private:
	// Constructor for Spheres
	Sphere( const Vector3D* pPos, 
			const Vector3D* pColor, 
			const Vector3D* vSpec,
			float fP,
			float fRef,
			float fRadius,
			long lID );
	Sphere( const Sphere* pNewSphere );	// Protected Copy Constructor.
	bool solveQuadratic( float a, float b, float c, float* t0, float *t1 );

	// Private Sphere Variables
	float m_fRadius;

	// Declare Object_Factory as a Friend to be sole progenitor of Spheres
	friend class Object_Factory;
};

