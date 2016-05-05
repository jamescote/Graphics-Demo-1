#include "Object3D.h"
#include "EnvironmentManager.h"

// Default Constructor.
// Initializes to the center of the world and adds
//		itself to the EnvironmentManager
Object3D::Object3D( long lID )
{
	m_pPosition			= Vector3D( 0.f, 0.f, 0.f );
	m_pColor			= Vector3D( 1.f, 1.f, 1.f );
	m_pSpecularColor	= Vector3D( 0.f, 0.f, 0.f );
	m_fReflectivity		= 0.f;
	m_fPVal				= 1.f;
	m_lID				= lID;
	EnvironmentManager::getInstance()->addObject( this );
}

// Overloaded Constructor to set up positions and Color
Object3D::Object3D( const Vector3D* pStartPos, const Vector3D* pColor, const Vector3D* vSpec, float fP, float fRef, long lID ) 
	: Object3D( lID )
{
	m_pPosition			= (*pStartPos);
	m_pColor			= (*pColor);
	m_pSpecularColor	= (*vSpec);
	m_fPVal				= fP;
	m_fReflectivity		= fRef;
}

Object3D::~Object3D()
{
	// Nothing to Destruct.
}
