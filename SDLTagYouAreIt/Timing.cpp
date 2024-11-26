#include "Timing.h"

Timing::Timing()
{
	m_currentTime = SDL_GetTicks();
	m_lastTime = m_currentTime;
	m_fpsStart = m_currentTime;
	m_fpsCount = 0;
	m_fpsLast = 0;
	m_deltaTime = 0;
	m_totalGameTime = 0.0f; // Initialize total game time
}

Timing::~Timing()
{
}

void Timing::Tick()
{
	m_currentTime = SDL_GetTicks();
	m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;

	// Update total game time
	m_totalGameTime += m_deltaTime;

	if (m_fpsStart + 1000 <= m_currentTime)
	{
		m_fpsLast = m_fpsCount;
		m_fpsCount = 0;
		m_fpsStart = m_currentTime;
	}
	else
	{
		m_fpsCount++;
	}
	m_lastTime = m_currentTime;
}