#include "nino.h"

#include "ApplicationLayer.h"

class SandboxApp : public nino::Application
{
public:
	SandboxApp() : Application() 
	{
		m_TestLayer = new ApplicationLayer(this);
		PushLayer(m_TestLayer);
	}

private:
	ApplicationLayer* m_TestLayer = nullptr;
};

InitializeEngine(SandboxApp);


