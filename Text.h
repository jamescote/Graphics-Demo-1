#pragma once

#include "Tess_Graphic.h"
#include "GlyphExtractor.h"
#include "stdafx.h"

// Class for outputting Text
class Text :
	public Tess_Graphic
{
public:
	Text(int iHeight, int iWidth);
	~Text();

	// Overloaded virtual functions
	virtual void renderGraphic(const MyGeometry* pGeometry);

	// Getters and Setters for private Variables
	virtual void setOutput(wstring sNewOutput);

	// Modification Function - Switch Fonts
	virtual void switchFonts();

private:
	// Private Text Values
	float m_fBottom;
	
	// Private Functions
	virtual void updatePxlSpcMod() { m_fPixelSpaceMod = 2.f / m_fTextLength; }
	void drawGlyph(const MyGeometry* pGeometry, MyGlyph pGlyph, float fLHS);
protected:
	// Text Values
	float m_fLHS;
	GlyphExtractor m_pGlyphXtractor;	// For Parsing and constructing Font Splines/Glyphs
	vector<MyGlyph> m_pOutputGlyphs;	// Calculated when necessary.

	// Text Variables
	int m_iCurrFont;	
	wstring m_sCurrentOutput;
	float m_fTextLength;
	float m_fPixelSpaceMod;	
};

