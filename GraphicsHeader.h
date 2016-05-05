#pragma once

/* Graphic Type Includes */
#include "stdafx.h"
#include "ImageReader.h"

/* DEFINES */
#define TYPE_NAME_LENGTH	64
#define NUM_COLORS			3
#define MAX_BUFFER_SIZE		100000
#define X_COORD				0
#define Y_COORD				0
#define SCALE_MOD			0.009f

// Enum for Graphics Types - Assignment 1
enum eGraphicsEnum
{
	SQUARE_DIAMOND = 0,
	SPIRAL,
	TRI_FRACTAL,
	SQ_FRACTAL,
	GPX_SIZE,
	UNDEFINED
};

enum eRGBEnum
{
	RED = 0,
	GREEN,
	BLUE,
	RGB_MAX
};

// Assignment 3 - Scene Types
enum eCurveScenes
{
	P3_CURVE = 0,
	P4_CURVE,
	NAME_SCENE,
	SCROLL_SCENE,
	A3_SIZE
};

// Commands to read from User
const char cTypes[GPX_SIZE][TYPE_NAME_LENGTH] =
{
	"SDiamonds",
	"PSpiral",
	"STriangle",
	"MSponge"
};

// Geometry Structure
struct MyGeometry
{
	// OpenGL names for array buffer objects, vertex array object
	GLuint  vertexBuffer;
	GLuint  colourBuffer;
	GLuint	textureBuffer;
	GLuint  vertexArray;


	// initialize object names to zero (OpenGL reserved value)
	MyGeometry() : vertexBuffer( 0 ), colourBuffer( 0 ), vertexArray( 0 )
	{
	}
};
