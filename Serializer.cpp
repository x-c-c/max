#include "Serializer.h"

static void Serializer::appendBytes(std::vector<uint8_t>& dest, const void* src, size_t count)
{
	const size_t offset = dest.size();
	dest.resize(offset + count);
	std::memcpy(dest.data() + offset, src, count);
}
void Serializer::writeUint16(std::vector<uint8_t>& buf, uint16_t val)
{
	const uint16_t net = htons(val);
	appendBytes(buf, &net, sizeof(net));
}
void Serializer::writeString(std::vector<uint8_t>& buf, const std::string& str)
{
	const uint16_t len = static_cast<uint16_t>(str.size());
	writeUint16(buf, len);
	appendBytes(buf, str.data(), len);
}



std::vector<uint8_t> Serializer::buildConnectRequest(uint32_t messageID, uint32_t sessionID)
{
	PacketHeaderRaw header;
	header.type_		= htons(static_cast<uint16_t>(PacketType::ConnectRequest));
	header.messageID_	= htonl(messageID);
	header.sessionID_	= htonl(sessionID);
	header.messageLen_	= 0;
	std::vector<uint8_t> result;
	appendBytes(result, &header, sizeof(header));
	return result;
}
std::vector<uint8_t> Serializer::buildConnectResponse(uint32_t messageID, uint32_t sessionID)
{
	PacketHeaderRaw header;
	header.type_		= htons(static_cast<uint16_t>(PacketType::ConnectResponse));
	header.messageID_	= htonl(messageID);
	header.sessionID_	= htonl(sessionID);
	header.messageLen_	= 0;
	std::vector<uint8_t> result;
	appendBytes(result, &header, sizeof(header));
	return result;
}
/*====================================================================*/
std::vector<uint8_t> Serializer::buildRegisterRequest(uint32_t messageID, uint32_t sessionID, const RegisterRequestPacket& packet)
{
	std::vector<uint8_t> body;
	writeString(body, packet.username);
	writeString(body, packet.password);
	
	PacketHeaderRaw header;
	header.type_		= htons(static_cast<uint16_t>(PacketType::RegisterRequest));
	header.messageID_	= htonl(messageID);
	header.sessionID_	= htonl(sessionID);
	header.messageLen_	= htons(static_cast<uint16_t>(body.size()));
	std::vector<uint8_t> result;
	appendBytes(result, &header, sizeof(header));
	appendBytes(result, body.data, body.size());
	
	return result;
}
















