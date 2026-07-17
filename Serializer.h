#pragma once
#include "Packets.h"
#include <cstdint>
#include <vector>
class Serializer
{
public:
	// сборка полного пакета [заголовок] + [тело]
	static std::vector<uint8_t> buildConnectRequest(	uint32_t messageID, uint32_t sessionID);
	static std::vector<uint8_t> buildConnectResponse(	uint32_t messageID, uint32_t sessionID);
	static std::vector<uint8_t> buildRegisterRequest(	uint32_t messageID, uint32_t sessionID, const RegisterRequestPacket& packet);
	static std::vector<uint8_t> buildRegisterResponse(	uint32_t messageID, uint32_t sessionID, const RegisterResponsePacket& packet);
	static std::vector<uint8_t> buildAuthRequest(		uint32_t messageID, uint32_t sessionID, const AuthRequestPacket& packet);
	static std::vector<uint8_t> buildAuthResponse(		uint32_t messageID, uint32_t sessionID, const AuthResponsePacket& packet);
	static std::vector<uint8_t> buildMessageSend(		uint32_t messageID, uint32_t sessionID, const MessageSendPacket& packet);
	static std::vector<uint8_t> buildDisconnectRequest(	uint32_t messageID, uint32_t sessionID);
	
private:
	static void appendBytes(std::vector<uint8_t>& dest, const void* src, size_t count);
	void writeUint16(std::vector<uint8_t>& buf, uint16_t val);
	void writeString(std::vector<uint8_t>& buf, const std::string& str);
};
