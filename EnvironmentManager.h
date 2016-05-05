#pragma once
#include "stdafx.h"
#include "Object3D.h"
#include "Light.h"

// Environment Manager
// Manages all 3D objects in an environment
// Given a ray, will determine what the ray intersects
//	with and will return the color of the object of intersection.
// Written by: James Coté
class EnvironmentManager
{
public:
	static EnvironmentManager* getInstance();
	~EnvironmentManager();

	// Clears the Environment so a new one can be loaded.
	void purgeEnvironment();
	void killObject( long lID );
	void killLight( long lID );
	void debugEnvironment();

	// Returns the Color for a given ray
	bool getColor( const Vector3D* pCameraPos, const Vector3D* pRay, Vector3D* r_pColor );

private:
	EnvironmentManager();
	static EnvironmentManager* m_pInstance;

	// Object Managing
	vector<Object3D*>	m_pObjects;
	vector<Light*>		m_pLights;

	// Declare friend class for Objects and lights so they can add themselves to the manager.
	friend class Object3D;
	friend class Light;
	// Set-up Utility - Add an object to the Environment.
	// Accessable only by the manager and Objects themselves.
	void addObject( Object3D* pNewObject );
	void addLight( Light* pNewLight );

	// Private Methods for Lighting
	void parseReflection( const Vector3D* pPixelPos, 
						  const Vector3D* pNormal, 
						  const Vector3D* pCameraPos,
						  const Object3D* vIntersectingObj,
						  Vector3D* r_pColorResult,
						  int iIteration );
	bool getIntersection( const Vector3D& vPos, const Vector3D& vRay, 
						  Object3D** r_pIntersectingObj, Vector3D* r_pNormal, float* r_fT );
};

