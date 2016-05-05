#include "Plane.h"

// Constructor
Plane::Plane( const Vector3D* pNormal,
			  const Vector3D* pPosition,
			  const Vector3D* pColor,
			  const Vector3D* vSpec,
			  float fP,
			  float fRef,
			  long lID ) : Object3D( pPosition, pColor, vSpec, fP, fRef, lID )
{
	m_pNormal = (*pNormal);
	m_pNormal.normalize();
}

// Destructor
Plane::~Plane()
{
	// Nothing to Destruct
}

// Overridden Debug Output
string Plane::getDebugOutput()
{
	string sOutput = "";

	sOutput += getType();
	sOutput += "/ID:" + to_string( m_lID );
	sOutput += "/Normal:" + m_pNormal.getDebugOutput();
	sOutput += "/P:" + m_pPosition.getDebugOutput();
	sOutput += "/Color:" + m_pColor.getDebugOutput();

	return sOutput;
}

// Intersection for Plane
bool Plane::intersect( const Vector3D* pCameraPos,
					   const Vector3D* pRay,
					   Vector3D* r_pNormal,
					   float* fT )
{
	// Intersection = (CamPos + t*ray - p) * normal
	//		Solve for t.
	// = t*ray * normal = (CamPos - p) * normal
	// t = ((CamPos - p) * normal) / (ray * normal)
	float fDenom = (m_pNormal * (*pRay));
	float fNumerator;
	bool bReturnValue = false;

	// Assume they don't intersect as denominator gets closer to 0.
	if ( fDenom < -1e-6 )
	{
		// Calculate Numerator
		fNumerator = (m_pPosition - (*pCameraPos)) * m_pNormal;
		(*fT) = fNumerator / fDenom;		// Calculate t distance away from camera

		// So long as the intersection is in front of the camera
		if ( (*fT) >= 0 )
		{
			bReturnValue = true;			// Report Success
			(*r_pNormal)	 = m_pNormal;	// Pass Normal.
		}
	}

	// Return Result
	return bReturnValue;
}
