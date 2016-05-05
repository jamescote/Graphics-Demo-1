// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// uniform = constant variable over everytime shader runs.
uniform sampler2DRect image;

// Grey Scale Variables
const int NUM_GREYSCALE = 3;
const vec4 iGreyScaleSetting[NUM_GREYSCALE] = vec4[]( vec4( 0.333, 0.333, 0.333, 1.0 ),
    	   		           vec4( 0.299, 0.587, 0.114, 1.0 ),
  				   vec4( 0.213, 0.715, 0.072, 1.0 ) );
uniform int iCurrGreyScale = -1;

// Negative Image Variables
uniform bool bNegativeSetting = false;

// Sobel kernels - Assg2 Part 3
const int V_SOBEL = 0;
const int H_SOBEL = 1;
const int U_MASK = 2;
uniform int iSobelIndex = 0;
const vec3 mSobel[9] = vec3[]( vec3( 1.0, 2.0, 1.0 ),		// Vertical Sobel Columns
						 vec3( 0.0, 0.0, 0.0 ),			
						 vec3( -1.0, -2.0, -1.0 ), 	
					     vec3( 1.0, 0.0, -1.0 ),	// Horizontal Sobel Columns
						 vec3( 2.0, 0.0, -2.0 ),		
						 vec3( 1.0, 0.0, -1.0 ),
					     vec3( 0.0, -1.0, 0.0 ),	// UMask Columns	
						 vec3( -1.0, 5.0, -1.0 ),		
						 vec3( 0.0, -1.0, 0.0 ) );	
uniform int iSobelVal = 0;	

// Gaussian Blur
const float E = 2.718;
const float GAUSS_MULT = .35;
const float PI = 3.14159265;
uniform int iBlurLevel = 1;
uniform bool bApplyBlur = false;
									  

// interpolated colour received from vertex stage
in vec2 fragTexture;

// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

void applyGaussianBlur()
{	
	float iDistance = iBlurLevel / 2.0;
	float fSigma = 0.36 + (iDistance * GAUSS_MULT);
	fSigma *= fSigma;
	float fSigmaDenom = 2.0 * fSigma;
	float fSgmaCalc = 1.0/(PI * fSigmaDenom);
	vec4 vCurrColor = FragmentColour;
	float fG;
	vec4 vLHS;
	vec4 vRHS;
	
	// Apply gaus to current color
	vCurrColor *= fSgmaCalc;
	
	for(float x = -iDistance; x <= iDistance; x += 1.0)
		for(float y = -iDistance; y <= iDistance; y += 1.0)
		{
			if( y != 0 || x != 0)
			{
				fG = fSgmaCalc*pow(E, -(((x*x) + (y*y))/fSigmaDenom));
				vCurrColor += texture(image, vec2(fragTexture.x + x, fragTexture.y + y)) * fG;
			}
		}
	
	FragmentColour = vCurrColor;
}

// Apply a given 3x3 Kernel to the current fragment.
void applyConvolution( mat3 mKernel )
{
	vec4 newColor;
	vec4 texel;
	for( int x = -1; x < 2; x++ )
		for( int y = -1; y < 2; y++)
		{
			texel = texture(image, vec2(fragTexture.x+x, fragTexture.y+y));
			newColor.r += (texel.r * mKernel[x+1][y+1]);
			newColor.g += (texel.g * mKernel[x+1][y+1]);
			newColor.b += (texel.b * mKernel[x+1][y+1]);
		}
	
	newColor = abs(newColor);
	FragmentColour *= newColor;
}

void main(void)
{
	float fGreyVal = 0.0;
	vec4 texelColor = texture(image, fragTexture);
	FragmentColour = texelColor;
	mat3 mSobelMatrix;
	int iSobelOffset = iSobelIndex * 3;

	// Apply Gaussian Blur
	if( bApplyBlur )
		applyGaussianBlur();
		
	// Apply Negative
	if( bNegativeSetting)
	{
		FragmentColour = vec4( (1.0 - FragmentColour.r), 
							   (1.0 - FragmentColour.g),
							   (1.0 - FragmentColour.b),
							   1.0 );
	}
	
	if( iSobelVal > 0 )
		mSobelMatrix = mat3( mSobel[iSobelOffset], mSobel[iSobelOffset+1], mSobel[iSobelOffset+2]);
	for( int i = 0; i < iSobelVal; ++i)
	{
		applyConvolution( mSobelMatrix );
	}
		
	// Apply Grey Scale
	if( iCurrGreyScale >= 0 && iCurrGreyScale < NUM_GREYSCALE )
	{
		texelColor = FragmentColour;
		
		fGreyVal = (iGreyScaleSetting[iCurrGreyScale].r * texelColor.r) +
				   (iGreyScaleSetting[iCurrGreyScale].g * texelColor.g) +
				   (iGreyScaleSetting[iCurrGreyScale].b * texelColor.b);
		
		FragmentColour = vec4( fGreyVal,
							   fGreyVal,
							   fGreyVal,
							   1.0 );
	}
	
	
}
