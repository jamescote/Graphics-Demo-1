#pragma once
#include "stdafx.h"
#include "Vector3D.h"

// Infinitely small point that emits a light source.
//	Managed by the Environment Manager.
class Light
{
public:
	~Light();

	// Calculates Diffuse, Specular and Ambient Light for this particular light source
	void calculateLighting( const Vector3D& vPixelPos,
							const Vector3D& vToCamera,
							const Vector3D& vNormal,
							const Vector3D& vPixelColor,
							const Vector3D* pSpecColor,
							float fSpecExp,
							Vector3D* r_vReturnColor,
							bool bShadow );

	// Get ID of the Light
	long ID() { return m_lID; }

	// Retrieve the Position of the Light
	const Vector3D& getPosition() { return m_pPosition; }

	// Ouptut Debug information for Light
	string getDebugOutput()
	{
		string sOutput = "";

		sOutput += "Light";
		sOutput += "/ID:" + to_string( m_lID );
		sOutput += "/Position:" + m_pPosition.getDebugOutput();
		sOutput += "/Color:" + m_pColor.getDebugOutput();

		return sOutput;
	}

private:
	// Protected Constructors, only accessible by the Object Factory.
	Light( const Vector3D* pPos,
		   const Vector3D* pColor,
		   long lID );
	Light( const Light* newLight );

	// Private Variables
	Vector3D m_pPosition, m_pColor, m_pAmbientColor;
	long m_lID;

	// Shake hands with the Object Factory so that Lights can only be made from the Object Factory.
	friend class Object_Factory;

	// Different Lighting methods
	void calculateDiffuse( const Vector3D& pPixelPos, 
						   const Vector3D& pNormal,
						   const Vector3D& vToLight,
						   Vector3D* r_pDiffColor );
	void calculateSpecular( const Vector3D& vToCamera,
							const Vector3D& vToLight,
							const Vector3D& vNormal,
							const Vector3D& vSpecColor,
							float fSpecExp,
							Vector3D* r_pSpecColor );

};

