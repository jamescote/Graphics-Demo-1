#include "Camera.h"

#define HEIGHT_ANGLE 0
#define WIDTH_ANGLE 1
#define HEIGHT_THETA m_iViewPortTheta[HEIGHT_ANGLE]
#define WIDTH_THETA m_iViewPortTheta[WIDTH_ANGLE]
#define PI 3.14159265f
const int c_iVERT_ANGLE = 60;
const int c_iV_ANGLE_HALVED = c_iVERT_ANGLE >> 1;
const int c_iHORIZ_ANGLE = 60;
const int c_iH_ANGLE_HALVED = c_iHORIZ_ANGLE >> 1;

// Constructor
Camera::Camera(int iHeight, int iWidth)
{
	m_pEnvMngr = EnvironmentManager::getInstance();
	HEIGHT_THETA = ((float)c_iV_ANGLE_HALVED * PI) / 180.f;	// Radians
	WIDTH_THETA = ((float)c_iH_ANGLE_HALVED * PI) / 180.f; 	// Radians
	m_pPosition = Vector3D( 0.0f, .0f, .0f); // Init to Origin.
	m_pCameraSpace = Matrix3D( Vector3D(1.f, 0.f, 0.f),		// X-Coord
							   Vector3D(0.f, 1.f, 0.f),		// Y-Coord
							   Vector3D(0.f, 0.f, -1.f) );	// Z-Coord
	m_iHeight = iHeight;	// Set size of Screen
	m_iWidth = iWidth;
	m_bUpdated = true;
	m_pImgBuffr.Initialize();
	calcVPDistance();
}

// Destructor
Camera::~Camera()
{
	// Drop association with Environment Manager
	m_pEnvMngr = NULL;
}

void Camera::calcVPDistance()
{
	// Height/Width halfed and adjusted to middle of pixel.
	m_iHlfHght = m_iHeight >> 1;
	m_iHlfWdth = m_iWidth >> 1;
	m_fHghtAdj = (float)m_iHlfHght - .5f;
	m_fWdthAdj = (float)m_iHlfWdth - .5f;

	// tan = opp/adj
	// We know the opposite (fcHeightAdj/fcWidthAdj), solve for
	//	adj to get distance to Viewport.
	//  adj = opp/tan(θ)
	m_fDistToVP = m_fHghtAdj / tan( HEIGHT_THETA );	
}

void Camera::renderScene()
{
	if ( m_bUpdated )
	{
		// iterate through pixels, storing each value as a point and color.
		for ( float y = -m_fHghtAdj; y <= 0.f; y += 1.f )
			for ( float x = -m_fWdthAdj; x <= 0.f; x += 1.f )
			{
				evaluatePixel( x, y );		// Lower Left Quadrant
				evaluatePixel( -x, y );		// Lower Right Quadrant
				evaluatePixel( x, -y );		// Upper Left Quadrant
				evaluatePixel( -x, -y );	// Upper Right Quadrant
			}

		m_bUpdated = false;
	}

	// Render Image.
	m_pImgBuffr.Render();
}

// Given an X and Y coordinate for a ViewPort:
//		Generate a ray through that coordinate,
//		Evaluate the color for that ray
//		Set that color to Image Buffer 
void Camera::evaluatePixel( float fX, float fY )
{
	Vector3D pRay( fX, fY, -m_fDistToVP );	// Direction Ray
	Vector3D pColorEval;					// Return Value
	bool bIntersected;
	pRay.normalize();

	// Evaluate Pixel of this Ray.
	bIntersected = m_pEnvMngr->getColor( &m_pPosition, &pRay, &pColorEval );	// Evaluate Color

	if ( bIntersected )
	{
		// Shift ViewPort Coords to Image Coords and set Pixel Color.
		m_pImgBuffr.SetPixel( ((int)(fX - 0.5f) + m_iHlfWdth),
							  ((int)(fY - 0.5f) + m_iHlfHght),
							  //glm::vec3( (float)((fX - .5f + m_iHlfWdth) / m_iWidth), (float)((fY - .5f + m_iHlfHght) / m_iHeight), m_fDistToVP ) );  // Pretty Image
							  glm::vec3( pColorEval.getElement( C_X ),
										 pColorEval.getElement( C_Y ),
										 pColorEval.getElement( C_Z ) ) );
	}
}

// Saves current scene as an image.
void Camera::snapShotScene()
{
	m_pImgBuffr.SaveToFile( "Part1Test.png" );
}

// Handle logic for changing window size.
void Camera::updateHxW( int iHeight, int iWidth ) 
{ 
	m_iHeight = iHeight; 
	m_iWidth = iWidth; 
	m_bUpdated = true; 
	m_pImgBuffr.Initialize(); 
	calcVPDistance();
}
