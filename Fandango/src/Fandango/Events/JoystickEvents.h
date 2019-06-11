#pragma once

#include "Event.h"

namespace Fandango
{
	struct JoystickData
	{
		int m_X, m_Y;

		JoystickData(int x, int y)
			: m_X(x), m_Y(y) {}
	};

	class FNDG_API JoystickEvent : public Event
	{
	public:
		JoystickEvent(unsigned int joystickIndex, int x, int y)
			: m_JoystickIndex(joystickIndex), m_Data(x, y) {}

		inline int GetJoystickIndex() const { return m_JoystickIndex; }
		EVENT_CLASS_CATEGORY(EventCategoryJoystick | EventCategoryInput)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "JoystickTiltedEvent: index " << m_JoystickIndex << "(" << m_Data.m_X << ", " << m_Data.m_Y << ")";
			return ss.str();
		}
	private:
		JoystickData m_Data;
		unsigned int m_JoystickIndex;
	};
}