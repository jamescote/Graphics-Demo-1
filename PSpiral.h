#pragma once
#include "Graphic.h"

#define PI 3.14159265
#define LIMIT_MULTIPLIER 2.f*PI

class PSpiral :
	public Graphic
{
public:
	PSpiral(int iHeight, int iWidth);
	~PSpiral();

	// Overloaded Pure Virtual Functions
	void genVertices(const MyGeometry* pGeometry);
	eGraphicsEnum getType() { return SPIRAL; };
	void draw();

	// Overloaded Virtual Functions
	void setLevel(int iNewLevel);
	void setHeight(int iHeight);

private:
	float m_fSampleRate;
	float m_fLimit;
	float m_fRatio;
};

