#include "ClientSession.h"



ClientSession::ClientSession(int socketFd): socketFd_(socketFd){}
ClientSession::~ClientSession()
{
	if (!closed_)
		closeSession();
}

void ClientSession::handleRead()
{
	uint8_t tmp[4096];
	ssize_t bytesRead  = recv(socketFd_, tmp, sizeof(tmp), 0);
	if (bytesRead  > 0)
	{
		// Добавляем прочитанные байты в буфер
		size_t oldSize = readBuffer_.size();
		readBuffer_.resize(oldSize + bytesRead);
		std::memcpy(readBuffer_.data() + oldSize, tmp, bytesRead);
		
		// Пытаемся выделить целые пакеты
		tryExtractPackets();
	}
	else if(bytesRead == 0)
	{
		// Клиент корректно закрыл соединение
		std::cout << "[Client " << socketFd_ << "] connection closed by peer" << std::endl;
		closeSession();
	}
	else
	{
		// Ошибка чтения
		if (errno != EAGAIN && errno != EWOULDBLOCK)
		{
			perror("recv error");
			closeSession();
		}
	}
}
bool ClientSession::extractPacket(PacketHeaderRaw& header, std::vector<uint8_t>& body)
{
	if (readBuffer_.size() < sizeof(PacketHeaderRaw))	// Минимальный размер заголовка = 12 байт (на 20.07.2026)
		return false;
	
	// Копируем заголовок и сразу переводим все поля в хостовый порядок
	std::memcpy(&header, readBuffer_.data(), sizeof(header));
	header.type_       = ntohs(header.type_);
	header.messageID_  = ntohl(header.messageID_);
	header.sessionID_  = ntohl(header.sessionID_);
	header.messageLen_ = ntohs(header.messageLen_);

	size_t totalSize = sizeof(PacketHeaderRaw) + header.messageLen_;
	if (readBuffer_.size() < totalSize)					// Если прочитано байт меньше чем требуется для выделения целого пакета, то ждём ещё
		return false;

	// Извлекаем тело
	body.assign(readBuffer_.begin() + sizeof(PacketHeaderRaw), readBuffer_.begin() + totalSize);

	// Удаляем обработанные байты из буфера
	readBuffer_.erase(readBuffer_.begin(), readBuffer_.begin() + totalSize);
	return true;
}
void ClientSession::tryExtractPackets()
{
	PacketHeaderRaw header;
	std::vector<uint8_t> body;
	while (extractPacket(header, body))
	{
		processPacket(header, body);
		if (closed_)
			return;
	}
}
/*
void ClientSession::processPacket(const PacketHeaderRaw& header, const std::vector<uint8_t>& body)
{
	switch (static_cast<PacketType>(header.type_))
	{
		case PacketType::ConnectRequest:
		{
			ConnectRequestPacket packet;
			Serializer::parseConnectRequestPacket(body, packet);
			handleConnectRequest(header.messageID_, header.sessionID_);
		}
		break;

		case PacketType::RegisterRequest:
		{
			RegisterRequestPacket packet;
			if (Serializer::parseRegisterRequestPacket(body, packet))
				handleRegisterRequest(header.messageID_, header.sessionID_, packet);
		}
		break;

		case PacketType::AuthRequest:
		{
			AuthRequestPacket packet;
			if (Serializer::parseAuthRequestPacket(body, packet))
				handleAuthRequest(header.messageID_, header.sessionID_, packet);
		}
		break;

		case PacketType::MessageSend:
		{
			MessageSendPacket packet;
			if (Serializer::parseMessageSendPacket(body, packet))
				handleMessageSend(header.messageID_, header.sessionID_, packet);
		}
		break;

		case PacketType::DisconnectRequest:
			handleDisconnectRequest();
		break;

		default:
			std::cerr << "[Client " << socketFd_ << "] unknown packet type 0x" << std::hex << header.type_ << std::dec << std::endl;
		break;
	}
}
*/
/*
void ClientSession::sendResponse(const std::vector<uint8_t>& data);
// Обработчики конкретных пакетов
void ClientSession::handleConnectRequestPacket(uint32_t messageID, uint32_t sessionID);
void ClientSession::handleRegisterRequestPacket(uint32_t messageID, uint32_t sessionID, const RegisterRequestPacket& packet);
void ClientSession::handleAuthRequestPacket(uint32_t messageID, uint32_t sessionID, const AuthRequestPacket& packet);
void ClientSession::handleMessageSendPacket(uint32_t messageID, uint32_t sessionID, const MessageSendPacket& packet);
void ClientSession::handleDisconnectRequestPacket();
*/










void ClientSession::closeSession()
{
	if (closed_)
		return;
	closed_ = true;
	close(socketFd_);
	std::cout << "[Client " << socketFd_ << "] session closed" << std::endl;
}



