#pragma once

#include "Core.h"

namespace nino
{
	class CORE_API Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time)
		{
			ThrowOnError(QueryPerformanceFrequency((LARGE_INTEGER*)&m_FrequencyCounter));
			ThrowOnError(QueryPerformanceCounter((LARGE_INTEGER*)&m_LastTimeCounter));

			m_MaxDelta = m_FrequencyCounter / 10;
		}

		void Tick()
		{
			uint64_t currentTime;

			ThrowOnError(QueryPerformanceCounter((LARGE_INTEGER*)&currentTime));

			m_Time = currentTime - m_LastTimeCounter;
			m_LastTimeCounter = currentTime;

			if (m_Time > m_MaxDelta)
				m_Time = m_MaxDelta;

			m_Time /= m_FrequencyCounter;
		}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		uint64_t m_FrequencyCounter;
		uint64_t m_LastTimeCounter;
		uint64_t m_MaxDelta;

		float m_Time;
	};
}