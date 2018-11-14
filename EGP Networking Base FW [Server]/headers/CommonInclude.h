#pragma once

// standard includes
#include <stdio.h>
#include <string.h>
#include <string>

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

//boids includes
#include "Define.h"
#include "Flock.h"

enum GameMessages
{
	DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
	REQUEST_CLIENT_DATA_MSG,
};

#pragma pack(push, 1)
struct BaseMessage
{
	unsigned int typeID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientData
{
	unsigned int typeID;

	Flock clientFlock;
	RakNet::SystemAddress clientIP;
	bool instantiated = false;
};
#pragma pack(pop)
