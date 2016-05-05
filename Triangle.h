#pragma once
#include "Object3D.h"

#define P1 0
#define P2 1
#define P3 2

class Triangle :
	public Object3D
{
public:
	~Triangle();

	// Overridden intersect function
	bool intersect( const Vector3D* pCameraPos,
					const Vector3D* pRay,
					Vector3D* r_pNormal,
					float* fT );
	
	// Overridden Type Output
	string getType() { return "Triangle"; }

	// Overridden Debug Output
	string getDebugOutput();

private:
	// Only Accessable by Object Factory
	Triangle( const Vector3D* pPos1,
			  const Vector3D* pPos2,
			  const Vector3D* pPos3,
			  const Vector3D* pColor,
			  const Vector3D* vSpec,
			  float fP,
			  float fRef,
			  long lID );
	Triangle( const Triangle* pNewTriangle );  // Protected Copy Constructor

	// 3 Positions for the Triangle
	Vector3D m_pPositions[3];

	// Declare Object_Factory as a Friend to be sole progenitor of Triangles
	friend class Object_Factory;
};

