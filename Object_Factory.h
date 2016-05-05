#pragma once

#include "stdafx.h"
#include "Object3D.h"
#include "Light.h"

// Solely Generates Objects and assigns IDs to them.
class Object_Factory
{
public:
	static Object_Factory* getInstance();
	~Object_Factory();

	// Creation Functions
	Object3D* createSphere( const Vector3D* pStartPos,
							const Vector3D* pColor,
							const Vector3D* vSpecular,
							float fP,
							float fRef,
							float fRadius );
	Object3D* createPlane( const Vector3D* pNormal,
						   const Vector3D* pPosition,
						   const Vector3D* pColor,
						   const Vector3D* vSpecular,
						   float fP,
						   float fRef );
	Object3D* createTriangle( const Vector3D* pPos1,
							  const Vector3D* pPos2,
							  const Vector3D* pPos3,
							  const Vector3D* pColor,
							  const Vector3D* vSpecular,
							  float fP,
							  float fRef );
	Light* createLight( const Vector3D* pPos,
						const Vector3D* pColor );

	void loadFromFile( string sFileName );

private:
	// Singleton Implementation
	Object_Factory();
	static Object_Factory* m_pInstance;

	long m_lNextID;
	long getNewID() { return ++m_lNextID; }
};

