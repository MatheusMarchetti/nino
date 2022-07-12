#pragma once

#include "nino.h"

class ApplicationLayer : public nino::Layer
{
public:
	ApplicationLayer(nino::Application* parent) : nino::Layer(parent) {};
	virtual ~ApplicationLayer() {};

	virtual void OnAttach() override;
	virtual void OnUpdate(nino::Timestep ts) override;
	virtual void OnEvent(nino::Event& event) override;
	virtual void RenderUserInterface() override;

private:
	float angle = 1.0f;
	nino::Ref<nino::Framebuffer> m_MainFramebuffer;
};

