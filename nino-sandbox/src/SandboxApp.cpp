#include "nino.h"

#include "ApplicationLayer.h"

class SandboxApp : public nino::Application
{
public:
	SandboxApp() : Application(1280, 720) 
	{
		PushLayer(new ApplicationLayer());
	}
};

InitializeEngine(SandboxApp);


