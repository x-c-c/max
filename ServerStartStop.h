#pragma once
#include "ServerConfig.h"
#include <sys/socket.h>
#include <unistd.h>

class ServerStartStop
{
private:
	int serverSocketFileDescriptor	= -1;
	int clientSocketFileDescriptor	= -1;
public:
	void start(const ServerConfig& config);
	void stop();
};

