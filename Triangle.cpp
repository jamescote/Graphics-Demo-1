#include "Triangle.h"

#define VERTEX_0 m_pPositions[P1]
#define VERTEX_1 m_pPositions[P2]
#define VERTEX_2 m_pPositions[P3]
#define EPSILON	1e-6

// Constructor.
Triangle::Triangle( const Vector3D* pPos1,
					const Vector3D* pPos2,
					const Vector3D* pPos3,
					const Vector3D* pColor,
					const Vector3D* vSpec,
					float fP,
					float fRef,
					long lID ) : Object3D( pPos1, pColor, vSpec, fP, fRef, lID )
{
	m_pPositions[P1] = (*pPos1);
	m_pPositions[P2] = (*pPos2);
	m_pPositions[P3] = (*pPos3);
}

// Destructor
Triangle::~Triangle()
{
	// Nothing to Destruct
}

// Overridden Debug Output
string Triangle::getDebugOutput()
{
	string sOutput = "";

	sOutput += getType();
	sOutput += "/ID:" + to_string( m_lID );
	sOutput += "/P1:" + m_pPositions[P1].getDebugOutput();
	sOutput += "/P2:" + m_pPositions[P2].getDebugOutput();
	sOutput += "/P3:" + m_pPositions[P3].getDebugOutput();
	sOutput += "/Color:" + m_pColor.getDebugOutput();

	return sOutput;
}

// Intersection for Triangles
bool Triangle::intersect( const Vector3D* pCameraPos,
						  const Vector3D* pRay,
						  Vector3D* r_pNormal,
						  float* fT )
{
	// Using Möller-Trumbore algorithm:
	//		http://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
	bool bReturnVal = false;
	Vector3D v0v1 = VERTEX_1 - VERTEX_0;
	Vector3D v0v2 = VERTEX_2 - VERTEX_0;
	Vector3D pVec = pRay->cross( v0v2 );
	float fDet = v0v1 * pVec;

	if ( fDet >= EPSILON ) // Only draw if not backfacing
	{
		float fDetInv = 1.f / fDet;
		Vector3D tVec = (*pCameraPos) - VERTEX_0;
		float fU = (tVec * pVec) * fDetInv;
		if ( fU >= 0.f && fU <= 1.f )			// No intersection if u < 0 or u > 1
		{
			Vector3D qVec = tVec.cross( v0v1 );
			float fV = ((*pRay) * qVec) * fDetInv;
			if ( fV >= 0.f && (fU + fV) <= 1.f ) // No intersection if v < 0 or u + v > 1
			{
				*fT = (v0v2 * qVec) * fDetInv;			// Calculate t
				(*r_pNormal)	= v0v1.cross( v0v2 );	// Pass back normal of Triangle
				(*r_pNormal).normalize();
				bReturnVal = true;		// Success
			}
		}
	}

	return bReturnVal;
}
