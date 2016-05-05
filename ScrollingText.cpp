#include "ScrollingText.h"

#define MAX_NUM_SCROLLING_FONTS 4
#define DEFAULT_SCROLL_SPEED 1.f
#define STARTING_POINT 1.f

const string c_sScrollingFonts[MAX_NUM_SCROLLING_FONTS] = {
	"Fonts/alex-brush/AlexBrush-Regular.ttf",
	"Fonts/Inconsolata.otf",
	"Fonts/Learning-Curve-Pro/LearningCurve_OT.otf",
	"Fonts/Learning-Curve-Pro/LearningCurveDashed_OT.otf" };

// Constructor
ScrollingText::ScrollingText(int iHeight, int iWidth) : Text( iHeight, iWidth)
{
	m_bScroll = true;
	m_fScrollSpeed = DEFAULT_SCROLL_SPEED;
	m_fElapsedDistance = 0.f;
	m_plastTick = clock();
	m_fLHS = STARTING_POINT;
}

// Destructor
ScrollingText::~ScrollingText()
{
	// Nothing to Destruct
}

// Overloaded Function from Parent
void ScrollingText::renderGraphic(const MyGeometry* pGeometry)
{
	scrollText();
	Text::renderGraphic(pGeometry);
}

// Scrolls the text x-distance based on the last time this function was called.
void ScrollingText::scrollText()
{
	// Get the Current Clock Count
	clock_t timeNow = clock();

	if (m_bScroll)
	{	
		float fTextLngthAdjusted = m_fTextLength * m_fPixelSpaceMod;
		float fElapsed_seconds = float(timeNow - m_plastTick) / CLOCKS_PER_SEC;
		float percentOfSecond = fElapsed_seconds / 60.f;
		m_fLHS -= m_fScrollSpeed * percentOfSecond;
		if (m_fLHS + fTextLngthAdjusted <= -1.f )
			m_fLHS = STARTING_POINT;
		else if (m_fLHS > 1.f) // Outside ViewSpace
			m_fLHS = -STARTING_POINT - fTextLngthAdjusted;
	}	

	// Set the time currently
	m_plastTick = timeNow;
}

// Loads the new font type and rereads in the Set of Glyphs for the output
void ScrollingText::switchFonts()
{
	// Update new Font Type
	m_iCurrFont += 1;
	m_iCurrFont %= MAX_NUM_SCROLLING_FONTS;

	// Load New Font
	m_pGlyphXtractor.LoadFontFile(c_sScrollingFonts[m_iCurrFont]);

	// Populate Glyph List
	setOutput(m_sCurrentOutput);
}
