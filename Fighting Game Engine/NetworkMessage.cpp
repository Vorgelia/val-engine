#include "NetworkMessage.h"
#include "DebugLog.h"
#include <assert.h>



std::vector<unsigned int> NetworkMessage::g_identifierSizes = { 1, 1, 1, 3, 3, 3, 5 };//Amount of bits that hold information on every message type. Includes the type identifying bit.
/*
Unidentified, 1 bit containing the type to overwrite the current one with
PingOut, 1 bit containing type
PingIn, 1 bit containing type
TextMessage, 1 bit containing type, 1 bit containing the message size high byte, 1 bit containing the message size low byte. The identifiers act as a prefix to the message bytes
NameSet, Same as above, but handled differently internally.
PropertySet, 1 bit containing type, 1 bit containing property ID, 1 bit containing value
InputFrame, 1 bit containing type, 1 bit containing frame ID low byte, 1 bit containing frame ID high byte, 1 bit containing direction mask, 1 bit containing button mask

The integrity of data received over the network could be unpredictable. The identifying bits themselves as well as their amount is used to check if a message is complete.
For instance. A NetworkMessage of type TextMessage whose identifying bits say is 60 characters long will consider itself incomplete if its data is shorter than that.
Ooooor if it has less than 3 identifying bits to begin with.
*/

NetworkMessageType NetworkMessage::type()
{
	return _type;
}

std::string NetworkMessage::Serialize()
{
	return _dataIdentifiers + data;
}
//This function appends incoming data to the network message, filling in incomplete data identifiers from it if the message is incomplete
void NetworkMessage::Append_Raw(std::string str)
{
	for(unsigned int index = 0; index < str.length(); ++index)
	{
		if(_dataIdentifiers.empty())
		{
			assert(_type == NetworkMessageType::Unidentified);//If data identifiers are empty, type has to be unidentified. Otherwise, something baaaaad happened.
			_type = static_cast<NetworkMessageType>(str[index]);
			_size = 0;
			data = "";
		}
		else
		{
			switch(_type)
			{
				case NetworkMessageType::NameSet:
				case NetworkMessageType::TextMessage:
					switch(_dataIdentifiers.size())
					{
						case 1:
							_size += static_cast<unsigned char>(str[index]);
							break;
						case 2:
							_size += static_cast<unsigned char>(str[index]) << 8;
							break;
						case 3://When all data identifiers are complete, just store the rest of the string to data and stop.
							data += str.substr(index, str.length() - index);
							return;
					}
					break;
				case NetworkMessageType::PingOut:
				case NetworkMessageType::PingIn:
				case NetworkMessageType::PropertySet:
				case NetworkMessageType::InputFrame:
					if(_dataIdentifiers.length() == g_identifierSizes[static_cast<int>(_type)])
						return;//These aren't meant to have any data associated with them, so after the identifiers are complete, discard the rest and stop.
					break;
				default:
					DebugLog::Push("ERROR: Invalid NetworkMessage type");
					break;
			}
		}
		_dataIdentifiers += str[index];
	}

}

//For simpler appending to data with error checking. Not sure where this could be used.
bool NetworkMessage::Append(std::string str)
{
	if((_type == NetworkMessageType::Unidentified) || _dataIdentifiers.length() != g_identifierSizes[static_cast<int>(_type)])
	{
		DebugLog::Push("ERROR: Incomplete identifiers");
		return false;
	}
	data.append(str);
	return true;
}

NetworkMessageState NetworkMessage::State()
{
	if(_dataIdentifiers.length() < g_identifierSizes[static_cast<int>(_type)])
		return NetworkMessageState::Incomplete;
	else if(_dataIdentifiers.length() > g_identifierSizes[static_cast<int>(_type)])
		return NetworkMessageState::Invalid;
	if(data.length() < _size)
		return NetworkMessageState::Incomplete;
	else if(data.length()>_size)
		return NetworkMessageState::Invalid;
	return NetworkMessageState::Complete;
}

int NetworkMessage::MissingDataSize()
{//Almost equivalent to State(), except State() produces more readable results and is less error-prone.
	return (g_identifierSizes[static_cast<int>(_type)] - _dataIdentifiers.length()) + (_size - data.length());
}

//Creates a message from raw data
NetworkMessage::NetworkMessage(std::string dat)
{
	_type = NetworkMessageType::Unidentified;
	Append_Raw(dat);
}

//Creates a message with the specified data, generating its identifiers from the type.
NetworkMessage::NetworkMessage(NetworkMessageType type, std::string data)
{
	_type = type;
	_size = 0;
	this->data = "";
	switch(_type)
	{
		case NetworkMessageType::NameSet:
		case NetworkMessageType::TextMessage:
			this->data = data;
			_dataIdentifiers.push_back((char)type);
			_dataIdentifiers.push_back((char)data.size());
			_dataIdentifiers.push_back((char)(data.size() >> 8));
			_size = data.size();
			return;
		case NetworkMessageType::PingOut:
		case NetworkMessageType::PingIn:
			return;
		case NetworkMessageType::PropertySet:
		case NetworkMessageType::InputFrame:
			_dataIdentifiers.push_back((char)type);
			_dataIdentifiers.append(data);
			return;
		default:
			DebugLog::Push("ERROR: Invalid NetworkMessage type");
			break;
	}
}

NetworkMessage::~NetworkMessage()
{
}
