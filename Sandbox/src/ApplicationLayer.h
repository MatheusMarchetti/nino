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
	virtual void RenderUI() override;

private:
	bool OnKeyPressed(nino::KeyPressedEvent& e);

private:
	nino::Ref<nino::Scene> m_TestScene;

	nino::GUILayer m_Layer;
};

