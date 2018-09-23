#include "ConnectionSocket.h"
#include "NetworkMessage.h"
#include "DebugLog.h"

int ConnectionSocket::GetLastSocketError()
{
	const int err = WSAGetLastError();
	//We're using sockets in non-blocking mode, so if a function like recv is called without something to receive it'll have an error of WSAEWOULDBLOCK
	//Naturally, we don't want that to be treated as an actual error reported to the user.
	if(err > 0 && err != WSAEWOULDBLOCK)
	{
		//VE_LOG("Socket Error: " + std::to_string(err));
		return err;
	}
	return 0;
}

void ConnectionSocket::Disconnect()
{
	closesocket(sock);
	mode = SocketMode::Uninitialized;
}

//Sends data in the send queue until it encounters an error
void ConnectionSocket::FlushSendQueue()
{
	while(!sendQueue.empty())
	{
		if(!_SendData(sendQueue.front().Serialize()))
			return;
		sendQueue.pop();
	}
}

//Lower level Send Data, used when we need to bypass the behaviours in SendData, like putting messages that failed to send in the sendQueue.
bool ConnectionSocket::_SendData(std::string data) const
{
	if(send(sock, data.c_str(), data.length(), 0) != data.length())
	{
		GetLastSocketError();
		return false;
	}
	return true;
}

//Attempt to send the data, storing the NetworkMessage in the send queue if the operation fails.
bool ConnectionSocket::SendData(NetworkMessage data)
{
	//Strict send order makes it so that we don't send data if we have earlier unsent data. Unless it's a ping.
	if(strictSendOrder && (data.type() != NetworkMessageType::PingOut&&data.type() != NetworkMessageType::PingIn) && !sendQueue.empty())
	{
		sendQueue.push(data);
		return false;
	}
	if(!_SendData(data.Serialize()))
	{
		if(data.type() != NetworkMessageType::PingOut&&data.type() != NetworkMessageType::PingIn)
		{
			sendQueue.push(data);
			//if(sendQueue.size() > 64)
			//	VE_LOG("Send queue size warning. SocketID:" + id + " SendQueue size:" + std::to_string(sendQueue.size()));
		}
		return false;
	}
	return true;
}

bool ConnectionSocket::BindAsServer()
{
	if(bind(sock, (const sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		GetLastSocketError();
		return false;
	}
	listen(sock, 100);
	mode = SocketMode::Server;
	return true;
}

bool ConnectionSocket::ReceiveData()
{
	char rec_msg[2048];

	//Receive a message, with a maximum length of 2048.
	int msgLen = recv(sock, rec_msg, 2048, 0);

	//If there is nothing to be received, a WSAEWOULDBLOCK error is raised and recv returns -1. It only returns 0 if the client has disconnected.
	if(msgLen == 0)
	{
		mode = SocketMode::Disconnected;
		return false;
	}
	else if(msgLen == SOCKET_ERROR)
	{
		GetLastSocketError();
		return false;
	}
	DecodeStream(std::string(rec_msg, rec_msg + msgLen));
	return true;
}

//This is a fun one.
void ConnectionSocket::DecodeStream(std::string stream)
{
	if(stream.length() == 0)
	{
		//VE_LOG("Attempting to decode null stream.");
		return;
	}
	unsigned int currentIndex = 0;
	unsigned int nextIndex = 0;
	while(currentIndex < stream.length())
	{
		if(!receiveQueue.empty() && receiveQueue.back().State() == NetworkMessageState::Incomplete)
		{
			//If the last message in the receive queue is incomplete, what we have now must be the data it's missing.
			nextIndex = min(stream.length(), currentIndex + receiveQueue.back().MissingDataSize());//Make sure the amount of characters we use isn't more than the stream has left.
			receiveQueue.back().Append_Raw(stream.substr(currentIndex, nextIndex - currentIndex));
		}
		else
		{//Otherwise, check the type at currentIndex and create a network message with however many identifiers this type of message has
			//If the stream doesn't have that many characters left, or has more data than the identifiers, the previous block will catch it next time we iterate. \o/
			switch(static_cast<NetworkMessageType>(stream[currentIndex]))
			{
			case NetworkMessageType::PingOut:
			case NetworkMessageType::PingIn:
				receiveQueue.push(NetworkMessage(static_cast<NetworkMessageType>(stream[currentIndex])));
				nextIndex = currentIndex + 1;
				break;
			case NetworkMessageType::TextMessage:
			case NetworkMessageType::NameSet:
			case NetworkMessageType::PropertySet:
			case NetworkMessageType::InputFrame:
				nextIndex = min(stream.length(), currentIndex + NetworkMessage::g_identifierSizes[(int)stream[currentIndex]]);
				receiveQueue.push(NetworkMessage(stream.substr(currentIndex, nextIndex - currentIndex)));
				break;
			default:
				//VE_LOG("Invalid message type on decoding. Something went very wrong.");
				return;
			}
		}
		currentIndex = nextIndex;//Advance currentIndex by however many characters we used in this iteration step.
	}
}

ConnectionSocket ConnectionSocket::AcceptConnectionFrom(SOCKET sock)
{
	ConnectionSocket cs = ConnectionSocket();
	if((cs.sock = accept(sock, (sockaddr*)&cs.saddr, nullptr)) == SOCKET_ERROR)
	{
		GetLastSocketError();
		cs.mode = SocketMode::Uninitialized;
	}
	else
	{
		u_long sMode = 1;
		if(ioctlsocket(cs.sock, FIONBIO, &sMode) == SOCKET_ERROR)
		{//I don't know how this operation could result in an error on a fresh socket besides an invalid sMode, but i'm checking it anyway.
			GetLastSocketError();
			cs.Disconnect();
		}
		else
		{
			cs.mode = SocketMode::RemoteClient;
		}
	}
	return cs;
}

bool ConnectionSocket::Connect()
{
	if(connect(sock, (const sockaddr*)&saddr, sizeof(saddr)) == SOCKET_ERROR&&GetLastSocketError() != 0)
		return false;
	mode = SocketMode::Client;
	return true;
}

bool ConnectionSocket::Initialize()
{
	if(mode != SocketMode::Uninitialized)
		Disconnect();
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		GetLastSocketError();
		return false;
	}
	else
	{
		u_long sMode = 1;
		if(ioctlsocket(sock, FIONBIO, &sMode) == SOCKET_ERROR)
		{
			GetLastSocketError();
			mode = SocketMode::Disconnected;//Make it act as if it's valid but not connected, even though it was never connected.
			return false;
		}
		else
		{
			mode = SocketMode::Initialized;
			return true;
		}
	}
}

int ConnectionSocket::port() const
{
	return ntohs(saddr.sin_port);
}

ConnectionSocket::ConnectionSocket(in_addr ip, int port)
{
	saddr.sin_addr = ip;
	saddr.sin_port = htons(port);
	saddr.sin_family = AF_INET;
	strictSendOrder = true;
	mode = SocketMode::Uninitialized;
	Initialize();
}

ConnectionSocket::ConnectionSocket()
{
	strictSendOrder = true;
	mode = SocketMode::Uninitialized;
}

