#include "Mouse_Handler.h"

#define UPDATE_THRESHOLD .01f

// Static Singleton instance
Mouse_Handler* Mouse_Handler::m_pInstance = NULL;

// Constructor
Mouse_Handler::Mouse_Handler(GLFWwindow *rWindow)
{
	m_pGpxMngr = GraphicsManager::getInstance(rWindow);
	m_bRotateFlag = m_bTranslateFlag = false;
}

// Fetch the Singleton instance
Mouse_Handler* Mouse_Handler::getInstance(GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new Mouse_Handler(rWindow);

	return m_pInstance;
}

// Destructor
Mouse_Handler::~Mouse_Handler()
{
	m_pGpxMngr = NULL;
}

// Updates the mouse posisions internally and applies a transform if necessary
void Mouse_Handler::updateMouse(float fX, float fY)
{
	if (m_bTranslateFlag)
		m_pGpxMngr->translateImage(fX, fY, &m_pInitialPos);
	else if (m_bRotateFlag)
		m_pGpxMngr->rotateImage(fX, fY, &m_pInitialPos);

	m_pInitialPos.setX(fX);
	m_pInitialPos.setY(fY);
}
