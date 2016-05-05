#include "Sphere.h"


// Constructor for Spheres
Sphere::Sphere( const Vector3D* pPos, 
				const Vector3D* pColor, 
				const Vector3D* vSpec,
				float fP,
				float fRef, 
				float fRadius,
				long lID ) 
	: Object3D( pPos, pColor, vSpec, fP, fRef, lID )
{
	m_fRadius = fRadius;
}

// Destructor
Sphere::~Sphere()
{
	// Nothing to Destruct
}

// Overridden Debug Output
string Sphere::getDebugOutput()
{
	string sOutput = "";

	sOutput += getType();
	sOutput += "/ID:" + to_string( m_lID );
	sOutput += "/Position:" + m_pPosition.getDebugOutput();
	sOutput += "/Radius:" + to_string( m_fRadius );
	sOutput += "/Color:" + m_pColor.getDebugOutput();

	return sOutput;
}

// Intersection for a Sphere
bool Sphere::intersect( const Vector3D* pCameraPos,
						const Vector3D* pRay,
						Vector3D* r_pNormal,
						float* fT )
{
	// Intersection equation:
	//		(ray - center) * (ray - center) - raidus² = 0, (solve for t)
	//		Creates quadratic equation: (-b +/- sqrt(b²-4ac)) / 2a
	// Algorithm found: http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/minimal-ray-tracer-rendering-spheres
	Vector3D vToCam = (*pCameraPos - m_pPosition);
	float a = ( (*pRay) * (*pRay) );
	float b = ((*pRay) * vToCam)* 2.f;
	float c = (vToCam * vToCam) - (m_fRadius * m_fRadius);
	float fT0, fT1 = 0.f;
	Vector3D pNormal;
	bool bReturnVal = false;

	// If ray cannot intersect, then stop
	if ( solveQuadratic( a, b, c, &fT0, &fT1 ) )
	{
		if ( fT0 > fT1 )	// Evaluate closest intersection
			swap( fT0, fT1 );

		if ( fT0 < 0 )		// If lowest is negative, ignore it
			fT0 = fT1;

		if ( fT0 > 0.f )	// If evaluated intersection is positive (valid), proceed
		{
			bReturnVal = true;

			*fT = fT0;
			(*r_pNormal) =  (((*pCameraPos) + (*pRay * (*fT))) - m_pPosition) *		// Vector from Sphere Center to Intersection
							(1.f/ m_fRadius);		
			(*r_pNormal).normalize();
		}
	}

	return bReturnVal;
}

// Applies Quadratic formula to the given a,b,c values
//	Returns false if no real solution can be found
//	Returns true and 2 possible t-values for the equation
bool Sphere::solveQuadratic( float a, float b, float c, float* t0, float *t1 )
{
	bool bSolutionExists = false;
		
	if ( 0.f != a )
	{
		float fDes = (b*b) - (4.f*a*c);

		if ( fDes >= 0.f )
		{
			float fDenom = 2.f * a;
			float fSq = sqrtf( fDes );

			*t0 = (-b + fSq) / fDenom;
			*t1 = (-b - fSq) / fDenom;
			bSolutionExists = true;
		}
	}


	return bSolutionExists;
}