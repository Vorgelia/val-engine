#pragma once
#include <WinSock2.h>
#include <queue>
#include <string>
#include "NetworkMessage.h"

enum class SocketMode
{
	Uninitialized,
	Initialized,
	Client,
	RemoteClient,
	Server,
	Disconnected
};

//class NetworkMessage;

class ConnectionSocket
{
	bool _SendData(std::string data);
public:
	std::string id;
	SocketMode mode;

	SOCKET sock;
	sockaddr_in saddr;

	bool strictSendOrder;

	std::queue<NetworkMessage> sendQueue;
	std::queue<NetworkMessage> receiveQueue;

	int port();

	static int GetLastSocketError();
	void Disconnect();
	void FlushSendQueue();
	bool SendData(NetworkMessage data);
	bool BindAsServer();
	bool ReceiveData();
	void DecodeStream(std::string stream);
	static ConnectionSocket AcceptConnectionFrom(SOCKET sock);
	bool Connect();
	bool Initialize();
	ConnectionSocket(in_addr ip, int port);
	ConnectionSocket();
};

