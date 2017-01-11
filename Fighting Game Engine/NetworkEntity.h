#pragma once
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "NetworkMessage.h"
#include "ConnectionSocket.h"

//A network entity is a collection of connections. It holds hosted servers, connection to other servers and remote connections to the hosted servers.

//I would use inheritance with NetworkClient and NetworkServer classes, but the difference between them is but a few lines.
//I did use separate child classes in my experiments before putting this here, and it only made it harder to iterate through them properly.
//Now, every network entity can have multiple servers that it can get clients from.
//Not entirely sure where this could be used. Potentially using different ports for different types of clients?

class NetworkEntity
{
	fd_set _readFDs, _writeFDs, _exceptFDs;
	double _pingOutTime, _pingInTime, _lastPing;
public:
	std::vector<ConnectionSocket> connections;

	double ping();

	bool ConnectTo(std::string ip, int port);
	bool HostServer(int port);
	void Disconnect(unsigned int index);
	void AcceptConnections(unsigned int index);
	void PopulateFDs();
	void SelectFDs();
	bool SendDataTo(unsigned int index, NetworkMessage msg);
	bool ReceiveDataFrom(unsigned int index);
	void HandleExceptions();
	bool ProcessQueuedMessageFrom(unsigned int index, NetworkMessage& out_msg);
	void BeginFrame();
	void EndFrame();

	NetworkEntity();
};

