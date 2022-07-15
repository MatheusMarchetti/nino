#include "nino.h"

#include "ApplicationLayer.h"

class SandboxApp : public nino::Application
{
public:
	SandboxApp(const nino::ApplicationDescriptor& descriptor) : Application(descriptor) 
	{
		m_TestLayer = new ApplicationLayer(this);
		PushLayer(m_TestLayer);
	}

private:
	ApplicationLayer* m_TestLayer = nullptr;
};

nino::ApplicationDescriptor descriptor("Sandbox", 1280, 720, false);

InitializeEngine(SandboxApp, descriptor);


