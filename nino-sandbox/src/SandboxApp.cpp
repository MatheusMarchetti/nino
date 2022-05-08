#include "nino.h"

class SandboxApp : public nino::Application
{
public:
	SandboxApp() : Application(1280, 720) {}
};

InitializeEngine(SandboxApp);


