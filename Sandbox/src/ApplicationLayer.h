#pragma once

#include "nino.h"

#include "CustomEvents.h"

class ApplicationLayer : public nino::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUpdate(nino::Timestep ts) override;
	virtual void OnEvent(nino::Event& event) override;
	virtual void RenderUI() override;

private:
	bool OnPlayerMoved(PlayerMovedEvent& event);

private:
	float m_CubeRotationX = 0.0f;
	float m_CubeRotationY = 0.0f;
	float m_CubeRotationZ = 0.0f;
	float m_CubeSizeX = 0.0f;
	float m_CubeSizeY = 0.0f;
	float m_CubeSizeZ = 0.0f;
	float m_CubePositionX = 0.0f;
	float m_CubePositionY = 0.0f;
	float m_CubePositionZ = 0.0f;
};

