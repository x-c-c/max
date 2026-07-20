#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Packets.h"
class ClientSession
{
private:
	int socketFd_;
	bool closed_ = false;
	std::vector<uint8_t> readBuffer_;
	static constexpr size_t MAX_BUFFER_SIZE = 64 * 1024;	// 64 kB
	
	void tryExtractPackets();
	bool extractPacket(PacketHeaderRaw& header, std::vector<uint8_t>& body);
	
	void processPacket(const PacketHeaderRaw& header, const std::vector<uint8_t>& body);
	void sendResponse(const std::vector<uint8_t>& data);

	// Обработчики конкретных пакетов
	void handleConnectRequestPacket(uint32_t messageID, uint32_t sessionID);
	void handleRegisterRequestPacket(uint32_t messageID, uint32_t sessionID, const RegisterRequestPacket& packet);
	void handleAuthRequestPacket(uint32_t messageID, uint32_t sessionID, const AuthRequestPacket& packet);
	void handleMessageSendPacket(uint32_t messageID, uint32_t sessionID, const MessageSendPacket& packet);
	void handleDisconnectRequestPacket();
	
public:
	explicit ClientSession(int socketFd);
	~ClientSession();
	
	void handleRead();
	void closeSession();

	int getSocketFd() const	{ return socketFd_; }
	bool isClosed() const { return closed_;	}
};
