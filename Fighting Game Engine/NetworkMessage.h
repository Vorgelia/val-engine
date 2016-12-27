#pragma once
#include <string>
#include <vector>
enum class NetworkMessageType{
	Unidentified = 0,
	PingOut = 1, //Sent as a general use ping
	PingIn = 2, //Sent as a response to a PingOut
	TextMessage = 3,
	NameSet = 4, //Will internally set the ID for connections to its data
	PropertySet = 5,//To be used later for stuff like icons or whatever.
	InputFrame = 6 //Data 
};

enum class NetworkMessageState{
	Invalid = -1,
	Incomplete = 0,
	Complete = 1
};
//Messages are composed of two parts. Identifiers and data. Identifiers can be thought of as the "managed" part of a message. One whose length is known beforehand.
//The length of data can be arbitrary, as long as it can be stored within two bytes. In messages with data, the identifiers will have to hold the length.
//An alternative to storing message length would be to append the message with a special character, but that's a bit more prone to errors or exploits.
class NetworkMessage
{
	//Raw data identifiers
	std::string _dataIdentifiers;
	//Data from the identifiers
	unsigned int _size;
	NetworkMessageType _type;
public:
	static std::vector<unsigned int> g_identifierSizes;

	std::string data;

	NetworkMessageType type();
	std::string Serialize();
	void Append_Raw(std::string str);
	bool Append(std::string str);
	NetworkMessageState State();
	int MissingDataSize();

	NetworkMessage(std::string dat);
	NetworkMessage(NetworkMessageType type, std::string data="");
	~NetworkMessage();
};