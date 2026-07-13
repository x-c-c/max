#pragma once
#include <cstdint>
#include <string>

enum class PacketType : uint16_t
{
	ConnectRequest	= 0x1,
	ConnectResponse,
	RegisterRequest,
	RegisterResponse,
	AuthRequest,
	AuthResponse,
	MessageSend,
	DisconnectRequest
};

// общий заголовок для всех пакетов
struct PacketHeader
{
public:
	PacketType type_;
	uint32_t messageID_  = 0;
	uint32_t sessionID_  = 0;
	uint16_t messageLen_ = 0;
	explicit PacketHeader(PacketType t): type_(t){}
};

struct ConnectRequestPacket
{
	PacketHeader header;
	ConnectRequestPacket(): header(PacketType::ConnectRequest){}
};
struct ConnectResponsePacket
{
	PacketHeader header;
	ConnectResponsePacket(): header(PacketType::ConnectResponse){}
};

struct RegisterRequestPacket
{
	PacketHeader header;
	std::string username;
	std::string password;	// change later to hash or smth
	RegisterRequestPacket(): header(PacketType::RegisterRequest){}
};
struct RegisterResponsePacket
{
	PacketHeader header;
	RegisterResponsePacket(): header(PacketType::RegisterResponse){}
};

struct AuthRequestPacket
{
	PacketHeader header;
	std::string username;
	std::string password;	// change later to hash or smth
	AuthRequestPacket(): header(PacketType::AuthRequest){}
};
struct AuthResponsePacket
{
	PacketHeader header;
	AuthResponsePacket(): header(PacketType::AuthResponse){}
};

struct MessageSend
{
	PacketHeader header;
};

struct DisconnectRequestPacket
{
	PacketHeader header;
	DisconnectRequestPacket(): header(PacketType::DisconnectRequest) {}
};


