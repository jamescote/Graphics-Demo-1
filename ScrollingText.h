#pragma once

#include "Text.h"

class ScrollingText : public Text
{
public:
	ScrollingText(int iHeight, int iWidth);
	~ScrollingText();

	// Getters & Setters
	bool getScroll() { return m_bScroll; }
	void setScroll(bool bScroll) { m_bScroll = bScroll; }
	void adjustScrollSpeed(float fOffset) { m_fScrollSpeed += fOffset; }

	// Overloaded Virtual Functions
	void renderGraphic(const MyGeometry* pGeometry);
	void switchFonts();
	void scrollText();

private:
	// Private Scroll Variables
	bool m_bScroll;
	float m_fScrollSpeed;					// Scroll Speed is x points of screen space per frame.
	clock_t m_plastTick;					// Stores timestamp at last tick.	
	float m_fElapsedDistance;

	void updatePxlSpcMod() { m_fPixelSpaceMod = 2.f / (m_fTextLength * .25f); }
};

