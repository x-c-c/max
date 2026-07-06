#include "ServerStartStop.h"
#include <iostream>
#include <sys/epoll.h>
#include <fcntl.h>
void ServerStartStop::start(const ServerConfig& config)
{
	// socket, bind, listen
	serverSocketFileDescriptor	= socket(config.getDomain(), config.getType(), config.getProtocol());
	
	int opt = 1;
	setsockopt(serverSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	sockaddr_in	serverAddr{};
	serverAddr.sin_family		= config.getDomain();
	serverAddr.sin_addr.s_addr	= config.getAddr();
	serverAddr.sin_port			= htons(config.getPort()); 
	
	sockaddr*	serverAddrPtr	= reinterpret_cast<sockaddr*>(&serverAddr);
	socklen_t	serverAddrLen	= sizeof(serverAddr);
	bind(serverSocketFileDescriptor, serverAddrPtr, serverAddrLen);
	listen(serverSocketFileDescriptor, SOMAXCONN);

	// epoll
	int epollFileDescriptor		= epoll_create1(0);
	// add listening socket in epoll
	epoll_event	ev{};
	ev.events					= EPOLLIN;	// waiting for new connections
	ev.data.fd					= serverSocketFileDescriptor;
	epoll_ctl(epollFileDescriptor, EPOLL_CTL_ADD, serverSocketFileDescriptor, &ev);
	const int MAX_EVENTS			= 1024;
	epoll_event	events[MAX_EVENTS];
	// main loop handle events
	while(true)
	{
		// waiting for events on every registred socket (all time)
		int numEvents = epoll_wait(epollFileDescriptor, events, MAX_EVENTS, -1);
		for (int i = 0; i < numEvents; ++i)
		{
			int fd					= events[i].data.fd;
			if (fd == serverSocketFileDescriptor)
			{
				// new incoming connection
				sockaddr_in	clientAddr{};
				sockaddr*	clientPtr		= reinterpret_cast<sockaddr*>(&clientAddr);
				socklen_t	clientLen		= sizeof(clientAddr);
				clientSocketFileDescriptor	= accept(serverSocketFileDescriptor, clientPtr, &clientLen);
				// no block socket
				int flags	= fcntl(clientSocketFileDescriptor, F_GETFL, 0);
				fcntl(clientSocketFileDescriptor, F_SETFL, flags | O_NONBLOCK);
				// add client in epoll
				ev.events	= EPOLLIN;	// waiting for client post data
				ev.data.fd	= clientSocketFileDescriptor;
				epoll_ctl(epollFileDescriptor, EPOLL_CTL_ADD, clientSocketFileDescriptor, &ev);
				std::cout << "New client connected, fd = " << clientSocketFileDescriptor << std::endl;			
			}
			else
			{
				char buffer[4096];
				ssize_t bytesRead	= recv(fd, buffer, sizeof(buffer), 0);
				if (bytesRead <= 0)
				{
					if (bytesRead == 0){std::cout << "Client fd = " << fd << "disconnected" << std::endl;}
				// delete from epoll and close
				epoll_ctl(epollFileDescriptor, EPOLL_CTL_DEL, fd, nullptr);
				close(fd);
				}
				else
				{
					// echo
					send(fd, buffer, bytesRead, MSG_NOSIGNAL);
				}
			}
		}
	}
	
	
	close(epollFileDescriptor);
	close(serverSocketFileDescriptor);
}

void ServerStartStop::stop(){}
