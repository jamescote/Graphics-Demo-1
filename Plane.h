#pragma once
#include "Object3D.h"
class Plane :
	public Object3D
{
public:
	~Plane();

	// Overridden intersect function
	bool intersect( const Vector3D* pCameraPos,
					const Vector3D* pRay,
					Vector3D* r_pNormal,
					float* fT );

	// Overridden Type Output
	string getType() { return "Plane"; }
	
	// Overridden Debug Output
	string getDebugOutput();

private:
	Plane( const Vector3D* pNormal,
		   const Vector3D* pPosition,
		   const Vector3D* pColor,
		   const Vector3D* vSpec,
		   float fP,
		   float fRef,
		   long lID );
	Plane( const Plane* pNewPlane );  // Protected Copy Constructor

	// Normal of the Plane.
	Vector3D m_pNormal;

	// Declare Object_Factory as a Friend to be sole progenitor of Planes
	friend class Object_Factory;
};

