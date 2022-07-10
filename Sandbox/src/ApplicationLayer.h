#pragma once

#include "nino.h"

class ApplicationLayer : public nino::Layer
{
public:
	ApplicationLayer() {};
	virtual ~ApplicationLayer() {};

	virtual void OnAttach() override;
	virtual void OnUpdate(nino::Timestep ts) override;
	virtual void OnEvent(nino::Event& event) override;
	virtual void RenderUserInterface() override;

};

