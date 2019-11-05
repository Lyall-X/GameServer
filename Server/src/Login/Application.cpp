#include "stdafx.h"

Application::Application()
{

}

Application::~Application()
{
}

bool Application::Initialize()
{
	INSTANCE(ConfigManager);
	INSTANCE(ConfigManager).reloadConfig();

	ServerConfig& cfg = INSTANCE(ConfigManager).getConfig("Login");
	mLoginServer = INSTANCE(Network).listen(cfg.Port);
	IF_FALSE(!mLoginServer)
		return false;

	LOG_DEBUG(LogSystem::csl_color_green, "Login listen Port:%d success", cfg.Port);


	return 0;
}


bool Application::Update()
{
	return 0;
}

bool Application::Destroy()
{
	return 0;
}
