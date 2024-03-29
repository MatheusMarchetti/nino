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
	nino::Ref<nino::Framebuffer> m_MainFramebuffer;
	nino::Camera m_MainCamera;
};

