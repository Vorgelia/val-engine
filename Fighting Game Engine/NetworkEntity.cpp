#include "NetworkEntity.h"
#include "DebugLog.h"
#include "Time.h"
bool NetworkEntity::ConnectTo(std::string ip, int port){
	in_addr addr;
	InetPton(AF_INET,ip.c_str(),&addr);
	connections.push_back(ConnectionSocket(addr, port));
	return connections.back().Connect();
}

bool NetworkEntity::HostServer(int port){
	in_addr addr;
	addr.S_un.S_addr = INADDR_ANY;

	connections.push_back(ConnectionSocket(addr, port));
	return connections.back().BindAsServer();
}

void NetworkEntity::Disconnect(unsigned int index){
	if (index > -1 && index < connections.size()){
		connections[index].Disconnect();
		connections.erase(connections.begin() + index);
	}
}

void NetworkEntity::PopulateFDs(){
	//The sockets are used in non-blocking mode, which means their functions will return immediately without waiting for something to happen.
	//select() is used to determine which of the sockets are available for reading or writing, as well as checking for errors.
	//It's passed fd_set structs that are glorified arrays of sockets, and modifies them to only contain the valid ones.

	//Empty the FD sets
	FD_ZERO(&_readFDs);
	FD_ZERO(&_writeFDs);
	FD_ZERO(&_exceptFDs);
	if (connections.empty())
		return;
	//Add all sockets to the FD sets
	for (auto i = connections.begin(); i != connections.end(); ++i){
		switch (i->mode){
		case SocketMode::Client:
		case SocketMode::RemoteClient:
			FD_SET(i->sock, &_writeFDs);
		case SocketMode::Server://Omit local server sockets from being checked for being writable. So far i am not aware of a reason to write to local servers.
			FD_SET(i->sock, &_readFDs);
			FD_SET(i->sock, &_exceptFDs);
			break;
		}
	}
}

void NetworkEntity::AcceptConnections(unsigned int index){
	if (index < 0 || index >= connections.size())
		return;
	if (connections[index].mode==SocketMode::Server&&FD_ISSET(connections[index].sock, &_readFDs)){//Server sockets are read-valid if they have pending connections.
		ConnectionSocket conn = ConnectionSocket::AcceptConnectionFrom(connections[index].sock);
		if (conn.mode == SocketMode::Client || conn.mode == SocketMode::RemoteClient)//This just tells us if accepting the connection was successful.
			connections.push_back(conn);
		else
			conn.Disconnect();//If not, close the socket
	}
}

void NetworkEntity::SelectFDs(){
	if (connections.empty())
		return;
	timeval timeOut;
	timeOut.tv_sec = 0;
	timeOut.tv_usec = 0;//Select supports a timeOut argument that tells it how long it can wait until things happen. 0 just means poll the socket for activity and is advisable for realtime applications.
	//I'm not sure if there would be any benefits to increasing this value slightly.
	if (select(0, &_readFDs, &_writeFDs, &_exceptFDs, &timeOut) == SOCKET_ERROR){
		ConnectionSocket::GetLastSocketError();
	}
}
//These are self-explanatory. The specifics of sending and receiving will be explained in the ConnectionSocket class
bool NetworkEntity::SendDataTo(unsigned int index, NetworkMessage msg){
	if (index < 0 || index >= connections.size()||connections[index].mode == SocketMode::Server)
		return false;

	if (FD_ISSET(connections[index].sock, &_writeFDs)){
		bool rb = connections[index].SendData(msg);
		if (msg.type() == NetworkMessageType::PingOut){
			_lastPing = ping();
			_pingOutTime = Time::time;
		}
		return rb;
	}
	else{
		connections[index].sendQueue.push(msg);
		return false;
	}
}

bool NetworkEntity::ReceiveDataFrom(unsigned int index){
	if (index < 0 || index >= connections.size())
		return false;
	if (FD_ISSET(connections[index].sock, &_readFDs)){
		return connections[index].ReceiveData();
	}
	return false;
}
//Removes all sockets from the active connections if they're either invalid, or have been marked to have exceptions by select()
void NetworkEntity::HandleExceptions(){
	for (unsigned int i = 0; i < connections.size(); ++i){
		if (connections[i].mode == SocketMode::Disconnected || connections[i].mode == SocketMode::Uninitialized)
			Disconnect(i--);
		else if(FD_ISSET(connections[i].sock,&_exceptFDs)){
			char cErr[200];
			int len = 200;
			if (getsockopt(connections[i].sock, SOL_SOCKET, SO_ERROR, cErr, &len) == SOCKET_ERROR)
				ConnectionSocket::GetLastSocketError();
			else{
				DebugLog::Push("Socket Exception:"+std::string(cErr,cErr+len),2);
				Disconnect(i--);
			}
		}
	}
}
//The handling of messages should always go through this. It will get the next message in the receive queue and either handle it internally, returning false
//or copies it to the out_msg reference and returns true, signifying that it should be manually handled.
//PingOut and PingIn for instance always have the same behaviour, so it's better for them to be handled internally.
//Input frame and text messages on the other hand have different behaviour depending on the sender.
bool NetworkEntity::ProcessQueuedMessageFrom(unsigned int index, NetworkMessage& out_msg){
	if (connections[index].receiveQueue.empty())
		return false;
	out_msg = connections[index].receiveQueue.front();
	
	if (out_msg.State() != NetworkMessageState::Complete){
		if (out_msg.State() == NetworkMessageState::Invalid)
			connections[index].receiveQueue.pop();
		return false;
	}
	connections[index].receiveQueue.pop();
	switch (out_msg.type()){
	case NetworkMessageType::NameSet:
		connections[index].id = out_msg.data;
		return false;
	case NetworkMessageType::PingOut:
		connections[index].SendData(NetworkMessage(NetworkMessageType::PingIn));//Always send a ping in as a result of a ping out.
		//This results in a one-way ping with two messages quite nicely, but a two way ping needs four. It's easy to do two-way with three, so that's on my to-do list for the future.
		return false;
	case NetworkMessageType::PingIn:
		_pingInTime = Time::time;
		return false;
	case NetworkMessageType::PropertySet:
		return false;
	case NetworkMessageType::InputFrame:
	case NetworkMessageType::TextMessage:
		return true;
	}
	return false;
}

void NetworkEntity::BeginFrame(){
	PopulateFDs();
	SelectFDs();
	HandleExceptions();
	for (unsigned int i = 0; i < connections.size(); ++i){
		ReceiveDataFrom(i);
		if (connections[i].mode == SocketMode::Server)
			AcceptConnections(i);
	}
}

void NetworkEntity::EndFrame(){
	for (unsigned int i = 0; i < connections.size(); ++i){
		if (FD_ISSET(connections[i].sock, &_writeFDs))
			connections[i].FlushSendQueue();
	}
}

double NetworkEntity::ping(){
	if (_pingOutTime > _pingInTime)
		return _lastPing;
	return _pingInTime - _pingOutTime;
}

NetworkEntity::NetworkEntity(){
	_pingOutTime = _pingInTime = _lastPing = 0;
}