#include "nino.h"

#include "ApplicationLayer.h"

class SandboxApp : public nino::Application
{
public:
	SandboxApp() : Application(1280, 720) 
	{
		m_TestLayer = new ApplicationLayer();
		PushLayer(m_TestLayer);
	}

private:
	ApplicationLayer* m_TestLayer = nullptr;
};

InitializeEngine(SandboxApp);


