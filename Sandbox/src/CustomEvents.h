#pragma once

class PlayerMovedEvent : public nino::Event
{
public:
	PlayerMovedEvent(float X, float Y) : posX(X), posY(Y), m_EventID(EVENT_ID(PlayerMovedEvent)) {}

	float GetPosX() const { return posX; }
	float GetPosY() const { return posY; }

	virtual size_t GetEventID() const override { return m_EventID; }
	virtual std::wstring ToString() const override
	{
		std::wstringstream wss;
		wss << "Player moved event";

		return wss.str();
	}

private:
	size_t m_EventID;
	float posX;
	float posY;
};