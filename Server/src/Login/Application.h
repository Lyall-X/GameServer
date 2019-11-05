#pragma once

class Application
{
public:
	Application();
	~Application();

	bool Initialize();
	bool Update();
	bool Destroy();
protected:
	SocketListener* mLoginServer;
};