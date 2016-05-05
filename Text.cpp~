#include "Text.h"
#include "ShaderManager.h"

#define CONTROL_POINT_SIZE 3.0f
#define MAX_NUM_FONTS 3
#define DEFAULT_SCROLL_SPEED .5f

const string sFontLocations[MAX_NUM_FONTS] = {
	"Fonts/lora/Lora-Regular.ttf",
	"Fonts/source-sans-pro/SourceSansPro-Regular.otf",
	"Fonts/Arizonia-Regular.ttf" };

// Constructor, set the Parent Constructor
Text::Text(int iHeight, int iWidth) : Tess_Graphic(iHeight, iWidth)
{
	m_iCurrFont = 0;
	m_pGlyphXtractor.LoadFontFile(sFontLocations[m_iCurrFont]);
	m_fBottom = 0.f;
	m_fLHS = -1.f;
}

// Destructor
Text::~Text()
{
	// Nothing to Destruct
}

// Overloaded virtual functions
void Text::renderGraphic(const MyGeometry* pGeometry)
{
	// Start at Current Left Side of Screen.
	float fCurrLHS = m_fLHS;

	// If updated, draw Text
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (vector<MyGlyph>::iterator glyphItr = m_pOutputGlyphs.begin();
		glyphItr != m_pOutputGlyphs.end();
		++glyphItr)
	{
		drawGlyph(pGeometry, *(glyphItr), fCurrLHS);
		fCurrLHS += glyphItr->advance * m_fPixelSpaceMod;
	}

}

/*******************************************************\
 * Text Functionality                                  *
\*******************************************************/

// Draw a given Glyph, MyGeometry necessary.
void Text::drawGlyph(const MyGeometry* pGeometry, MyGlyph pGlyph, float fLHS)
{
	// Locals - Store current value of m_vVertices and m_vColors
	vector<MyContour>::iterator contourItr = pGlyph.contours.begin();
	vector<MySegment>::iterator segmentItr;

	for (contourItr; contourItr != pGlyph.contours.end(); ++contourItr)
	{
		for (segmentItr = contourItr->begin(); segmentItr != contourItr->end(); ++segmentItr)
		{
			// Store Copies
			m_vColors.clear();
			m_vVertices.clear();

			// Populate our points.
			for (unsigned int i = 0; i <= segmentItr->degree; ++i)
			{
				addPosVertex(fLHS + (segmentItr->x[i] * m_fPixelSpaceMod),
					m_fBottom + (segmentItr->y[i] * m_fPixelSpaceMod));
				addColorVertex(m_fColors[0][RED], m_fColors[0][GREEN], m_fColors[0][BLUE]);
			}

			// Draw
			if (DRAW_CURVES <= m_iLevels)
			{
				switch (segmentItr->degree)
				{
				case DEGREE_POINT:
					drawPoints(pGeometry);
					CheckGLErrors();
					break;
				case DEGREE_LINE:
					drawLine(pGeometry, GL_LINE_STRIP);
					CheckGLErrors();
					break;
				case DEGREE_QUAD:
					drawQuad(pGeometry);
					if (DRAW_TANGENTS <= m_iLevels)
						drawTangents(pGeometry, true);
					if (DRAW_CNTRL_POINTS <= m_iLevels)
						drawControlPoints(pGeometry, CONTROL_POINT_SIZE, true);
					CheckGLErrors();
					break;
				case DEGREE_CUBIC:
					drawCubic(pGeometry);
					if (DRAW_TANGENTS <= m_iLevels)
						drawTangents(pGeometry, false);
					if (DRAW_CNTRL_POINTS <= m_iLevels)
						drawControlPoints(pGeometry, CONTROL_POINT_SIZE, false);
					CheckGLErrors();
					break;
				default:
					cout << "Error: Incorrect Degree pulled from GlyphExtractor." << endl;
					break;
				}
			}
		}
	}
}

// Set a new vector of Glyphs based on Input String
void Text::setOutput(wstring sNewOutput)
{
	// Locals
	MyGlyph pNextGlyph;
	m_fTextLength = 0;
	m_sCurrentOutput = sNewOutput;

	// Fill our Working set with Glyphs
	m_pOutputGlyphs.clear();
	for (unsigned int i = 0; i < sNewOutput.length(); ++i)
	{
		pNextGlyph = m_pGlyphXtractor.ExtractGlyph(sNewOutput[i]);
		m_fTextLength += pNextGlyph.advance;
		m_pOutputGlyphs.push_back(pNextGlyph);
	}

	updatePxlSpcMod();
}

// Loads the new font type and rereads in the Set of Glyphs for the output
void Text::switchFonts()
{
	// Update new Font Type
	m_iCurrFont += 1;
	m_iCurrFont %= MAX_NUM_FONTS;

	// Load New Font
	m_pGlyphXtractor.LoadFontFile(sFontLocations[m_iCurrFont]);

	// Populate Glyph List
	setOutput(m_sCurrentOutput);
}
