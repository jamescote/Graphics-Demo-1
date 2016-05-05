#pragma once

#include "stdafx.h"
#include "GraphicsHeader.h"
#include "AssignmentHeader.h"

// Class: GeometryManager
// Purpose: Manages a geometry structure for each Assignment.  Ensures proper setup of
//			each VAO for requirements of the Assignment.  
// TODO: Store textures in GeometryManager and be able to hand out binding numbers to associate
//			Textures to desired classes.
// Written By: James Coté
class GeometryManager
{
public:
	static GeometryManager* getInstance();
	~GeometryManager();

	bool initializeGeometry();
	void bindTextureData(eAssignmentState eAssg, GLsizeiptr iPtr, void* data);

	// To get a Geometry Pointer, unable to be modified
	MyGeometry const* getGeometry(eAssignmentState eAssg) { return &m_pGeometry[eAssg]; }
private:
	// Singleton Implementation
	GeometryManager();
	static GeometryManager* m_pInstance;

	bool m_bInitialized;
	MyGeometry m_pGeometry[MAX_ASSG];

	// Seperate Initializers for each Assignment
	bool initializeAssg1();
	bool initializeAssg2();
	bool initializeAssg3();
	
};

