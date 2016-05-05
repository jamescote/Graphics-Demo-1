#include "Light.h"
#include "EnvironmentManager.h"

// Constructor
Light::Light( const Vector3D* pPos,
			  const Vector3D* pColor,
			  long lID)
{
	m_pColor		= (*pColor);
	m_pAmbientColor = Vector3D( 0.4f, .4f, .4f );
	m_pPosition		= (*pPos);
	m_lID			= lID;
	EnvironmentManager::getInstance()->addLight( this );
}

// Destructor
Light::~Light()
{
	// Nothing to Destruct
}

// Given a position of the Pixel to light and the Normal of that pixel,
// Calculate the return color as: c = cRcLmax(0, n*l)
// where:
//	cR = Color of Pixel
//	cL = Color of Light
//  n = Normal of Pixel's surface
//  l = Vector from pixel to the light: (Light Position - Pixel Position.
void Light::calculateDiffuse( const Vector3D& pPixelPos, 
							  const Vector3D& pNormal, 
							  const Vector3D& vToLight,
							  Vector3D* r_pDiffColor )
{
	float fLightIntensity = (pNormal*vToLight);		// Intensity of that Light
	fLightIntensity = max( 0.f, fLightIntensity );		// Ignore negative Intensities.

	// Evaluate RGB intensities.
	float fR_Intensity = m_pColor.getElement( C_X ) * fLightIntensity;
	float fG_Intensity = m_pColor.getElement( C_Y ) * fLightIntensity;
	float fB_Intensity = m_pColor.getElement( C_Z ) * fLightIntensity;

	// Set the color.
	r_pDiffColor->setElement( C_X, fR_Intensity );
	r_pDiffColor->setElement( C_Y, fG_Intensity );
	r_pDiffColor->setElement( C_Z, fB_Intensity );
}

void Light::calculateSpecular( const Vector3D& vToCamera,
							   const Vector3D& vToLight,
							   const Vector3D& vNormal,
							   const Vector3D& vSpecColor,
							   float fSpecExp,
							   Vector3D* r_pSpecColor )
{
	float fCosSigma;

	Vector3D vHVect = vToLight + vToCamera;
	vHVect.normalize();

	// Calculate New Color
	fCosSigma = vHVect * vNormal;
	fCosSigma = max( 0.f, fCosSigma );
	fCosSigma = pow( fCosSigma, fSpecExp );
	(*r_pSpecColor) = m_pColor * fCosSigma;

	r_pSpecColor->setElement( C_X, r_pSpecColor->getElement( C_X ) * vSpecColor.getElement( C_X ) );
	r_pSpecColor->setElement( C_Y, r_pSpecColor->getElement( C_Y ) * vSpecColor.getElement( C_Y ) );
	r_pSpecColor->setElement( C_Z, r_pSpecColor->getElement( C_Z ) * vSpecColor.getElement( C_Z ) );
}

// Calculates Diffuse, Specular and Ambient Light for this particular light source
void Light::calculateLighting( const Vector3D& vPixelPos,
							   const Vector3D& vToCamera,
							   const Vector3D& vNormal,
							   const Vector3D& vPixelColor,
							   const Vector3D* pSpecColor,
							   float fSpecExp,
							   Vector3D* r_vReturnColor,
							   bool bShadow )
{
	Vector3D vCDiffuse;
	Vector3D vCSpecular;
	Vector3D vToLight = m_pPosition - vPixelPos;
	vToLight.normalize();

	if ( !bShadow )
	{
		calculateDiffuse( vPixelPos, vNormal, vToLight, &vCDiffuse );
		calculateSpecular( vToCamera, vToLight, vNormal, (*pSpecColor), fSpecExp, &vCSpecular );

		vCDiffuse += m_pAmbientColor;

		// c = cR( cA + Diffuse ) + Specular
		float fR_Intensity = (vPixelColor.getElement( C_X ) * vCDiffuse.getElement( C_X )) + vCSpecular.getElement( C_X );
		float fG_Intensity = (vPixelColor.getElement( C_Y ) * vCDiffuse.getElement( C_Y )) + vCSpecular.getElement( C_Y );
		float fB_Intensity = (vPixelColor.getElement( C_Z ) * vCDiffuse.getElement( C_Z )) + vCSpecular.getElement( C_Z );

		r_vReturnColor->setElement( C_X, fR_Intensity );
		r_vReturnColor->setElement( C_Y, fG_Intensity );
		r_vReturnColor->setElement( C_Z, fB_Intensity );
	}
	else
	{
		r_vReturnColor->setElement( C_X, m_pAmbientColor.getElement( C_X ) * vPixelColor.getElement( C_X ) );
		r_vReturnColor->setElement( C_Y, m_pAmbientColor.getElement( C_Y ) * vPixelColor.getElement( C_Y ) );
		r_vReturnColor->setElement( C_Z, m_pAmbientColor.getElement( C_Z ) * vPixelColor.getElement( C_Z ) );
	}
}