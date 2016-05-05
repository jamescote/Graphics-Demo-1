// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream> 
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <limits.h>
#include <ctime>
#include "EnvSpec.h"

#ifdef USING_LINUX
#include <string.h>
#define GLFW_INCLUDE_GLCOREARB
#define GL_GLEXT_PROTOTYPES
#endif
#ifdef USING_WINDOWS
#include <string>
#include <GLAD/glad.h>
#endif
#include <GLFW/glfw3.h>

#define ERR_CODE -1

/* NAMESPACES */
using namespace std;

// From Boilerplate code,
// Shouldn't need to modify this.
// Accessible anywhere stdafx.h is included and GL Error Checking is required.
inline bool CheckGLErrors()
{
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			cout << "GL_INVALID_ENUM" << endl; break;
		case GL_INVALID_VALUE:
			cout << "GL_INVALID_VALUE" << endl; break;
		case GL_INVALID_OPERATION:
			cout << "GL_INVALID_OPERATION" << endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
		case GL_OUT_OF_MEMORY:
			cout << "GL_OUT_OF_MEMORY" << endl; break;
		default:
			cout << "[unknown error code]" << endl;
		}
		error = true;
	}
	return error;
}

/**
inline void CheckForShaderIssues(GLuint iProgram)
{
	GLint iErr;

	glGetProgramiv(iProgram, GL_DELETE_STATUS, &iErr);
	cout << "DELETE_STATUS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_LINK_STATUS, &iErr);
	cout << "LINK_STATUS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_VALIDATE_STATUS, &iErr);
	cout << "VALIDATE_STATUS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_INFO_LOG_LENGTH, &iErr);
	cout << "INFO_LOG_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ATTACHED_SHADERS, &iErr);
	cout << "ATTACHED_SHADERS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_ATOMIC_COUNTER_BUFFERS, &iErr);
	cout << "ACTIVE_ATOMIC_COUNTER_BUFFERS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_ATTRIBUTES, &iErr);
	cout << "ACTIVE_ATTRIBUTES: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &iErr);
	cout << "ACTIVE_ATTRIBUTE_MAX_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_UNIFORMS, &iErr);
	cout << "ACTIVE_UNIFORMS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_UNIFORM_BLOCKS, &iErr);
	cout << "ACTIVE_UNIFORM_BLOCKS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &iErr);
	cout << "ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &iErr);
	cout << "ACTIVE_UNIFORM_MAX_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_COMPUTE_WORK_GROUP_SIZE, &iErr);
	cout << "COMPUTE_WORK_GROUP_SIZE: " << iErr << endl;
	glGetProgramiv(iProgram, GL_PROGRAM_BINARY_LENGTH, &iErr);
	cout << "PROGRAM_BINARY_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_TRANSFORM_FEEDBACK_BUFFER_MODE, &iErr);
	cout << "TRANSFORM_FEEDBACK_BUFFER_MODE: " << iErr << endl;
	glGetProgramiv(iProgram, GL_TRANSFORM_FEEDBACK_VARYINGS, &iErr);
	cout << "TRANSFORM_FEEDBACK_VARYINGS: " << iErr << endl;
	glGetProgramiv(iProgram, GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, &iErr);
	cout << "TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH: " << iErr << endl;
	glGetProgramiv(iProgram, GL_GEOMETRY_VERTICES_OUT, &iErr);
	cout << "GEOMETRY_VERTICES_OUT: " << iErr << endl;
	glGetProgramiv(iProgram, GL_GEOMETRY_INPUT_TYPE, &iErr);
	cout << "GEOMETRY_INPUT_TYPE: " << iErr << endl;
	glGetProgramiv(iProgram, GL_GEOMETRY_OUTPUT_TYPE, &iErr);
	cout << "GEOMETRY_OUTPUT_TYPE: " << iErr << endl;
}//*/

