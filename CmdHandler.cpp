#include "CmdHandler.h"

/* Initialize Statics */
const char CmdHandler::cCommands[NUM_CMDS][CMD_SIZE] = { "type",
														 "color",
														 "level",
														 "draw",
														 "load",
														 "delete" };
CmdHandler* CmdHandler::m_pInstance = NULL;

CmdHandler::CmdHandler(GLFWwindow *rWindow) : InputHandler()
{
	// Initializing Base Class
	m_pGPXMngr = GraphicsManager::getInstance(rWindow);
}

// Get a copy of CmdHandler that doesn't have any initialized
//   Input associated with it.
CmdHandler* CmdHandler::getInstance(GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new CmdHandler(rWindow);

	return m_pInstance;
}

// Get a copy of CmdHandler with an associated Input.
CmdHandler* CmdHandler::getInstance(const char* c_Input, int iInputSize, GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new CmdHandler(c_Input, iInputSize, rWindow);
	else
		m_pInstance->load_Input(c_Input, iInputSize);

	return m_pInstance;
}

// Destructor
CmdHandler::~CmdHandler()
{
	delete m_pInstance;
}

// Process initial input, acts as though key word is first word of input.
// First word can be ignored and function will grab the first word.
void CmdHandler::process_Input( const char* c_FirstWord)
{
	// Parse initial command
	if (!strcmp(c_FirstWord, cCommands[DRAW]))
	{
		if (ERR_CODE == exec_Draw())
			cout << "Error occurred when processing draw input." << endl;
	}
	else if (!strcmp(c_FirstWord, cCommands[TYPE]))
		exec_Type();
	else if (!strcmp(c_FirstWord, cCommands[LEVEL]))
		exec_Levels();
	else if (!strcmp(c_FirstWord, cCommands[COLOR]))
		exec_Colors();
	else if (!strcmp(c_FirstWord, cCommands[LOAD]))
		exec_Load();
	else if (!strcmp(c_FirstWord, cCommands[eDELETE]))
		exec_Delete();
	else
		cout << "Unknown Command: \"" << c_FirstWord << ".\"" << endl;
}

/*****************************************************************************\
 * Assignment 2 functions													 *
\*****************************************************************************/

// Attempts to load in an image from a given image name
void CmdHandler::exec_Load()
{
	char c_ImgName[MAX_INPUT_SIZE] = {};
	int iErr = get_Next_Word(c_ImgName, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		do
		{
			m_pGPXMngr->loadImage(c_ImgName);
			iErr = get_Next_Word(c_ImgName, MAX_INPUT_SIZE);

		} while (!p_EOB() && (ERR_CODE != iErr));
	}

}

// Delete the current Image.
void CmdHandler::exec_Delete()
{
	m_pGPXMngr->deleteImage();
}

/*****************************************************************************\
* Assignment 1 functions													 *
\*****************************************************************************/
// Handle Execution of the Draw Command
int CmdHandler::exec_Draw()
{
	// Locals
	char c_NextWord[MAX_INPUT_SIZE] = {};
	int iErr = 0;

	// Handle rest of the input.
	do
	{
		iErr = get_Next_Word(c_NextWord, MAX_INPUT_SIZE);

		if (ERR_CODE != iErr)
		{
			if (!strcmp(c_NextWord, cCommands[TYPE]))
				exec_Type();
			else if (!strcmp(c_NextWord, cCommands[LEVEL]))
				exec_Levels();
			else if (!strcmp(c_NextWord, cCommands[COLOR]))
				exec_Colors();
			else
				cout << "Unknown Command: \"" << c_NextWord << ".\"" << endl;
		}
	} while (!p_EOB() && (ERR_CODE != iErr));

	return iErr;
}

// Handle Execution of the Type Command
void CmdHandler::exec_Type()
{
	// Locals
	char c_NextWord[MAX_INPUT_SIZE] = {};
	int iErr = get_Next_Word(c_NextWord, MAX_INPUT_SIZE);

	// Parse Type
	if (ERR_CODE != iErr)
	{
		if (!strcmp(cTypes[SQUARE_DIAMOND], c_NextWord))
			m_pGPXMngr->setType(SQUARE_DIAMOND);
		else if (!strcmp(cTypes[SPIRAL], c_NextWord))
			m_pGPXMngr->setType(SPIRAL);
		else if (!strcmp(cTypes[TRI_FRACTAL], c_NextWord))
			m_pGPXMngr->setType(TRI_FRACTAL);
		else if (!strcmp(cTypes[SQ_FRACTAL], c_NextWord))
			m_pGPXMngr->setType(SQ_FRACTAL);
		else
			cout << "Unknown Type : \"" << c_NextWord << ".\"" << endl;
	}
}

// Handle Execution of the Level Command
void CmdHandler::exec_Levels()
{
	// Locals
	char c_Num[MAX_INPUT_SIZE] = {};
	char * p_End;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);
	long lNewLevel = 0l;
	
	// get New Level
	if (ERR_CODE != iErr)
		lNewLevel = strtol(c_Num, &p_End, NUM_BASE);

	// Verify Level
	if (lNewLevel > INT_MAX)
		cout << "Error : " << lNewLevel << " levels is too many.  Number of Levels unchanged at " <<
		m_pGPXMngr->getLevel() << "." << endl;
	else if (0 == lNewLevel)
		cout << "Error : Invalid input: \"" << c_Num << ".\"  Please pick a number > 0." << endl;
	else
		m_pGPXMngr->setLevel((int)lNewLevel);
}

// Handle Execution of the Color Command
void CmdHandler::exec_Colors()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);
	char * p_End;
	int iColorIndex = 0;
	float fColorRGB[RGB_MAX] = { 1.0f, 1.0f, 1.0f };
	float fTempVal = 0.f;

	if (ERR_CODE != iErr) // No initial Error?  Begin parse.
	{
		// Verify Index - Convert to 0-based
		iColorIndex = (int)strtol(c_Num, &p_End, 10) - 1;
		if (iColorIndex >= 0 && iColorIndex < NUM_COLORS)
		{
			// Set to defaults
			m_pGPXMngr->getColor(iColorIndex, &fColorRGB[RED], &fColorRGB[GREEN], &fColorRGB[BLUE]);
			
			for (int i = 0; i < RGB_MAX; i++)
			{
				iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);
				if (ERR_CODE != iErr) // Ensure no error occurred.
				{
					// Verify Value
					fTempVal = strtof(c_Num, &p_End);
					if (fTempVal < 0.f || fTempVal > 1.f)
						cout << "Error : Invalid range for RGB Value: " << c_Num <<
							".  Please choose value between 0.0 and 1.0." << endl;
					else // Save Value
						fColorRGB[i] = fTempVal;
				}
			}
			if (ERR_CODE == iErr)
				cout << "Error Reading in RGB Values. \"" << c_Num << "\"" << endl;
			else
				m_pGPXMngr->setColor(iColorIndex, fColorRGB[RED], fColorRGB[GREEN], fColorRGB[BLUE]);
		}
	}
	else // Initial Read
		cout << "Error Reading in Color Index. \"" << c_Num << "\"" << endl;
}
