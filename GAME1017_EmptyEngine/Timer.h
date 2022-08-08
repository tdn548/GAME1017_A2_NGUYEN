#pragma once
#include <ctime>
#include <string>
class Timer
{
private:
	bool m_started;
	bool m_running;
	bool m_hasChanged;

	int m_elapsed;
	int m_elapsedTotal;

	time_t m_startTime;
	time_t m_lastTime;
	time_t m_currentTime;

public:
	Timer();
	void Start();
	void Update();
	void Resume();
	void Pause();

	std::string GetTime();
	int GetLastTime() { return m_elapsedTotal + m_elapsed; };
	bool HasChanged();
};

