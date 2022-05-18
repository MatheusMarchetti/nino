#pragma once

#include "nino.h"

#include "CustomEvents.h"

class ApplicationLayer : public nino::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(nino::Timestep ts) override;
	virtual void OnEvent(nino::Event& event) override;

private:
	bool OnPlayerMoved(PlayerMovedEvent& event);

private:
	float PlayerX = 40.0f;
	float PlayerY = 25.0f;
};

