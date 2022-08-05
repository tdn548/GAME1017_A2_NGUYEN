#include "Timer.h"

Timer::Timer():m_started(false), m_running(false), m_hasChanged(false), m_elapsed(0), m_elapsedTotal(0), m_startTime(0), m_lastTime(0), m_currentTime(0)
{
	
}

void Timer::Start()
{
	if (!m_started)
	{
		m_started = true;
		m_running = true;
		m_startTime = time(nullptr);
		m_currentTime = m_lastTime = m_startTime;
	}
	else
	{
		Resume();
	}
}

void Timer::Update()
{
	if (m_running)
	{
		m_lastTime = m_currentTime;
		m_currentTime = time(nullptr);
		if (m_lastTime != m_currentTime)
		{
			m_hasChanged = true;
		}
		else
		{
			m_hasChanged = false;
		}
		m_elapsed = static_cast<int>(m_currentTime) - static_cast<int>(m_startTime);
	}
}

void Timer::Resume()
{
	if (m_started && !m_running)
	{
		m_startTime = time(nullptr);
		m_running = true;
	}
}

void Timer::Pause()
{
	if (m_running)
	{
		m_running = false;
		m_hasChanged = false;
		m_elapsedTotal += m_elapsed;
		m_elapsed = 0;
	}
}

std::string Timer::GetTime()
{
	return std::to_string(m_elapsedTotal + m_elapsed);
}

bool Timer::HasChanged()
{
	return m_hasChanged;
}
