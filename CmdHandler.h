#pragma once

/* INCLUDES */
#include "stdafx.h"
#include "InputHandler.h"
#include "GraphicsHeader.h"
#include "GraphicsManager.h"

/* DEFINES */
#define CMD_SIZE	10
#define NUM_BASE	10

/* CLASS */
// Command Handler Class
// Inherits from the InputHandler to process specific input of commands necessary
// for this particular program.
class CmdHandler :
	public InputHandler
{
public:
	static CmdHandler* getInstance(GLFWwindow *rWindow);
	static CmdHandler* getInstance(const char* c_Input, int iInputSize, GLFWwindow *rWindow);
	~CmdHandler();

	void process_Input(const char* c_FirstWord);

private:
	// Singleton Variables
	CmdHandler(GLFWwindow *rWindow);
	CmdHandler(const char* c_Input, int iInputSize, GLFWwindow *rWindow) : InputHandler(c_Input, iInputSize) { m_pGPXMngr = GraphicsManager::getInstance(rWindow); }
	static CmdHandler* m_pInstance;
	GraphicsManager *m_pGPXMngr;
	
	// Assignment 1 functions
	int		exec_Draw();
	void	exec_Type();
	void	exec_Levels();
	void	exec_Colors();

	// Assignment 2 functions
	void	exec_Load();
	void	exec_Delete();

	// Enum for Character Commands
	enum eCommands
	{
		TYPE = 0,
		COLOR,
		LEVEL,
		DRAW,
		LOAD,
		eDELETE,
		NUM_CMDS
	};

	// All available commands
	static const char cCommands[NUM_CMDS][CMD_SIZE];
};

