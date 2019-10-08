#pragma once

#include "Event.h"

namespace Fandango
{
	// Blueprint for all key-events. No one is supposed to instanciate one
	class FNDG_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(unsigned int keycode)
			:m_KeyCode(keycode) {}
		unsigned int m_KeyCode;
	};

	class FNDG_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned int keycode, unsigned int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		unsigned int m_RepeatCount;
	};

	class FNDG_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};

	class FNDG_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
	};
}