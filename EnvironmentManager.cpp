#include "EnvironmentManager.h"

#define INTERSECTION_EPSILON 1e-4	// Minimum intersect distance (so we don't intersect with ourselves)
#define MAX_REFLECTIONS	800

// Initialize Static Instance Variable
EnvironmentManager* EnvironmentManager::m_pInstance = NULL;

EnvironmentManager::EnvironmentManager()
{
	// Constructor.
}

// Gets the instance of the environment manager.
EnvironmentManager* EnvironmentManager::getInstance()
{
	if ( NULL == m_pInstance )
		m_pInstance = new EnvironmentManager();
	return m_pInstance;
}

// Destructor.
EnvironmentManager::~EnvironmentManager()
{
	purgeEnvironment();
}

// Adds object to back of List
void EnvironmentManager::addObject( Object3D* pNewObject )
{
	m_pObjects.push_back( pNewObject );
}

// Adds a Light to back of List
void EnvironmentManager::addLight( Light* pNewLight )
{
	m_pLights.push_back( pNewLight );
}

// Remove Object from List with given ID
void EnvironmentManager::killObject( long lID )
{
	unsigned int i = 0;

	// Iterate to find Object
	while ( i < m_pObjects.size() && NULL != m_pObjects[i] && lID != m_pObjects[i]->ID() )
		++i;

	// Delete Object and remove it from list.
	if ( i < m_pObjects.size() )
	{
		swap( m_pObjects[i], m_pObjects.back() );
		delete m_pObjects.back();
		m_pObjects.pop_back();
	}
}

// Remove Object from List with given ID
void EnvironmentManager::killLight( long lID )
{
	unsigned int i = 0;

	// Iterate to find Object
	while ( i < m_pLights.size() && NULL != m_pLights[i] && lID != m_pLights[i]->ID() )
		++i;

	// Delete Object and remove it from list.
	if ( i < m_pLights.size() )
	{
		swap( m_pLights[i], m_pLights.back() );
		delete m_pLights.back();
		m_pLights.pop_back();
	}
}

// Outputs all the objects in the environment for debugging.
void EnvironmentManager::debugEnvironment()
{
	cout << "Environment:" << endl;
	for ( vector<Object3D*>::iterator pIter = m_pObjects.begin();
		  pIter != m_pObjects.end();
		  ++pIter )
	{
		cout << "\t" << (*pIter)->getDebugOutput() << endl;
	}

	for ( vector<Light*>::iterator pIter = m_pLights.begin();
		  pIter != m_pLights.end();
		  ++pIter )
	{
		cout << "\t" << (*pIter)->getDebugOutput() << endl;
	}

	cout << endl;
}

// Clears out the entire environment
void EnvironmentManager::purgeEnvironment()
{
	// Clean Up objects
	for ( vector<Object3D*>::iterator pIter = m_pObjects.begin();
		  pIter != m_pObjects.end();
		  ++pIter )
	{
		if ( NULL != (*pIter) )
		{
			delete (*pIter);
		}
	}

	// Clean up Lights
	for ( vector<Light*>::iterator pIter = m_pLights.begin();
		  pIter != m_pLights.end();
		  ++pIter )
	{
		if ( NULL != (*pIter) )
		{
			delete (*pIter);
		}
	}

	// Clear the array of Dangling pointers
	m_pObjects.clear();
	m_pLights.clear();
}

// Checks through all Objects for intersections and gets the color for a given ray.
bool EnvironmentManager::getColor( const Vector3D* pCameraPos, const Vector3D* pRay, Vector3D* r_pColor )
{
	// Locals
	bool bIntersect = false;
	vector<Object3D*>::iterator pObjIter = m_pObjects.begin();
	float fT = FLT_MAX;
	Object3D* vIntersectingObj = NULL;
	Vector3D pTempColor;
	Vector3D pSpecColor;
	Vector3D pNormal;
	Vector3D pPixelPosition;

	// find an intersection for this ray.
	bIntersect = getIntersection( (*pCameraPos), (*pRay), &vIntersectingObj, &pNormal, &fT );

	// If there's an intersection, give the Position of the intersection and calculate the color of that intersection.
	if ( bIntersect )
	{
		pPixelPosition = (*pRay * fT);
		parseReflection( &pPixelPosition, &pNormal, pCameraPos, vIntersectingObj, r_pColor, MAX_REFLECTIONS );
	}
			
	return bIntersect;
}

// Evaluate Shadows, lighting and reflection for a given pixel position.
void EnvironmentManager::parseReflection( const Vector3D* pPixelPos, 
										  const Vector3D* pNormal, 
										  const Vector3D* pCameraPos,
										  const Object3D* vIntersectingObj,
										  Vector3D* r_pColorResult,
										  int iIteration)
{
	// Store Current Color.
	(*r_pColorResult) = vIntersectingObj->getColor();
	Vector3D pCurrentColor;
	Vector3D pSpecColor = vIntersectingObj->getSpecColor();
	Vector3D vReflectedRay;
	Vector3D pShadowNormal;
	Vector3D vToLight;
	Vector3D vToCamera = (*pCameraPos) - (*pPixelPos);
	Object3D* vShadowingObj = NULL;
	bool bShadow = false;
	vToCamera.normalize();
	float fT;

	// Calculate Lighting effects for all lights in the environment.
	for ( vector<Light*>::iterator pIter = m_pLights.begin();
		  pIter != m_pLights.end();
		  ++pIter )
	{
		pCurrentColor = (*r_pColorResult);	// Store for next loop.

		// Calculate Shadow
		vToLight = (*pIter)->getPosition() - (*pPixelPos);
		fT = vToLight.length();
		vToLight.normalize();
		bShadow = getIntersection( (*pPixelPos), vToLight, &vShadowingObj, &pShadowNormal, &fT );

		// Evaluate Lighting for Object
		(*pIter)->calculateLighting( (*pPixelPos),
									 vToCamera,
									 (*pNormal),
									 pCurrentColor,
									 &pSpecColor,
									 vIntersectingObj->getSpecExp(),
									 r_pColorResult,
									 bShadow );
		pCurrentColor = (*r_pColorResult);
		vReflectedRay = (vToCamera * -1.f).reflect( pNormal );	// Reflect ray to this pixel over the normal for our reflected ray.
		vReflectedRay.normalize();

		if( 0.f != vIntersectingObj->getReflectivity() )
		{
			Object3D* vNewObj = NULL;
			Vector3D vNewNormal;
			float fS = FLT_MAX;
			bool bIntersect = getIntersection( (*pPixelPos), vReflectedRay, &vNewObj, &vNewNormal, &fS );
			if ( bIntersect && iIteration >= 0 )
			{
				Vector3D vNewPixPos = (*pPixelPos) + vReflectedRay * fS;
				vNewNormal.normalize();
				parseReflection( &vNewPixPos, &vNewNormal, pPixelPos, vNewObj, r_pColorResult, iIteration - 1 );
				(*r_pColorResult) *= vIntersectingObj->getReflectivity();
				(*r_pColorResult) += pCurrentColor;
			}
			else
				(*r_pColorResult) = pCurrentColor;

		}
	}
}

bool EnvironmentManager::getIntersection( const Vector3D& vPos, const Vector3D& vRay, 
										  Object3D** r_pIntersectingObj, Vector3D* r_pNormal, float *r_fT )
{
	vector<Object3D*>::iterator pObjIter = m_pObjects.begin();
	Vector3D vTempNormal;
	bool bIntersect = false;
	bool bReturnVal = false;
	float fTempT = FLT_MAX;

	// Iterate through and determine the closest object
	//		the ray intersects with.
	while ( pObjIter != m_pObjects.end() )
	{
		bIntersect = (*pObjIter)->intersect( &vPos, &vRay, &vTempNormal, &fTempT );
		if ( bIntersect && fTempT > INTERSECTION_EPSILON && fTempT < *r_fT )
		{
			(*r_fT) = fTempT;
			(*r_pIntersectingObj) = (*pObjIter);
			(*r_pNormal) = vTempNormal;
			bReturnVal = true;
		}
		++pObjIter;
	}

	return bReturnVal;
}

