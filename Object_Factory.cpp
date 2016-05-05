#include "Object_Factory.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"
#include <sstream>
#include <iterator>

/***********\
 * Defines *
\***********/
#define MAX_CHARS_PER_LINE 256
#define COMMENT_CHAR '#'

// Singleton Declaration
Object_Factory* Object_Factory::m_pInstance = NULL;

// Private Constructor
Object_Factory::Object_Factory()
{
	m_lNextID = 0;
}

// Returns the singleton instance of the Object Factory
Object_Factory* Object_Factory::getInstance()
{
	if ( NULL == m_pInstance )
		m_pInstance = new Object_Factory();

	return m_pInstance;
}


Object_Factory::~Object_Factory()
{
	// Nothing to Destruct
}

// Creation Functions
// Create a Sphere given a starting position, color and radius
Object3D* Object_Factory::createSphere( const Vector3D* pStartPos,
										const Vector3D* pColor,
										const Vector3D* vSpecular,
										float fP,
										float fRef,
										float fRadius )
{
	return new Sphere( pStartPos,
					   pColor,
					   vSpecular,
					   fP,
					   fRef,
					   fRadius,
					   getNewID() );
}

// Create a Plane given a normal, a position on the plane and a color
Object3D* Object_Factory::createPlane( const Vector3D* pNormal,
									   const Vector3D* pPosition,
									   const Vector3D* pColor,
									   const Vector3D* vSpecular,
									   float fP,
									   float fRef )
{
	return new Plane( pNormal,
					  pPosition,
					  pColor,
					  vSpecular,
					  fP,
					  fRef,
					  getNewID() );
}

// create a Triangle given 3 positions and a color.
Object3D* Object_Factory::createTriangle( const Vector3D* pPos1,
										  const Vector3D* pPos2,
										  const Vector3D* pPos3,
										  const Vector3D* pColor,
										  const Vector3D* vSpecular,
										  float fP,
										  float fRef )
{
	return new Triangle( pPos1,
						 pPos2,
						 pPos3,
						 pColor,
						 vSpecular, 
						 fP,
						 fRef,
						 getNewID() );
}

Light* Object_Factory::createLight( const Vector3D* pPos,
									const Vector3D* pColor )
{
	return new Light( pPos,
					  pColor,
					  getNewID() );
}

/**************************************************************************\
 * File Reading                                                           *
\**************************************************************************/

void Object_Factory::loadFromFile( string sFileName )
{
	ifstream inFile;
	string sBuffer;
	vector< string > sData;
	string sIndicator, sParser;
	Vector3D vIn1, vIn2, vIn3, vSpec, vColor;
	float fP, fRef;

	// Open File
	inFile.open( sFileName );

	// Parse File if opened properly
	if ( inFile.good() )
	{
		// Parse to eof
		while ( !inFile.eof() )
		{
			// Get next line to parse
			getline( inFile, sBuffer );

			// Determine if we care about the line
			if ( "" != sBuffer && COMMENT_CHAR != sBuffer[0] )
			{
				// Determine keyword for the segment
				istringstream sStream( sBuffer );
				sStream >> sIndicator;

				// Store Data for the segment
				sData.clear();
				do
				{
					getline( inFile, sBuffer );	// Next line of Data
					sStream.str( sBuffer );		// Parse next Line
					sStream.clear();			// Clear Stream Flags
					while ( getline( sStream, sParser, ' ') )	// Extrapolate Data
						if ( "" != sParser )					// Avoid Garbage
							sData.push_back( sParser );
				} while ( "}" != sData.back() );				// Repeat until end delimiter

				if ( "sphere" == sIndicator )			// Parse Sphere
				{
					vColor	= Vector3D( stof( sData[0] )/*R*/, stof( sData[1] )/*G*/, stof( sData[2] )/*B*/ );	// Color of Sphere
					vSpec	= Vector3D( stof( sData[3] )/*R*/, stof( sData[4] )/*G*/, stof( sData[5] )/*B*/ );	// Specular Color of Sphere
					fP		= stof( sData[6] );																	// Specular Exponent
					fRef	= stof( sData[7] );																	// Reflectiveness value
					vIn1	= Vector3D( stof( sData[8] )/*X*/, stof( sData[9] )/*Y*/, stof( sData[10] )/*Z*/ );	// Position
					createSphere( &vIn1, &vColor, &vSpec, fP, fRef, stof( sData[11] ) );
				}
				else if ( "plane" == sIndicator )		// Parse Plane
				{
					vColor	= Vector3D( stof( sData[0] )/*R*/, stof( sData[1] )/*G*/, stof( sData[2] )/*B*/ );		// Color of Plane
					vSpec	= Vector3D( stof( sData[3] )/*R*/, stof( sData[4] )/*G*/, stof( sData[5] )/*B*/ );		// Specular Color of Plane
					fP		= stof( sData[6] );																		// Specular Exponent
					fRef	= stof( sData[7] );																	// Reflectiveness value
					vIn1	= Vector3D( stof( sData[8] )/*X*/, stof( sData[9] )/*Y*/, stof( sData[10] )/*Z*/ );		// Normal
					vIn2	= Vector3D( stof( sData[11] )/*X*/, stof( sData[12] )/*Y*/, stof( sData[13] )/*Z*/ );	// Position on Plane
					createPlane( &vIn1, &vIn2, &vColor, &vSpec, fP, fRef );
				}
				else if ( "triangle" == sIndicator )	// Parse Triangle
				{
					vColor	= Vector3D( stof( sData[0] )/*R*/, stof( sData[1] )/*G*/, stof( sData[2] )/*B*/ );		// Color of Triangle
					vSpec	= Vector3D( stof( sData[3] )/*R*/, stof( sData[4] )/*G*/, stof( sData[5] )/*B*/ );		// Specular Color of Triangle
					fP		= stof( sData[6] );																		// Specular Exponent
					fRef	= stof( sData[7] );																	// Reflectiveness value
					vIn1	= Vector3D( stof( sData[8] )/*X*/, stof( sData[9] )/*Y*/, stof( sData[10] )/*Z*/ );		// Point 1 of Triangle
					vIn2	= Vector3D( stof( sData[11] )/*X*/, stof( sData[12] )/*Y*/, stof( sData[13] )/*Z*/ );	// Point 2 of Triangle
					vIn3	= Vector3D( stof( sData[14] )/*X*/, stof( sData[15] )/*Y*/, stof( sData[16] )/*Z*/ );	// Point 3 of Triangle
					createTriangle( &vIn1, &vIn2, &vIn3, &vColor, &vSpec, fP, fRef );
				}
				else if ( "light" == sIndicator )		// Parse Light
				{
					Vector3D pColor( stof( sData[0] )/*R*/, stof( sData[1] )/*G*/, stof( sData[2] )/*B*/ );
					Vector3D pPos( stof( sData[3] )/*X*/, stof( sData[4] )/*Y*/, stof( sData[5] )/*Z*/ );
					createLight( &pPos, &pColor );
				}

				// Debugging
#ifdef DEBUGGING
				cout << sIndicator << endl;
				for ( unsigned int d = 0; d < sData.size(); ++d )
					cout << sData[d] << endl;
				cout << "\t\t END LINE" << endl;
#endif

			}
		}
	}

	// Close File
	inFile.close();
}
