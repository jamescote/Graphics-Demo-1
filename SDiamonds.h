#pragma once
#include "Graphic.h"

class SDiamonds :
	public Graphic
{
public:
	SDiamonds(int iHeight, int iWidth);
	~SDiamonds();

	void genVertices(const MyGeometry* pGeometry);
	eGraphicsEnum getType() { return SQUARE_DIAMOND; }
	void draw();

private:
	void genSquare(const Vector2D *pStart, float fLngth, float fCIntensity);
	void genDiamond(const Vector2D *pStart, float fLngth, float fCIntensity);
};

